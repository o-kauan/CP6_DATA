#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hanoi.h"

static Torre A={'A',NULL},B={'B',NULL},C={'C',NULL};
static Movimento *historico=NULL;
static int quantidadeDiscos=4,inicializado=0;

static Torre *torrePorNome(char n){if(n=='A')return &A;if(n=='B')return &B;if(n=='C')return &C;return NULL;}
static void imprimirDiscos(No *topo){int primeiro=1;while(topo){if(!primeiro)printf(",");printf("%d",topo->disco);primeiro=0;topo=topo->proximo;}}
static int minimoMovimentos(int n){return (1<<n)-1;}
static void respostaEstado(const char *msg){
    printf("OK|A=");imprimirDiscos(A.topo);printf("|B=");imprimirDiscos(B.topo);printf("|C=");imprimirDiscos(C.topo);
    printf("|MOVES=%d|MIN=%d|WIN=%d|MESSAGE=%s\n",contarMovimentos(historico),minimoMovimentos(quantidadeDiscos),verificarVitoria(&C,quantidadeDiscos),msg?msg:"");fflush(stdout);
}
static void respostaHistorico(void){
    printf("HISTORY|");Movimento *m=historico;int primeiro=1;
    while(m){if(!primeiro)printf(";");printf("%d,%c,%c",m->disco,m->origem,m->destino);primeiro=0;m=m->proximo;}
    printf("\n");fflush(stdout);
}

int main(void){
    char linha[128];
    while(fgets(linha,sizeof(linha),stdin)){
        linha[strcspn(linha,"\r\n")]='\0';
        if(strncmp(linha,"INIT ",5)==0||strncmp(linha,"RESET ",6)==0){
            int n=atoi(strchr(linha,' ')+1);
            if(n<3||n>8){printf("ERROR|INVALID_DISK_COUNT|MESSAGE=Use de 3 a 8 discos.\n");fflush(stdout);continue;}
            reiniciarJogo(&A,&B,&C,&historico,n);quantidadeDiscos=n;inicializado=1;respostaEstado("Jogo inicializado.");continue;
        }
        if(strcmp(linha,"STATE")==0){if(!inicializado){printf("ERROR|NOT_INITIALIZED|MESSAGE=Inicialize o jogo primeiro.\n");fflush(stdout);}else respostaEstado("Estado atual.");continue;}
        if(strcmp(linha,"HISTORY")==0){respostaHistorico();continue;}
        if(strncmp(linha,"MOVE ",5)==0){
            char o=0,d=0;if(sscanf(linha+5," %c %c",&o,&d)!=2){printf("ERROR|INVALID_COMMAND|MESSAGE=Formato: MOVE A C\n");fflush(stdout);continue;}
            Torre *origem=torrePorNome(o),*destino=torrePorNome(d);
            if(!inicializado){printf("ERROR|NOT_INITIALIZED|MESSAGE=Inicialize o jogo primeiro.\n");fflush(stdout);}
            else if(!origem||!destino||origem==destino){printf("ERROR|INVALID_TOWER|MESSAGE=Use torres A, B ou C distintas.\n");fflush(stdout);}
            else{int st=moverDisco(origem,destino,&historico);if(st==1)respostaEstado("Movimento realizado.");else if(st==-1){printf("ERROR|MEMORY_ERROR|MESSAGE=Falha de memoria.\n");fflush(stdout);}else{printf("ERROR|INVALID_MOVE|MESSAGE=Movimento invalido.\n");fflush(stdout);}}
            continue;
        }
        if(strcmp(linha,"QUIT")==0)break;
        printf("ERROR|INVALID_COMMAND|MESSAGE=Comando desconhecido.\n");fflush(stdout);
    }
    liberarTorre(&A.topo);liberarTorre(&B.topo);liberarTorre(&C.topo);liberarHistorico(&historico);
    return 0;
}
