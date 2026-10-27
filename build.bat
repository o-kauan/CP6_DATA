@echo off
if not exist bin mkdir bin
gcc -Wall -Wextra -std=c11 -O2 src\main.c src\hanoi.c -o bin\hanoi_engine.exe
if errorlevel 1 (
  echo ERRO NA COMPILACAO.
  pause
  exit /b 1
)
echo Compilacao concluida.
echo Execute: python bridge\bridge.py
pause
