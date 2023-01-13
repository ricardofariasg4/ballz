#ifndef LISTA_H
#define LISTA_H

#include "estruturas.h"

typedef struct _NODO {
    QUADRADO *quadrado;
    BOLA *bola;
    struct _NODO *proximo;
} NODO;

typedef struct _LISTA {
    NODO *primeiro_quadrado;
    NODO *primeira_bola;
    int qtd_quadrados;
    int qtd_bolas;
    int bola_kills;
} LISTA;

LISTA* criar_lista ();
void insere_quadrado (QUADRADO *quadrado, LISTA *lista);
void insere_bola (BOLA *bola, LISTA *lista);
void remove_quadrado (QUADRADO *quadrado, LISTA *lista);
bool secao_vazia (LISTA *lista);
void exibe_lista_bola (LISTA *lista);
void redefinir_origem_bolas(LISTA *lista);
void destruir_lista (LISTA *lista, int formato);

#endif