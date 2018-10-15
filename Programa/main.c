#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lista/listaligada.h"

typedef
    struct Celula // estrutura das celulas da matriz de coeficientes
    {
        char* nome;  // indica o nome da inc�gnita
        double valor;  // indica o valor que ser� armazenado na c�lula
    }
    Celula;

// M�todo que retorna quantas linhas existem no arquivo
int quantasLinhas(FILE *arquivo)
{
    if (arquivo == NULL)  // arquivo n�o existe
        return 0;

    fseek(arquivo, 0, SEEK_SET); // posiciona o ponteiro do arquivo no in�cio

    char c;
    int linhas = 1;  // presume que o arquivo sempre ter� no m�nimo uma linha
    while ((c = fgetc(arquivo)) != EOF)
    {
        if (c == '\n') // soma uma linha a cada '\n', que indica um [ENTER]
            linhas++;
    }

    fseek(arquivo, 0, SEEK_SET); // posiciona o ponteiro do arquivo no in�cio

    return linhas;
}

// M�todo que l� um arquivo passado como par�metro e devolve o texto do arquivo em uma string
char* lerArquivo(FILE *arquivo)
{
    if (arquivo == NULL) // arquivo n�o existe
        return NULL;

    char* texto;
    size_t i = 0;
    int c;

    fseek(arquivo, 0, SEEK_END);
    long int f_size = ftell(arquivo);  // obtem o tamanho do arquivo
    fseek(arquivo, 0, SEEK_SET);
    texto = (char*)malloc(f_size*sizeof(char)); // aloca um ponteiro para armazenar todo o texto do arquivo

    while ((c = fgetc(arquivo)) != EOF)
    {
        *(texto+i++) = (char)c;
    }

    *(texto+i) = '\0';  // indica o final do texto

    return texto;
}

// M�todo que lista as inc�gnitas em ordem alfab�tica
Lista* listarIncognitas(char** equacoes, int qtdEquacoes)
{
    Lista* ret = (Lista*)malloc(sizeof(Lista));  // aloca a mem�ria da lista
    ret->inicio = NULL;
    ret->compareTo = strcmp;  // a fun��o de compara��o dos itens da lista, comparar� por ordem alfab�tica

    char* incognita = NULL;  // vetor no qual ser�o geradas as inc�gnitas
    int i;
    for (i = 0; i < qtdEquacoes; i++)  // percorre todas as equa��es
    {
        int incognita_index = 0;
        int j;
        for (j = 0; j < strlen(*(equacoes+i)); j++)
        {
            int char_ascii = (int)*(*(equacoes+i)+j);
            if ((char_ascii >= 65 && char_ascii <= 90) || (char_ascii >= 97 && char_ascii <= 122)) // caso seja igual a quaisquer letras
            {
                if (incognita == NULL)
                {
                    int tam = 0;  // indica quanta memm�ria dever� ser alocada
                    int k;
                    for (k = j; k < strlen(*(equacoes+i)); k++)
                    {
                        int char_ascii_aux = (int)*(*(equacoes+i)+k);
                        if ((char_ascii_aux >= 65 && char_ascii_aux <= 90) || (char_ascii_aux >= 97 && char_ascii_aux <= 122))
                            tam++;
                        else
                            break;
                    }

                    incognita = (char*)malloc((tam+1)*sizeof(char));  // aloca m�moria necess�ria para armazenar a inc�gnita
                    incognita_index = 0;
                }

                *(incognita+incognita_index) = (char)char_ascii;
                incognita_index++;
            }
            else if (incognita != NULL)  // caso a inc�gnita j� tenha sido armazenada
            {
                *(incognita+incognita_index) = '\0';
                inserir(ret, incognita);  // insere a inc�gnita na lista
                incognita = NULL;
            }
        }
    }

    return ret;
}

// M�todo que cria uma matriz com as inc�gnitas organizadas por colunas e os valores padr�es como 0
Celula** criarMatriz(Lista* incognitas, int qtdEquacoes)
{
    Celula** coeficientes = (Celula**) malloc(qtdEquacoes * sizeof(Celula*));  // aloca mem�ria para a matriz
    int i;
    for (i = 0; i < qtdEquacoes; i++)  // percorre todas as equa��es
    {
        *(coeficientes+i) = (Celula*) malloc((qtdEquacoes+1) * sizeof(Celula));  // aloca mem�ria para a matriz

        int j;
        for (j = 0; j < qtdEquacoes+1; j++) {  // percorre todas as c�lulas
            char* nome;
            if (j < qtdEquacoes)
                nome = getFromIndex(incognitas, j);  // d� um nome de inc�gnita para a matriz
            else
            {
                nome = (char*) malloc(sizeof(char));  // d� um nome vazio caso seja a coluna de resultados
                *nome = '\0';
            }

            (*(coeficientes+i)+j)->nome = nome;  // insere os valores
            (*(coeficientes+i)+j)->valor = 0.0;  // na matriz
        }
    }

    return coeficientes;
}

// M�todo que extrai todos os coeficientes das equa��es e os atribui �s suas inc�gnitas
void extrairCoeficientes(Celula** coeficientes, Lista* incognitas, char** equacoes, int qtdEquacoes)
{
    char* incognitaAtual;  // vetor no qual ser�o geradas as inc�gnitas
    char* coeficienteAtual;  // vetor no qual ser�o gerados os coeficientes
    char* resultado;  // vetor no qual ser�o gerados os resultados

    int i;
    for (i = 0; i < qtdEquacoes; i++)  // percorre as equa��es
    {
        incognitaAtual = NULL;
        coeficienteAtual = NULL;
        resultado = NULL;

        int incognitaAtual_index = 0;     //
        int coeficienteAtual_index = 0;   // �ndices que controlam a indexa��o das principais vari�veis
        int resultado_index = 0;          //

        int ladoDaEquacao = -1;
        int j;
        for (j = 0; j < strlen(*(equacoes+i)); j++)
        {
            int char_ascii = (int)*(*(equacoes+i)+j);

            if (ladoDaEquacao < 0) // antes do "="
            {
                if (char_ascii == 61) // igual a "="
                    ladoDaEquacao = 1;

                if (char_ascii == 43 || char_ascii == 45 || char_ascii == 61) // igual a "+" ou a "-" ou a "="
                {
                    if (coeficienteAtual != NULL && incognitaAtual != NULL)  // caso um coeficiente e uma inc�gnita j� tenham sido
                    {                                                        // lidos, os insere na matriz de coeficientes
                        *(coeficienteAtual+coeficienteAtual_index) = '\0';
                        *(incognitaAtual+incognitaAtual_index) = '\0';

                        int k;
                        for (k = 0; k < qtdEquacoes; k++)  // insere na matriz de coeficientes
                        {
                            char* colunaAtual = (*(coeficientes+i)+k)->nome;

                            if (strcmp(incognitaAtual, colunaAtual) == 0)
                                (*(coeficientes+i)+k)->valor += atof(coeficienteAtual);
                        }


                        free(coeficienteAtual);  // desaloca o coeficiente armazenado at� agora
                        free(incognitaAtual);  // desaloca a inc�gnita armazenada at� agora

                        coeficienteAtual = NULL;
                        incognitaAtual = NULL;
                    }
                }

                if ((char_ascii >= 48 && char_ascii <= 57) || char_ascii == 45 || char_ascii == 46) // seja quaisquer n�meros ou "-" ou "."
                {
                    if (coeficienteAtual == NULL)
                    {
                        int tam = 1;  // indica quanta mem�ria dever� ser alocada
                        int k;
                        for (k = j+1; k < strlen(*(equacoes+i)); k++)
                        {
                            int char_ascii_aux = (int)*(*(equacoes+i)+k);
                            if ((char_ascii_aux >= 48 && char_ascii_aux <= 57) || char_ascii_aux == 46)
                                tam++;
                            else if (char_ascii_aux != 32) {
                                if (tam == 1 && char_ascii == 45)
                                    tam++;
                                break;
                            }
                        }

                        coeficienteAtual = (char*)malloc((tam+1)*sizeof(char));  // aloca a mem�ria para o coeficiente
                        coeficienteAtual_index = 0;
                    }

                    *(coeficienteAtual+coeficienteAtual_index) = (char)char_ascii;  // preenche o coeficiente
                    coeficienteAtual_index++;
                }
                else if ((char_ascii >= 65 && char_ascii <= 90) || (char_ascii >= 97 && char_ascii <= 122)) // qualquer letra
                {
                    if (coeficienteAtual == NULL)  // caso o coeficiente seja nulo, ir� gerar um coeficiente neutro ("1")
                    {
                        coeficienteAtual = (char*)malloc((2)*sizeof(char));
                        *coeficienteAtual = '1';  // preenche o coeficiente
                        coeficienteAtual_index = 1;
                    }

                    if (coeficienteAtual_index == 1 && *coeficienteAtual == '-')
                    {
                        *(coeficienteAtual+coeficienteAtual_index) = '1';  // preenche o coeficiente
                        coeficienteAtual_index++;
                    }

                    if (incognitaAtual == NULL)
                    {
                        int tam = 0;  // indica quanta mem�ria dever� ser alocada
                        int k;
                        for (k = j; k < strlen(*(equacoes+i)); k++)
                        {
                            int char_ascii_aux = (int)*(*(equacoes+i)+k);
                            if ((char_ascii_aux >= 65 && char_ascii_aux <= 90) || (char_ascii_aux >= 97 && char_ascii_aux <= 122))
                                tam++;
                            else if (char_ascii_aux != 32)
                                break;
                        }

                        incognitaAtual = (char*)malloc((tam+1)*sizeof(char));  // aloca a mem�ria para a inc�gnita
                        incognitaAtual_index = 0;
                    }

                    *(incognitaAtual+incognitaAtual_index) = (char)char_ascii;  // preenche a inc�gnita
                    incognitaAtual_index++;
                }
            }
            else if (ladoDaEquacao > 0) // depois do "="
            {
                if (char_ascii >= 48 && char_ascii <= 57) // quaisquer n�meros
                {
                    if (resultado == NULL)
                    {
                        int tam = 0;  // indica quanta mem�ria dever� ser alocada
                        int k;
                        for (k = j; k < strlen(*(equacoes+i)); k++)
                        {
                            int char_ascii_aux = (int)*(*(equacoes+i)+k);
                            if (char_ascii_aux >= 48 && char_ascii_aux <= 57)
                                tam++;
                            else if (char_ascii_aux != 32)
                                break;
                        }

                        resultado = (char*)malloc((tam+1)*sizeof(char));  // aloca a mem�ria para o resultaddo
                        resultado_index = 0;
                    }

                    *(resultado+resultado_index) = (char)char_ascii;  // preenche o resultado
                    resultado_index++;
                }
            }
        }

        if (resultado != NULL)
        {
            *(resultado+resultado_index) = '\0';
            (*(coeficientes+i)+qtdEquacoes)->valor = atof(resultado);
        }

        free(incognitaAtual);
        free(coeficienteAtual);
        free(resultado);
    }
}

// M�todo que desaloca as vari�veis
void desalocarVariaveis(FILE* arquivo, char* texto, Lista* lis, Celula** mat, char** equacoes, int qtdEquacoes)
{
    // desaloca o arquivo, pois ele n�o ser� mais usado
    free(arquivo);

    // desaloca o texto pois ele n�o ser� mais usado
    free(texto);

    // desaloca a lista
    free(lis);

    // desacola a matriz de c�lulas
    int i;
    for (i = qtdEquacoes-1; i >= 0; i--)
    {
        int j;
        for (j = qtdEquacoes-1; j >= 0; j--) {
            if (j < qtdEquacoes-1)
                free((*(mat+i)+j)->nome);
            free((*(mat+i)+j));
        }
    }

    // desaloca a matriz de equa��es, pois n�o ser� mais usada
    for (i = qtdEquacoes-1; i >= 0; i--)
        free(*(equacoes+i));
}


//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////

// funcao para operacao elementar de trocar duas linhas
void mudar_linhas(Celula** mat, int size, int i, int j)
{
    //printf("Linhas mudadas %d e %d\n", i, j);
    int k;
    for (k=0; k <= size; k++)
    {
        Celula temp = *(*(mat+i)+k);
        *(*(mat+i)+k) = *(*(mat+j)+k);
        *(*(mat+j)+k) = temp;
    }
}

// funcao para printar a matriz em qualquer estagio
void print(Celula** mat, int size)
{
    int i;
    for (i=0; i < size; i++, printf("\n"))
    {
        int j;
        for (j=0; j <= size; j++) {
            printf("%s : %.2lf  ", (*(mat+i)+j)->nome, (*(mat+i)+j)->valor);
        }
    }
    printf("\n");
}

// funcao para escalonar a matriz desejada
int escalonamento(Celula** mat, int size)
{
    int k;
    for (k=0; k < size; k++)
    {
        // Inicializa m�ximos valor e index para pivotamento
        int i_max = k;
        double v_max = (*(mat + i_max)+k)->valor;

        /* encontra maior amplitude para pivot */
        int i;
        for (i = k+1; i < size; i++)
            if (abs((*(mat+i)+k)->valor > v_max))
                v_max = (*(mat+i)+k)->valor, i_max = i;

        /* se houver um elemento na diagonal principal equivalente a zero,
         a matriz ser� singular, pois levar� a uma divis�o por zero
         por isso, inverte-se as colunas*/

        /* troca a maior linha pela atual*/
        if (i_max != k)
            mudar_linhas(mat, size, k, i_max);

        for (i=k+1; i < size; i++)
        {
            /* fator f para setar linha atual k� elemento para 0,
               deixando a k� coluna para 0 */
            double f = (*(mat+i)+k)->valor / (*(mat+k)+k)->valor;


            /* subtrai f� multiplo do correspondente elemento da linha k*/
            int j;
            for (j=k+1; j <= size; j++)
                (*(mat+i)+j)->valor -= (*(mat+k)+j)->valor *f;

            /* colocando zeros na matriz triangular inferior */
            (*(mat+i)+k)->valor = 0.0;
        }
    }

    return -1;
}

// funcao para calcular o valor das incognitas
Celula* backSub(Celula** mat, int size)
{
    Celula* x;  // vetor para guardar os resultados
    x = (Celula*)malloc(size*sizeof(Celula));

    /* comeca calculando a partir da ultima equacao*/
    int i;
    for (i = size-1; i >= 0; i--)
    {
        /* come�a com o lado direito da equa��o */
        (x+i)->nome = (*(mat+i)+i)->nome;
        (x+i)->valor = (*(mat+i)+size)->valor;

        /* inicializa j como i+1, pois a matriz �
           triangular superior*/
        int j;
        for (j= i+1; j < size; j++)
        {
            /* subtrai todos os valores da esquerda
             * exceto o coeficiente da variavel
             * cujo valor esta sendo calculado */
            (x+i)->valor -= (*(mat+i)+j)->valor * (x+j)->valor;
        }

        /* divide o lado direito pelo coeficiente da
           incognita sendo calculada*/
        (x+i)->valor = (x+i)->valor / (*(mat+i)+i)->valor;
    }

    return x;  // retorna os resultados
}

// funcao principal de resolu��o do sistema
Celula* eliminacaoGaussiana(Celula** mat, int size)
{
    /* reduzir para forma escalonada */
    int sinalizadordeSingular = escalonamento(mat, size);

    /* se matrix for singular */
    if (sinalizadordeSingular != -1)
    {
        printf("\nMatriz Singular!\n");

        /* se a forma escalonada da equacao correspondente
           a linha 0 for 0, o sistema possuir�
           infinitas solu��es: SI*/
        if ((*(mat+sinalizadordeSingular)+size))
            printf("\nO sistema eh Sistema Indeterminado.");
        else
            printf("\nExistem infinitas solucoes.");

        return NULL;
    }

    /* solucionar sistema e printar usando substitui��o
       da �ltima linha para a primeira */
    Celula* ret = backSub(mat, size);
    return ret;
}

// M�todo principal do programa
int main()
{
    char path[250];
    FILE *arq;
    char* texto;
    int qtdEquacoes;

    /////////////////////////////////////

    printf("=================================================================\n");
    printf("======================== SISTEMAS LINEARES ======================\n");
    printf("=================================================================\n");
    printf("\n");

    INICIO:printf("Digite o arquivo de sistemas que deseja ler: ");
    scanf("%s", path);

    arq = fopen(path, "r");
	if (arq == NULL)
    {
		printf("Arquivo inexistente! \n\n");
		goto INICIO;
	}

    qtdEquacoes = quantasLinhas(arq);  // indica quantas equa��es existem no arquivo
    texto = lerArquivo(arq);  // armazena todo o texto (e equa��es) do arquivo

    fclose(arq);  // fecha o arquivo, encerrando a leitura

    /////////////////////////////////////

    char** equacoes = (char**)malloc(qtdEquacoes * sizeof(char*));  // aloca a matriz de equa��es
    char* equacao = strtok(texto, "\n");  // separa o texto em equa��es

    int i = 0;
    while (equacao != NULL)
    {
        *(equacoes+i) = (char*)malloc(strlen(equacao) * sizeof(char));  // aloca mem�ria para a equa��o
        strcpy(*(equacoes+i), equacao);  // armazena na matriz de equa��es
        equacao = strtok(NULL, "\n");
        i++;
    }

    /////////////////////////////////////

    Lista* incognitas = listarIncognitas(equacoes, qtdEquacoes);  // lista todas as inc�gnitas do sistema

    if (length(incognitas) > qtdEquacoes) // indica caso o sistema n�o seja poss�vel determinado
    {
		printf("\nSistema nao eh possivel determinado, ou seja, possui mais de um conjunto solucao! \n\n");
		goto INICIO;
	}

    Celula** coeficientes = criarMatriz(incognitas, qtdEquacoes);  // cria uma matriz com inc�gnitas organizadas por colunas e valores padr�o
    extrairCoeficientes(coeficientes, incognitas, equacoes, qtdEquacoes);  // extrai os coeficientes, passando a matriz de coeficientes por refer�ncia

    print(coeficientes, qtdEquacoes);

    Celula* resultado = eliminacaoGaussiana(coeficientes, qtdEquacoes); // resolve o sistema

    print(coeficientes, qtdEquacoes);

    /////////////////////////////////////

    if (resultado != NULL)
    {
        // resolve o sistema
        printf("\nSistema:\n");
        for (i = 0; i < qtdEquacoes; i++)
            printf(" %s\n", *(equacoes+i));

        printf("\nSolucao do sistema:\n");
        for (i = 0; i < qtdEquacoes; i++)
            printf(" %s = %.2lf\n", (resultado+i)->nome, (resultado+i)->valor);
    }

    printf("\n================================\n");
    printf("\n Por Caio Petrucci e Joao Henri \n");
    printf("\n             17167 | 17185      \n");

    /////////////////////////////////////

    //desalocarVariaveis(arq, texto, incognitas, coeficientes, equacoes, qtdEquacoes);  // desaloca as vari�veis que n�o ser�o mais usadas
}
