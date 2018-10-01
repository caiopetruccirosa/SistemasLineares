#include <stdlib.h>

typedef
    struct
    {
        void* info;
        struct No* prox;
    }
    No;

typedef
    struct
    {
        No* inicio;
        int (*compareTo) (void*,void*);
        void (*print) (void*);
    }
    Lista;

char/*boolean*/ insira (Lista* lis, void* inf)
{
    if (lis->inicio==NULL)
    {
        lis->inicio       = (No*)malloc(sizeof(No));
        lis->inicio->info = inf;
        return 1/*true*/;
    }

    int comp=*(lis->compareTo)(inf,lis->inicio->info);

    if (comp<0)
    {
        No* novo     = (No*)malloc(sizeof(No));
        novo->info   = inf;
        novo->prox   = lis->inicio;
        lis ->inicio = novo;
        return 1/*true*/;
    }

    if (comp==0)
        return 0/*false*/;

    No* atual = lis->inicio;

    for(;;) // parar ao achar onde inserir
    {
        if (atual->prox==NULL)
            break;

        comp=*(lis->compareTo)(inf,atual->prox->info);

        if (comp==0)
            return 0/*false*/;

        if (comp<0)
            break;

        atual=atual->prox;
    }

    No* novo = (No*)malloc(sizeof(No));
    novo->info = inf;
    novo->prox = atual->prox;
    atual->prox = novo;
}