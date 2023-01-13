#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static void confere_malloc (bool teste, char *descricao)
{
    if (teste) return;
    
    perror(descricao);
    exit(1);
}

static bool lista_vazia (LISTA *l, int formato)
{
    if (formato==0)
        if (l->primeiro_quadrado) return 0;
    
    if (formato==1)
        if (l->primeira_bola) return 0;
    
    return 1;
}

static NODO* criar_nodo (QUADRADO *quadrado, BOLA *bola, int formato)
{
    if (formato==0)
    {
        NODO *nodo = (NODO *) malloc(sizeof(NODO));
        confere_malloc(nodo, "lista.c->criar_nodo()");

        nodo->quadrado = quadrado;
        nodo->bola = NULL;
        nodo->proximo = NULL;

        return nodo;
    }

    else if (formato==1)
    {
        NODO *nodo = (NODO *) malloc(sizeof(NODO));
        confere_malloc(nodo, "lista.c->criar_nodo()");

        nodo->bola = bola;
        nodo->quadrado = NULL;
        nodo->proximo = NULL;

        return nodo;
    }

    else
        return NULL;
}

LISTA* criar_lista ()
{    
    LISTA *l = (LISTA *) malloc(sizeof(LISTA));
    confere_malloc(l, "lista.c->criar_lista()");

    l->primeiro_quadrado = NULL;
    l->primeira_bola = NULL;
    l->qtd_quadrados = 0;
    l->qtd_bolas = 0;
    l->bola_kills = 0;

    return l;
}

void insere_bola (BOLA *bola, LISTA *lista)
{
    NODO *novo_bola = criar_nodo(NULL, bola, 1);

    if (lista_vazia(lista, 1))
        lista->primeira_bola = novo_bola;
    else
    {
        //Verificando a posição de inserção
        NODO *aux = lista->primeira_bola;

        while (aux->proximo != NULL)
            aux = aux->proximo;

        aux->proximo = novo_bola;
    }
}

void insere_quadrado (QUADRADO *quadrado, LISTA *lista)
{
    NODO *novo_quadrado = criar_nodo(quadrado, NULL, 0);

    if (lista_vazia(lista, 0))
        lista->primeiro_quadrado = novo_quadrado;
    else
    {
        //Verificando a posição de inserção
        NODO *aux = lista->primeiro_quadrado;

        while (aux->proximo != NULL)
            aux = aux->proximo;

        aux->proximo = novo_quadrado;
    }
}

void remove_quadrado (QUADRADO *quadrado, LISTA *lista)
{
    //Qual será a regra para remoção?
}

bool secao_vazia (LISTA *lista)
{
    NODO *aux = lista->primeiro_quadrado;

    while (aux!=NULL)
    {
        if (aux->quadrado->visibilidade==true)
            return false;

        aux = aux->proximo;
    }

    return true;
}

void redefinir_origem_bolas (LISTA *lista)
{
    NODO *aux = lista->primeira_bola;

    while (aux != NULL)
    {
        aux->bola->px_origem = aux->bola->px;
        aux->bola->py_origem = 535;

        aux = aux->proximo;    
    }
}

//formato 0 é quadrado. formato 1 é bola.
void destruir_lista (LISTA *lista, int formato)
{
    NODO *aux=NULL;

    if (formato==0)
    {
        NODO *iterador_quadrado = lista->primeiro_quadrado;
        
        while (iterador_quadrado != NULL)
        {
            aux = iterador_quadrado;
            iterador_quadrado = iterador_quadrado->proximo;
            free(aux->quadrado);
            free(aux);
        }
    }
    
    else if (formato==1)
    {
        NODO *iterador_bola = lista->primeira_bola;
        
        while (iterador_bola != NULL)
        {
            aux = iterador_bola;
            iterador_bola = iterador_bola->proximo;
            free(aux->bola);
            free(aux);
        }
    }
    else
    {
        perror("Destruicao de lista indefinida");
        exit(1);
    }

    aux = NULL;
    free(lista);
}