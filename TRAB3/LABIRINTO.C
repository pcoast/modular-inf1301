/***************************************************************************
*   $MCI Módulo de implementação: Módulo Labirinto
*
*   Arquivo gerado:              LABIRINTO.C
*   Letras identificadoras:      LAB
*
*   Projeto: Disciplina INF1301
*   Departamento: DI/PUC-Rio
*   Autores: avc - Antônio Chaves
*            jpp - João Pedro Paiva
*            pc	 - Pedro Costa
*
*   $HA Histórico de evolução:
*       Versão  Autor   Data        Observações
*       1.00    jpp     22/09/2019  Início do desenvolvimento
*       2.00    jpp     27/09/2019  LAB_criaLabirinto()
*       3.00    jpp     29/09/2019  LAB_imprimeLabirinto()
*       4.00    jpp     02/10/2019  Acertar retornos e comentários
*		5.00	pc		08/10/2019	Adiciona estrutura do labirinto e adequa a implementação das funções
*
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "MATRIZ.H"
#include "LABIRINTO.H"

/***********************************************************************
*
*   $TC Tipo de dados:
*       ARV Descritor da cabeça de uma matriz.
*
*
*   $ED Descrição do tipo:
*       A cabeça da matriz é o ponto de acesso para uma determinada
*       matriz.
*       Por intermédio da referência para o nó corrente e dos
*       ponteiros
*       adjascentes pode-se navegar a matriz sem necessitar de uma
*       pilha.
*
***********************************************************************/

/***** Tipos encapsulados no módulo *****/

typedef struct tgLabirinto
{
    MAT_tppMatriz matriz;
} tpLabirinto;

typedef struct tgElementoLabirinto
{
    char paredeCaminhoEntradaOuSaida;
    char numPassadas;

} tpElementoLabirinto;

/***** Variáveis encapsuladas no módulo *****/

static char colunas, linhas, colunaInicio, linhaInicio;

/***** Protótipos das funções encapsuladas no módulo *****/

static LAB_tpCondRet LAB_converteCondRet(MAT_tpCondRet CondicaoDeRetornoMatriz);
static LAB_tpCondRet LAB_percorreLabirinto(LAB_tppLabirinto CabecaDoLabirinto, char *resposta);


/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************
*
*	$FC Função:
*       LAB Converter condições de retorno do
*       módulo de matriz para condições de retorno
*       do módulo de labirinto.
*
*
*   $AE Assertivas de entrada esperadas:
*       A condição de retorno de matriz está
*       correta.
*
*
*	$AS Assertivas de saída esperadas:
*       Condição de retorno foi convertida
*       corretamente.
*
****************************************************/
LAB_tpCondRet LAB_converteCondRet(MAT_tpCondRet CondicaoDeRetornoMatriz)
{
    if (CondicaoDeRetornoMatriz == MAT_CondRetFaltouMemoria)
        return LAB_CondRetFaltouMemoria;
    return LAB_CondRetProblemaMatriz;
}

/***************************************************
*
*	$FC Função:
*       LAB Criar o Labirinto armazenando os
*       caracteres corretos (inferidos a partir do
*       arquivo de texto) numa matriz criada
*       pela função MAT_criaMatriz().
*       Definir os valores globais das
*       dimensões do labirinto.
*
*
*   $AE Assertivas de entrada esperadas:
*       Arquivo de texto possui no máximo 10 linhas
*       e 11 colunas (sendo a última coluna composta
*       por caracteres de linebreak).
*		Valem as assertivas estruturais do
*       labirinto.
*
*
*	$AS Assertivas de saída esperadas:
*       Labirinto foi criado com as dimensões
*       corretas.
*       Caracteres corretos foram adicionados à
*       matriz.
*       Dimensões corretas do labirinto foram
*       armazenadas nas variáveis globais
*       "linhas" e "colunas".
*       Posição de inicio do labirinto foi
*       armazenada nas variáveis "colunaInicio"
*       e "linhaInicio".
*       Nó corrente da matriz aponta para a posição
*       de inicio do labirinto.
*		Valem as assertivas estruturais do
*       labirinto.
*
****************************************************/
LAB_tpCondRet LAB_criaLabirinto(char *arquivo, LAB_tppLabirinto *LabirintoASerCriado)
{

    int i, colCount = 0, lineCount = 0;
    FILE *fp;
    char CaractereTexto, CaractereLabirinto;
    MAT_tpCondRet CondRetDeMatriz;
    LAB_tpCondRet CondRetConvertidoParaLabirinto;
    tpElementoLabirinto *elementoLabirinto;

    MAT_tppMatriz matriz = NULL;

    fp = fopen(arquivo, "r"); /* Abre arquivo de texto no modo leitura */

    if (!fp)
        return LAB_CondRetArquivoTextoNulo; /* fp == NULL retorna condição de arquivo de texto nulo */

    colunas = 0;
    linhas = 0; /* Inicia contadores de coluna e de linha */

    while ((CaractereTexto = fgetc(fp)) != EOF) /* Loop para saber as dimensões do labirinto contando caracteres */
    {

        if (CaractereTexto == '\n')
        {
            linhas++; /* Conta número de linhas pelo caractere de linebreak (\n) no final de cada linha do texto */
            if (colCount != colunas)
                return LAB_CondRetTamanhoInvalido;
            colCount = 0;
        }
        else
            colCount++;
        if (!linhas)
            colunas++; /* Conta número de colunas pelo número de carácteres da primeira linha do texto */
    }

    if (++linhas > 10 || colunas > 10)
        return LAB_CondRetTamanhoInvalido; /* Soma 1 ao número de linhas porque o contador não conta com a última */

    CondRetDeMatriz = MAT_cria(linhas, colunas, free, &matriz); /* Cria matriz para armazenar o labirinto */

    if (CondRetDeMatriz != MAT_CondRetOK)
        return LAB_converteCondRet(CondRetDeMatriz); /* Retorna condição de retorno de matriz se algo estiver incorreto */

    (*LabirintoASerCriado) = (LAB_tppLabirinto)malloc(sizeof(tpLabirinto));
    if (!(*LabirintoASerCriado))
        return LAB_CondRetFaltouMemoria;

    (*LabirintoASerCriado)->matriz = matriz;

    rewind(fp);

    colCount = 0;
    while ((CaractereTexto = fgetc(fp)) != EOF) /* Loop de preencher labirinto */
    {

        if (CaractereTexto == '\n') /* Loop coloca nó corrente no primeiro nó da linha de baixo quando chegar no
									final da linha atual (caractere de linebreak) */
        {
            if (++lineCount < linhas)
            {
                MAT_vaiParaBaixo((*LabirintoASerCriado)->matriz);

                for (i = 1; i < colunas; i++)
                    MAT_vaiParaEsquerda((*LabirintoASerCriado)->matriz);

                colCount = 0;
            }
        }
        else if (colCount < colunas)
        {

            if (CaractereTexto == '|' || CaractereTexto == '-')
                CaractereLabirinto = 'p'; /* Parede */

            else if (CaractereTexto == 'I')
            {
                CaractereLabirinto = 'i'; /* Início do labirinto */
                colunaInicio = colCount;
                linhaInicio = lineCount;
            }

            else if (CaractereTexto == 'F')
                CaractereLabirinto = 'f'; /* Fim do labirinto */

            else if (CaractereTexto == ' ')
                CaractereLabirinto = 'c'; /* Caminho */

            else
                return LAB_CondRetCaractereInvalido;

            elementoLabirinto = (tpElementoLabirinto *)malloc(sizeof(tpElementoLabirinto)); /* Malloc do Elemento do Labirinto */

            if (!elementoLabirinto)
                return LAB_CondRetFaltouMemoria; /* Se elementoLabirinto == NULL, retorna condição de insufuciência de memória */

            elementoLabirinto->numPassadas = 0;

            elementoLabirinto->paredeCaminhoEntradaOuSaida = CaractereLabirinto;

            MAT_inserir((*LabirintoASerCriado)->matriz, elementoLabirinto);

            if (++colCount < colunas)
                MAT_vaiParaDireita((*LabirintoASerCriado)->matriz);
        }
    }

    fclose(fp); /* Fecha arquivo de texto */

    MAT_vaiParaPos((*LabirintoASerCriado)->matriz,colunaInicio,linhaInicio); /* Coloca nó corrente na mesma posição que o primeiro nó */

    return LAB_CondRetOK;
}

/***************************************************
*
*	$FC Função:
*       LAB Determina se todos os nós do labirinto
*           foram percorridos.
*
*
*   $AE Assertivas de entrada esperadas:
*       Cabeça do labirinto != NULL.
*		Valem as assertivas estruturais do
*       labirinto.
*
*
*	$AS Assertivas de saída esperadas:
*       Determinou-se se o labirinto foi
*       completamente percorrido ou não.
*       Nó corrente da matriz aponta para a posição
*       de inicio do labirinto.
*		Valem as assertivas estruturais do
*       labirinto.
*
****************************************************/
LAB_tpCondRet LAB_percorreLabirinto(LAB_tppLabirinto CabecaDoLabirinto, char *resposta)
{
    int l, c, i;

    tpElementoLabirinto **ptElementoLabirinto = (tpElementoLabirinto **)malloc(sizeof(tpElementoLabirinto *)); /* Malloc do Elemento do Labirinto */

    if (!ptElementoLabirinto)
        return LAB_CondRetFaltouMemoria; /* Se ptElementoLabirinto == NULL, retorna condição de insufuciência de memória */

    MAT_vaiParaPos((*CabecaDoLabirinto)->matriz,0,0); /* Coloca nó corrente na mesma posição que o primeiro nó */

    for (l = 0; l < linhas; l++)
    {
        for (c = 0; c < colunas; c++)
        {
            MAT_obterElemento(CabecaDoLabirinto->matriz, ptElementoLabirinto);

            if (((**ptElementoLabirinto)->paredeCaminhoEntradaOuSaida == 'c' || (**ptElementoLabirinto)->paredeCaminhoEntradaOuSaida == 'f') && (**ptElementoLabirinto)->numPassadas == 0)
            {
                *resposta = 1;
                return LAB_CondRetOK;
            }

            if (c < (colunas - 1))
                MAT_vaiParaDireita(CabecaDoLabirinto->matriz);
        }

        if (l < (linhas - 1))
        {

            MAT_vaiParaBaixo(CabecaDoLabirinto->matriz);

            for (i = 1; i < colunas; i++)
                MAT_vaiParaEsquerda(CabecaDoLabirinto->matriz);
        }
    }

    *resposta = 0;

    return LAB_CondRetOK;
}

/***************************************************
*
*	$FC Função:
*       LAB Imprimir o labirinto.
*
*
*   $AE Assertivas de entrada esperadas:
*       Cabeça do labirinto != NULL.
*		Valem as assertivas estruturais do
*       labirinto.
*
*
*	$AS Assertivas de saída esperadas:
*       Labirinto foi impresso.
*		Valem as assertivas estruturais do
*       labirinto.
*
****************************************************/
LAB_tpCondRet LAB_imprimeLabirinto(LAB_tppLabirinto CabecaDoLabirinto)
{
    int l, c, i;

    tpElementoLabirinto **ptElementoLabirinto = (tpElementoLabirinto **)malloc(sizeof(tpElementoLabirinto *)); /* Malloc do Elemento do Labirinto */

    if (!ptElementoLabirinto)
        return LAB_CondRetFaltouMemoria; /* Se ptElementoLabirinto == NULL, retorna condição de insufuciência de memória */

    MAT_vaiParaPos((*CabecaDoLabirinto)->matriz,0,0); /* Coloca nó corrente na mesma posição que o primeiro nó */

    for (l = 0; l < linhas; l++)
    {
        for (c = 0; c < colunas; c++)
        {
            MAT_obterElemento(CabecaDoLabirinto->matriz, ptElementoLabirinto);

            printf("%c ", (**ptElementoLabirinto)->paredeCaminhoEntradaOuSaida);

            if (c < (colunas - 1))
                MAT_vaiParaDireita(CabecaDoLabirinto->matriz);
        }

        if (l < (linhas - 1))
        {
            printf("\n");

            MAT_vaiParaBaixo(CabecaDoLabirinto->matriz);

            for (i = 1; i < colunas; i++)
                MAT_vaiParaEsquerda(CabecaDoLabirinto->matriz);
        }
    }

    return LAB_CondRetOK;
}

/***************************************************
*
*	$FC Função:
*       LAB Destruir o labirinto.
*
*
*   $AE Assertivas de entrada esperadas:
*       Cabeça do labirinto != NULL.
*		Valem as assertivas estruturais do
*       labirinto.
*
*
*	$AS Assertivas de saída esperadas:
*       Labirinto foi destruído.
*		Valem as assertivas estruturais do
*       labirinto.
*
****************************************************/
void LAB_destroiLabirinto(LAB_tppLabirinto CabecaDoLabirinto)
{
    MAT_destroi(CabecaDoLabirinto->matriz);
    free(CabecaDoLabirinto);
}