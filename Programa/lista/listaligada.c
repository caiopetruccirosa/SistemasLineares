#include <stdlib.h>
#include "listaligada.h"

// Método que insere, já ordenado, um elemento na lista e retorna
// boolean indicando se o elemento foi inserido ou não.
// Obs: não permite elementos repetidos.
char inserir(Lista* lis, void* inf)
{
    if (lis == NULL || inf == NULL)
        return 0; // false

    if (lis->inicio == NULL)
    {
        lis->inicio = (No*) malloc(sizeof(No));
        lis->inicio->info = inf;
        lis->inicio->prox = NULL;
        return 1; // true
    }

    int comp = (*lis->compareTo)(inf, lis->inicio->info);

    if (comp == 0)
            return 0; // false

    if (comp < 0)
    {
        No* novo = (No*) malloc(sizeof(No));
        novo->info = inf;
        novo->prox = lis->inicio;
        lis->inicio = novo;
        return 1; // true
    }


    No* atual = lis->inicio;
    while (atual->prox != NULL)
    {
        comp = (*lis->compareTo)(inf, atual->prox->info);

        if (comp == 0)
            return 0; // false

        if (comp < 0)
            break;

        atual = atual->prox;
    }

    No* novo = (No*) malloc(sizeof(No));
    novo->info = inf;
    novo->prox = atual->prox;
    atual->prox = novo;

    return 1; // true
}


// Método que remove o último elemento da lista e
// retorna o elemento que foi removido.
// Obs: caso não haja elementos para remover, retorna NULL.
void* remover(Lista* lis)
{
    if (lis == NULL || lis->inicio == NULL)
        return NULL;

    if (lis->inicio->prox == NULL)
    {
        void* info = lis->inicio->info;

        free(lis->inicio->info);
        free(lis->inicio->prox);

        lis->inicio = NULL;

        return info;
    }

    No* atual = lis->inicio;
    while (atual->prox->prox != NULL)
        atual = atual->prox;

    void* info = atual->prox->info;
    free(atual->prox->info);
    free(atual->prox->prox);

    atual->prox = NULL;

    return info;
}

// Método que devolve a informação de um item da lista
// a partir do índice passado.
// Obs: o método retorna NULL, caso o índice passado seja inválido,
// a lista esteja vazia ou o item não for encontrado.
void* getFromIndex(Lista* lis, int index)
{
    if (lis == NULL || index < 0)
        return NULL;

    void* info = NULL;

    int i = 0;
    No* atual = lis->inicio;
    while (atual != NULL)
    {
        if (i == index)
        {
            info = atual->info;
            break;
        }

        i++;
        atual = atual->prox;
    }

    return info;
}

int length(Lista* lis)
{
    if (lis == NULL)
        return 0;

    No* atual = lis->inicio;
    int ret = 0;
    while (atual != NULL)
    {
        ret++;
        atual = atual->prox;
    }

    return ret;
}
