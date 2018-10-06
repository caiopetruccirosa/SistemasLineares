#ifndef LISTALIGADA_H_INCLUDED
#define LISTALIGADA_H_INCLUDED

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

char inserir(Lista* lis, void* inf);
void* remover(Lista* lis);
void* getFromIndex(Lista* lis, int index);
int length(Lista* lis);

#endif // LISTALIGADA_H_INCLUDED
