#include <stdlib.h>

// struct que define como deve ser o Nó
typedef
    struct No
    {
        void* info;
        struct No* prox;
    }
    No;

// struct que define como deve ser a Lista
typedef
    struct Lista
    {
        No* inicio;
        int (*compareTo) (void*, void*);
        //char* (*toString) (void*);
    }
    Lista;


// Método que insere, já ordenado, um elemento na lista e retorna 
// boolean indicando se o elemento foi inserido ou não.
// Obs: não permite elementos repetidos.
char inserir(Lista* lis, void* inf)
{
    if (inf == NULL)
        return 0; // false

    if (lis->inicio == NULL)
    {
        lis->inicio = (No*) malloc(sizeof(No));
        lis->inicio->info = inf;
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

    free(atual);

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
    free(atual);

    return info;
}

// Método que devolve a informação de um item da lista
// a partir do índice passado.
// Obs: o método retorna NULL, caso o índice passado seja inválido,
// a lista esteja vazia ou o item não for encontrado.
void* getFromIndex(Lista* lis, int index)
{
    if (lis == NULL || lis->inicio == NULL)
        return NULL;

    if (index < 0)
        return NULL;

    void* info = NULL;

    int i = 0;
    No* atual = lis->inicio;
    while (atual->prox != NULL)
    {
        if (i == index)
        {
            info = atual->info;
            break;
        }

        atual = atual->prox;
    }

    return info;
}