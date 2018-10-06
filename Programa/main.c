#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lista/listaligada.h"

typedef
    struct Celula // estrutura das celulas da matriz de coeficientes
    {
        char* nome;
        double valor;
    }
    Celula;

char* lerArquivo(FILE *arquivo) // método que lê um arquivo passado como parâmetro e devolve o texto do arquivo em uma string
{
    if (arquivo == NULL) // arquivo nao existe
        return NULL;

    char* texto;
    size_t n = 0;
    int c;

    fseek(arquivo, 0, SEEK_END);
    long int f_size = ftell(arquivo);  // pega o tamanho do arquivo
    fseek(arquivo, 0, SEEK_SET);
    texto = malloc(f_size);

    while ((c = fgetc(arquivo)) != EOF)
    {
        texto[n++] = (char)c;
    }

    texto[n] = '\0';

    return texto;
}

Lista* listarIncognitas(char** equacoes, int qtdEquacoes)
{
    Lista* ret = (Lista*)malloc(sizeof(Lista));
    ret->inicio = NULL;
    ret->compareTo = strcmp;

    char* incognita = NULL;
    int i;
    for (i = 0; i < qtdEquacoes; i++)
    {
        int incognita_index = 0;
        int j;
        for (j = 0; j < strlen(*(equacoes+i)); j++)
        {
            int char_ascii = (int)*(*(equacoes+i)+j);
            if ((char_ascii >= 65 && char_ascii <= 90) || (char_ascii >= 97 && char_ascii <= 122))
            {
                if (incognita == NULL)
                {
                    int tam = 0;
                    int k;
                    for (k = j; k < strlen(*(equacoes+i)); k++)
                    {
                        int char_ascii_aux = (int)*(*(equacoes+i)+k);
                        if ((char_ascii_aux >= 65 && char_ascii_aux <= 90) || (char_ascii_aux >= 97 && char_ascii_aux <= 122))
                            tam++;
                        else
                            break;
                    }

                    incognita = (char*)malloc((tam+1)*sizeof(char));
                    incognita_index = 0;
                }

                *(incognita+incognita_index) = (char)char_ascii;
                incognita_index++;
            }
            else if (incognita != NULL)
            {
                *(incognita+incognita_index) = '\0';
                inserir(ret, incognita);
                incognita = NULL;
            }
        }
    }

    return ret;
}

Celula** criarMatriz(Lista* incognitas, int qtdEquacoes)
{
    Celula** coeficientes = (Celula**) malloc(qtdEquacoes * sizeof(Celula*));
    int i;
    for (i = 0; i < qtdEquacoes; i++)
    {
        *(coeficientes+i) = (Celula*) malloc((qtdEquacoes+1) * sizeof(Celula));

        int j;
        for (j = 0; j < qtdEquacoes+1; j++) {
            char* nome;
            if (j < qtdEquacoes)
                nome = getFromIndex(incognitas, j);
            else
            {
                nome = (char*) malloc(sizeof(char));
                *nome = '\0';
            }

            (*(coeficientes+i)+j)->nome = nome;
            (*(coeficientes+i)+j)->valor = 0.0;
        }
    }

    return coeficientes;
}

void extrairCoeficientes(Celula** coeficientes, Lista* incognitas, char** equacoes, int qtdEquacoes)
{
    char* incognitaAtual = NULL;
    char* coeficienteAtual = NULL;
    char* resultadoAtual = NULL;

    int i;
    for (i = 0; i < qtdEquacoes; i++)
    {
        int incognitaAtual_index = 0;
        int coeficienteAtual_index = 0;
        int resultadoAtual_index = 0;

        int ladoDaEquacao = -1;

        int j;
        for (j = 0; j < strlen(*(equacoes+i)); j++)
        {
            /*
            if (ladoDaEquacao < 0)
            {

            }
            else if (ladoDaEquacao > 0)
            {
                *(resultadoAtual+resultadoAtual_index) = (char)char_ascii;
                resultadoAtual_index++;
            }
            */

            int char_ascii = (int)*(*(equacoes+i)+j);

            if ((char_ascii >= 65 && char_ascii <= 90) || (char_ascii >= 97 && char_ascii <= 122))
            {
                if (coeficienteAtual == NULL || strcmp(coeficienteAtual, "-") == 0)
                {
                    coeficienteAtual = (char*)malloc(2*sizeof(char));
                    *coeficienteAtual = "1";
                    coeficienteAtual_index = 1;
                }

                if (incognitaAtual == NULL)
                {
                    int tam = 0;
                    int k;
                    for (k = j; k < strlen(*(equacoes+i)); k++)
                    {
                        int char_ascii_aux = (int)*(*(equacoes+i)+k);
                        if ((char_ascii_aux >= 65 && char_ascii_aux <= 90) || (char_ascii_aux >= 97 && char_ascii_aux <= 122))
                            tam++;
                        else if (char_ascii_aux != 32)
                            break;
                    }

                    incognitaAtual = (char*)malloc((tam+1)*sizeof(char));
                    incognitaAtual_index = 0;
                }

                *(incognitaAtual+incognitaAtual_index) = (char)char_ascii;
                incognitaAtual_index++;
            }
            else if (char_ascii >= 48 && char_ascii <= 57) // seja quaisquer números
            {
                if (coeficienteAtual == NULL)
                {
                    int tam = 0;
                    int k;
                    for (k = j; k < strlen(*(equacoes+i)); k++)
                    {
                        int char_ascii_aux = (int)*(*(equacoes+i)+k);
                        if (char_ascii_aux >= 48 && char_ascii_aux <= 57)
                            tam++;
                        else if (char_ascii_aux != 32)
                            break;
                    }

                    coeficienteAtual = (char*)malloc((tam+1)*sizeof(char));
                    coeficienteAtual_index = 0;
                }

                *(coeficienteAtual+coeficienteAtual_index) = (char)char_ascii;
                coeficienteAtual_index++;
            }
            else if (char_ascii == 45) // igual a "-"
            {
                if (coeficienteAtual != NULL && incognitaAtual != NULL)
                {
                    *(incognitaAtual+incognitaAtual_index) = '\0';
                    *(coeficienteAtual+coeficienteAtual_index) = '\0';

                    int k;
                    for (k = 0; k < qtdEquacoes; k++)
                    {
                        char* colunaAtual = (*(coeficientes+i)+k)->nome;

                        if (strcmp(incognitaAtual, colunaAtual) == 0)
                            (*(coeficientes+i)+k)->valor += atof(coeficienteAtual);
                    }

                    incognitaAtual = NULL;
                    coeficienteAtual = NULL;
                }

                int tam = 0;
                int k;
                for (k = j+1; k < strlen(*(equacoes+i)); k++)
                {
                    int char_ascii_aux = (int)*(*(equacoes+i)+k);
                    if (char_ascii_aux >= 48 && char_ascii_aux <= 57)
                        tam++;
                    else if (char_ascii_aux != 32)
                        break;
                }

                coeficienteAtual = (char*)malloc((tam+2)*sizeof(char));
                coeficienteAtual_index = 0;

                *(coeficienteAtual) = "-";
                coeficienteAtual_index++;
            }
            else if (char_ascii == 61) // igual a "="
            {
                if (coeficienteAtual != NULL && incognitaAtual != NULL)
                {
                    *(incognitaAtual+incognitaAtual_index) = '\0';
                    *(coeficienteAtual+coeficienteAtual_index) = '\0';

                    int k;
                    for (k = 0; k < qtdEquacoes; k++)
                    {
                        char* colunaAtual = (*(coeficientes+i)+k)->nome;

                        if (strcmp(incognitaAtual, colunaAtual) == 0)
                            (*(coeficientes+i)+k)->valor += atof(coeficienteAtual);
                    }

                    incognitaAtual = NULL;
                    coeficienteAtual = NULL;
                }
            }
        }

        if (resultadoAtual != NULL)
        {
            *(resultadoAtual+resultadoAtual_index) = '\0';
            (*(coeficientes+i)+(qtdEquacoes-1))->valor += atof(resultadoAtual);

            resultadoAtual = NULL;
        }
    }

    free(incognitaAtual);
    free(coeficienteAtual);
    free(resultadoAtual);
}

int main()
{
    char path[250];
    FILE *arq;
    char* texto;

    /////////////////////////////////////

    INICIO:printf("Digite o arquivo que deseja ler: ");
    scanf("%s", path);

    arq = fopen(path, "r");
	if (arq == NULL)
    {
		printf("Arquivo inexistente! \n\n");
		goto INICIO;
	}

    texto = lerArquivo(arq);
    free(arq);

    /////////////////////////////////////

    int qtdEquacoes = atoi(strtok(texto, "\n"));
    char** equacoes = (char**)malloc(qtdEquacoes * sizeof(char*));

    int i = 0;
    char* equacao = strtok(NULL, "\n");
    while (equacao != NULL)
    {
        *(equacoes+i) = (char*)malloc(strlen(equacao) * sizeof(char));
        strcpy(*(equacoes+i), equacao);
        equacao = strtok(NULL, "\n");
        i++;
    }

    free(texto);

    /////////////////////////////////////

    Lista* incognitas = listarIncognitas(equacoes, qtdEquacoes);

    if (length(incognitas) > qtdEquacoes) // indica caso o sistema não seja possível determinado
    {
		printf("Sistema nao eh possivel determinado, ou seja, possui mais de um conjunto solucao! \n\n");
		goto INICIO;
	}


    Celula** coeficientes = criarMatriz(incognitas, qtdEquacoes);
    extrairCoeficientes(coeficientes, incognitas, equacoes, qtdEquacoes);

    // a partir de agora, toda a informação necessária está na matriz de coeficientes

    for (i = 0; i < qtdEquacoes; i++) {
        int j;
        for (j = 0; j < qtdEquacoes + 1; j++) {
            char* nome = (*(coeficientes+i)+j)->nome;
            double valor = (*(coeficientes+i)+j)->valor;
            printf("%s:%.2lf ", nome, valor);
        }

        printf("\n");
    }
}
