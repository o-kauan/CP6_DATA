#!/usr/bin/env python3
from http.server import ThreadingHTTPServer, SimpleHTTPRequestHandler
from pathlib import Path
from urllib.parse import urlparse
import json, os, subprocess, threading

BASE=Path(__file__).resolve().parents[1]
FRONTEND=BASE/"frontend"
ENGINE=BASE/"bin"/("hanoi_engine.exe" if os.name=="nt" else "hanoi_engine")
PORT=int(os.environ.get("HANOI_PORT","8080"))
engine=None
lock=threading.Lock()

def start_engine():
    global engine
    if not ENGINE.exists(): return False
    if engine is None or engine.poll() is not None:
        engine=subprocess.Popen([str(ENGINE)],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True,bufsize=1)
    return True

def parse_list(value):
    return [] if value=="" else [int(x) for x in value.split(",") if x.strip()]

def parse_state(parts):
    state={}
    for part in parts:
        if "=" not in part: continue
        k,v=part.split("=",1)
        if k in ("A","B","C"): state[k]=parse_list(v)
        elif k in ("MOVES","MIN","WIN"): state[k]=int(v or "0")
    return state

def parse_raw(raw):
    parts=raw.strip().split("|");kind=parts[0] if parts else "ERROR"
    if kind=="OK":
        message="";fields=[]
        for p in parts[1:]:
            if p.startswith("MESSAGE="): message=p.split("=",1)[1]
            else: fields.append(p)
        return {"ok":True,"raw":raw.strip(),"state":parse_state(fields),"message":message}
    if kind=="HISTORY":
        history=[];payload=parts[1] if len(parts)>1 else ""
        if payload:
            for item in payload.split(";"):
                f=item.split(",")
                if len(f)==3: history.append({"disk":int(f[0]),"from":f[1],"to":f[2]})
        return {"ok":True,"raw":raw.strip(),"history":history}
    code=parts[1] if len(parts)>1 else "ENGINE_ERROR";message=""
    for p in parts[2:]:
        if p.startswith("MESSAGE="): message=p.split("=",1)[1]
    return {"ok":False,"raw":raw.strip(),"code":code,"message":message or code}

class Handler(SimpleHTTPRequestHandler):
    def translate_path(self,path):
        rel=urlparse(path).path.lstrip("/") or "index.html"
        return str(FRONTEND/rel)
    def end_headers(self):
        self.send_header("Access-Control-Allow-Origin","*")
        self.send_header("Access-Control-Allow-Headers","Content-Type")
        self.send_header("Access-Control-Allow-Methods","GET,POST,OPTIONS")
        super().end_headers()
    def do_OPTIONS(self):
        self.send_response(204);self.end_headers()
    def do_GET(self):
        if self.path=="/api/ping":
            ok=start_engine()
            return self._json(200 if ok else 503,{"ok":ok})
        return super().do_GET()
    def do_POST(self):
        if self.path!="/api/command": return self._json(404,{"ok":False,"message":"Not found"})
        length=int(self.headers.get("Content-Length","0"))
        payload=json.loads(self.rfile.read(length) or b"{}")
        command=str(payload.get("command","")).strip()
        if not command:return self._json(400,{"ok":False,"message":"Comando vazio"})
        if not start_engine():return self._json(503,{"ok":False,"message":"Compile o motor C primeiro."})
        with lock:
            try:
                engine.stdin.write(command+"\n");engine.stdin.flush()
                raw=engine.stdout.readline().strip()
                if not raw:
                    err=engine.stderr.readline().strip()
                    return self._json(500,{"ok":False,"message":err or "O motor C nao respondeu."})
                data=parse_raw(raw)
                return self._json(200 if data.get("ok") else 422,data)
            except Exception as exc:
                return self._json(500,{"ok":False,"message":str(exc)})
    def _json(self,status,data):
        body=json.dumps(data,ensure_ascii=False).encode("utf-8")
        self.send_response(status);self.send_header("Content-Type","application/json; charset=utf-8")
        self.send_header("Content-Length",str(len(body)));self.end_headers();self.wfile.write(body)

if __name__=="__main__":
    print(f"Hanoi Bridge - http://127.0.0.1:{PORT}")
    print("Engine:",ENGINE)
    ThreadingHTTPServer(("127.0.0.1",PORT),Handler).serve_forever()
