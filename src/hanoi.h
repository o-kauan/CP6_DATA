#ifndef HANOI_H
#define HANOI_H
typedef struct No No;
struct No { int disco; No *proximo; };
typedef struct { char nome; No *topo; } Torre;
typedef struct Movimento Movimento;
struct Movimento { int disco; char origem; char destino; Movimento *proximo; };

int push(No **topo,int disco);
int pop(No **topo);
int peek(No *topo);
int torreVazia(No *topo);
int moverDisco(Torre *origem,Torre *destino,Movimento **historico);
int verificarVitoria(Torre *destino,int quantidadeDiscos);
int registrarMovimento(Movimento **historico,char origem,char destino,int disco);
int contarMovimentos(Movimento *historico);
void liberarTorre(No **topo);
void liberarHistorico(Movimento **historico);
int inicializarJogo(Torre *a,Torre *b,Torre *c,int quantidadeDiscos);
void reiniciarJogo(Torre *a,Torre *b,Torre *c,Movimento **historico,int quantidadeDiscos);
#endif
