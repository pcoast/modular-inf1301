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
*
***************************************************************************/

#include "LABIRINTO.H"
#include "MATRIZ.H"
#include <stdio.h>
#include <stdlib.h>

/***** Variáveis encapsuladas no módulo *****/

static char colunas,linhas;


/***** Protótipos das funções encapsuladas no módulo *****/

static LAB_tpCondRet LAB_converteCondRet(MAT_tpCondRet CondicaoDeRetornoMatriz);

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
    LAB_tpCondRet CondicaoDeRetornoConvertida;

    switch (CondicaoDeRetornoMatriz)
    {
    case MAT_CondRetNoNaoExiste:
        return LAB_CondRetNoNaoExiste;

    case MAT_CondRetFaltouMemoria:
        return LAB_CondRetFaltouMemoriaNaMatriz; 

    case MAT_CondRetNoOcupado:
        return LAB_CondRetNoOcupado; 

    case MAT_CondRetNoVazio:
        return LAB_CondRetNoVazio;

    default:
        break;
    }
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
*		Valem as assertivas estruturais do
*       labirinto.
*
****************************************************/
LAB_tpCondRet LAB_criaLabirinto(void (*destruirElemento)(void *elemento), MAT_tppMatriz LabirintoASerCriado)
{
    FILE *fp;
    char CaractereTexto, *CaractereLabirinto;
    MAT_tpCondRet CondRetDeMatriz;
    LAB_tpCondRet CondRetConvertidoParaLabirinto;

    fp = fopen("LABTXT.TXT", "r"); /* Abre arquivo de texto no modo leitura */

    if (!fp) return LAB_CondRetArquivoTextoNulo; /* fp == NULL retorna condição de arquivo de texto nulo */

    colunas = linhas = 0; /* Inicia contadores de coluna e de linha */

    while ((CaractereTexto = fgetc(fp)) != EOF) /* Loop para saber as dimensões do labirinto contando caractéres */
    {

        if (!linhas) colunas++; /* Conta número de colunas pelo número de carácteres da primeira linha do texto */


        if (CaractereTexto=='\n') linhas++; /* Conta número de linhas pelo caractere de linebreak (\n) no final de cada linha do texto */

    }


    CondRetDeMatriz = MAT_criaMatriz(linhas,colunas,destruirElemento,LabirintoASerCriado); /* Cria matriz para armazenar o labirinto */

    if (CondRetDeMatriz!=MAT_CondRetOK) return LAB_converteCondRet(CondRetDeMatriz); /* Retorna condição de retorno de matriz se algo estiver incorreto */

    rewind(fp);

    while ((CaractereTexto = fgetc(fp)) != EOF) /* Loop de preencher labirinto */
    {

        if (CaractereTexto=='\n') /* Loop coloca nó corrente no primeiro nó da linha de baixo quando chegar no 
        final da linha atual (caractere de linebreak) */
        {
            CondRetDeMatriz = MAT_vaiParaBaixo(LabirintoASerCriado);
            if (CondRetDeMatriz!=MAT_CondRetOK) return LAB_converteCondRet(CondRetDeMatriz); /* Retorna condição de retorno de matriz se algo estiver incorreto */


            for (int i = 0; i<colunas; i++)
            {
                CondRetDeMatriz = MAT_vaiParaEsquerda(LabirintoASerCriado);
                if (CondRetDeMatriz!=MAT_CondRetOK) return LAB_converteCondRet(CondRetDeMatriz); /* Retorna condição de retorno de matriz se algo estiver incorreto */
            }

        }

        else
        {
            CaractereLabirinto = (char*) malloc(sizeof(char)); /* Malloc do Elemento da matriz */

            if (!CaractereLabirinto) return LAB_CondRetFaltouMemoriaNoLabirinto; /* Se CaractereLabirinto == NULL, retorna condição de insufuciência de memória */

            else if (CaractereTexto=='|' || CaractereTexto=='-') *CaractereLabirinto = 'p'; /* Parede */

            else if (CaractereTexto=='I') *CaractereLabirinto = 'i'; /* Início do labirinto */

            else if (CaractereTexto=='F') *CaractereLabirinto = 'f'; /* Fim do labirinto */

            else *CaractereLabirinto = 'c'; /* Caminho */

            CondRetDeMatriz = MAT_inserirElemento(LabirintoASerCriado,CaractereLabirinto);
            
            if (CondRetDeMatriz!=MAT_CondRetOK) return LAB_converteCondRet(CondRetDeMatriz); /* Retorna condição de retorno de matriz se algo estiver incorreto */

            CondRetDeMatriz = MAT_vaiParaDireita(LabirintoASerCriado);
            
            if (CondRetDeMatriz!=MAT_CondRetOK) return LAB_converteCondRet(CondRetDeMatriz); /* Retorna condição de retorno de matriz se algo estiver incorreto */

        }
        

    }

    fclose(fp); /* Fecha arquivo de texto */

    MAT_resetNoCorrente(LabirintoASerCriado); /* Coloca nó corrente na mesma posição que o primeiro nó */
    
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
LAB_tpCondRet LAB_imprimeLabirinto(MAT_tppMatriz CabecaDoLabirinto)
{
    char elementoLabirinto;
    MAT_tpCondRet CondRetDeMatriz;

    for (int l = 0; l<linhas; l++)
    {

        for (int c = 0; c<colunas; c++)
        {
            CondRetDeMatriz = MAT_obterElemento(CabecaDoLabirinto,&elementoLabirinto);
            if (CondRetDeMatriz != MAT_CondRetOK) return LAB_converteCondRet(CondRetDeMatriz);
            printf("%c",elementoLabirinto);
            CondRetDeMatriz = MAT_vaiParaDireita(CabecaDoLabirinto);
            if (CondRetDeMatriz != MAT_CondRetOK) return LAB_converteCondRet(CondRetDeMatriz);
        }

            CondRetDeMatriz = MAT_vaiParaBaixo(CabecaDoLabirinto);
            if (CondRetDeMatriz!=MAT_CondRetOK) return LAB_converteCondRet(CondRetDeMatriz); /* Retorna condição de retorno de matriz se algo estiver incorreto */


            for (int i = 0; i<colunas; i++)
            {
                CondRetDeMatriz = MAT_vaiParaEsquerda(CabecaDoLabirinto);
                if (CondRetDeMatriz!=MAT_CondRetOK) return LAB_converteCondRet(CondRetDeMatriz); /* Retorna condição de retorno de matriz se algo estiver incorreto */
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
void LAB_destroiLabirinto(MAT_tppMatriz CabecaDoLabirinto)
{
    MAT_destroiMatriz(CabecaDoLabirinto);
    
}