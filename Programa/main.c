#include<string.h>
#include<stdio.h>
#include<stdlib.h>

char* lerArquivo(FILE *arquivo) {
    char* texto;
    size_t n = 0;
    int c;

    if (arquivo == NULL)
        return NULL;                  // arquivo nao existe

    fseek(arquivo, 0, SEEK_END);
    long int f_size = ftell(arquivo); // pega o tamanho do arquivo
    fseek(arquivo, 0, SEEK_SET);
    texto = malloc(f_size);

    while ((c = fgetc(arquivo)) != EOF)
    {
        texto[n++] = (char)c;
    }

    texto[n] = '\0';

    return texto;
}

typedef
    struct
    {
        char* nome;
        double valor;
    }
    celula;

int main()
{
	FILE *arq;
	char path[250];

	INICIO:printf("Digite o arquivo que deseja ler: ");
	scanf("%s", path);

    arq = fopen(path, "r");
	if (arq == NULL)
    {
		printf("Arquivo inexistente! \n\n");
		goto INICIO;
	}

    char* texto = lerArquivo(arq);
    int linhas = atoi(strtok(texto, "\n"));

    char** equacoes = (char**)malloc(linhas * sizeof(char*));

    int i = 0;
    char* equacao = strtok(NULL, "\n");
    while (equacao != NULL)
    {
        *(equacoes+i) = (char*)malloc(strlen(equacao) * sizeof(char));
        strcpy(*(equacoes+i), equacao);
        equacao = strtok(NULL, "\n");
        i++;
    }

    free(equacao);
    free(texto);
    free(arq);

    celula** coeficientes = (celula**) malloc(linhas * sizeof(celula*));
    for (i = 0; i < linhas; i++) {
        *(coeficientes+i) = (celula*) malloc((linhas+1) * sizeof(celula));

        int j;
        for (j = 0; j < strlen(*(equacoes+i)); j++) {
            char c = *(*(equacoes+i)+j);
        }
    }
    /*
    printf("Solucao do sistema:\n");
    for (i = 0; i < linhas; i++) {
        char* incognita = (*(coeficientes+i)+i)->nome;
        double valor = (*(coeficientes+i)+(linhas+1))->valor;

        printf("%s : %.3lf\n", incognita, valor);
    }
    */
}
