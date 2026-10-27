/*
 * ============================================================
 *  HANOI ENGINE - Checkpoint Final
 *  Data Structures and Algorithms em C | FIAP
 * ============================================================
 *  Integrantes:
 *    - Guilherme Figueira Velloso ................ RM 568827
 *    - José Augusto Ribeiro Freire Manfrinato .... RM 571151
 *    - Lais da Silva Dias ........................ RM 569943
 *    - João Augusto Poloniato Telles ............. RM 571443
 *    - Thiago Soalheiro Diamantino ............... RM 569316
 *    - Kauan Damasceno de Lima ................... RM 573727
 *
 *  Repositório: https://github.com/o-kauan/CP6_DATA
 * ============================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include "hanoi.h"

int push(No **topo, int disco) {
    No *novo = (No*)malloc(sizeof(No));
    
    if (novo == NULL) {
        return 0;
    }
    
    novo->disco = disco;
    novo->proximo = *topo;
    *topo = novo;
    
    return 1;
}

int pop(No **topo) {
    if (*topo == NULL) {
        return -1;
    }

    No *remover = *topo;
    int disco = remover->disco;
    *topo = (*topo)->proximo;
    
    free(remover);   
    return disco;
}

int peek(No *topo) {
    if (topo == NULL) return -1;
    return topo->disco;
}

int torreVazia(No *topo) {
    return topo == NULL;
}

int registrarMovimento(Movimento **historico, char origem, char destino, int disco) {
    Movimento *novo = (Movimento*)malloc(sizeof(Movimento));

    if (novo == NULL) {
        return 0;
    }

    novo->disco = disco;
    novo->origem = origem;
    novo->destino = destino;
    novo->proximo = NULL;

    if (*historico == NULL) {
        *historico = novo;
    } else {
        Movimento *atual = *historico;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novo;
    }

    return 1;
}

int contarMovimentos(Movimento *historico) {
    int contador = 0;
    Movimento *atual = historico;

    while (atual != NULL) {
        contador += 1;
        atual = atual->proximo;
    }
    return contador;
}

int moverDisco(Torre *origem, Torre *destino, Movimento **historico) {
    if (origem == NULL || destino == NULL) {
        return 0;
    }
    if (origem == destino) {
        return 0;
    }
    if (origem->topo == NULL) {
        return 0;
    }
    int disco = origem->topo->disco;

    if (destino->topo != NULL) {
        if (disco >= destino->topo->disco) {
            return 0;
        }
    }
    pop(&origem->topo);
    push(&destino->topo, disco);

    if (registrarMovimento(historico, origem->nome, destino->nome, disco) == 0) {
        return -1;
    }
    return 1;
}

int verificarVitoria(Torre *destino, int quantidadeDiscos) {
    if (destino == NULL || destino->topo == NULL) return 0;

    No *atual = destino->topo;
    int discoEsperado = 1;
    int contador = 0;

    while (atual != NULL) {
        if (atual->disco != discoEsperado) {
            return 0;
        }
        discoEsperado++;
        contador++;
        atual = atual->proximo;
    }

    return (contador == quantidadeDiscos);
}

void liberarTorre(No **topo) {
    if (topo == NULL || *topo == NULL) return;

    No *atual = *topo;
    while (atual != NULL) {
        No *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }

    *topo = NULL;
}

void liberarHistorico(Movimento **historico) {
    if (historico == NULL || *historico == NULL) return;

    Movimento *atual = *historico;
    while (atual != NULL) {
        Movimento *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }

    *historico = NULL;
}

int inicializarJogo(Torre *a, Torre *b, Torre *c, int quantidadeDiscos) {
    if (quantidadeDiscos < 3 || quantidadeDiscos > 8) {
        return 0;
    }

    a->topo = NULL;
    b->topo = NULL;
    c->topo = NULL;

    a->nome = 'A';
    b->nome = 'B';
    c->nome = 'C';

    for (int disco = quantidadeDiscos; disco >= 1; disco--) {
        push(&a->topo, disco);
    }

    return 1;
}

void reiniciarJogo(Torre *a, Torre *b, Torre *c, Movimento **historico, int quantidadeDiscos) {
    if (a) liberarTorre(&(a->topo));
    if (b) liberarTorre(&(b->topo));
    if (c) liberarTorre(&(c->topo));

    liberarHistorico(historico);

    inicializarJogo(a, b, c, quantidadeDiscos);
}