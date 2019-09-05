#include "MATRIZ.h"
#include <stdlib.h>
/***********************************************************************
*
*  $TC Tipo de dados: MAT Descritor do nó da matriz
*
*
*  $ED Descrição do tipo
*     Descreve a organização do nó
*
***********************************************************************/

typedef struct tgNoMatriz
{

    struct tgNoMatriz *pNoEsquerdaSuperior;
    /* Ponteiro para nó da esquerda superior
               *
               *$EED Assertivas estruturais
               *    */

    struct tgNoMatriz *pNoCima;
    /* Ponteiro para nó de cima
               *
               *$EED Assertivas estruturais
               *    */

    struct tgNoMatriz *pNoDireitaSuperior;
    /* Ponteiro para nó da direita superior
               *
               *$EED Assertivas estruturais
               *    */

    struct tgNoMatriz *pNoEsquerda;
    /* Ponteiro para nó da esquerda
               *
               *$EED Assertivas estruturais
               *    */

    void *Lista;
    /* Lista contida no nó */

    struct tgNoMatriz *pNoDireita;
    /* Ponteiro para nó da direita
               *
               *$EED Assertivas estruturais
               *    */

    struct tgNoMatriz *pNoEsquerdaInferior;
    /* Ponteiro para nó da esquerda inferior
               *
               *$EED Assertivas estruturais
               *    */

    struct tgNoMatriz *pNoBaixo;
    /* Ponteiro para nó de baixo
               *
               *$EED Assertivas estruturais
               *    */

    struct tgNoMatriz *pNoDireitaInferior;
    /* Ponteiro para nó da direita inferior
               *
               *$EED Assertivas estruturais
               *    */

} tpNoMatriz;

/***********************************************************************
*
*  $TC Tipo de dados: ARV Descritor da cabeça de uma matriz
*
*
*  $ED Descrição do tipo
*     A cabeça da matriz é o ponto de acesso para uma determinada matriz.
*     Por intermédio da referência para o nó corrente e dos ponteiros
*     adjascentes pode-se navegar a matriz sem necessitar de uma pilha.
*
***********************************************************************/

typedef struct tgMatriz
{

    tpNoMatriz *pNoPrimeiro;
    /* Ponteiro para a o primeiro nó da matriz, escolhido arbitrariamente */

    tpNoMatriz *pNoCorr;
    /* Ponteiro para o nó corrente da matriz */

} tpMatriz;

/***************************************************
*
*	$FC Função: MAT Criar um nó da matriz
*
****************************************************/
tpNoMatriz *MAT_criaNo(void)
{
    tpNoMatriz *NoMatrizCriado;

    NoMatrizCriado = (tpNoMatriz*) malloc(sizeof(tpNoMatriz));

    if (NoMatrizCriado == NULL)
        return NULL;

    NoMatrizCriado->Lista = NULL;
    NoMatrizCriado->pNoBaixo = NULL;
    NoMatrizCriado->pNoCima = NULL;
    NoMatrizCriado->pNoDireita = NULL;
    NoMatrizCriado->pNoDireitaInferior = NULL;
    NoMatrizCriado->pNoDireitaSuperior = NULL;
    NoMatrizCriado->pNoEsquerda = NULL;
    NoMatrizCriado->pNoEsquerdaInferior = NULL;
    NoMatrizCriado->pNoEsquerdaSuperior = NULL;

    return NoMatrizCriado;
}

/***************************************************
*
*	$FC Função: MAT Criar a estrutura da matriz,
*       criando os nós com a MAT_criaNo() e os
*       interconectando.
*
****************************************************/
MAT_tpCondRet MAT_cria(char LinhasEColunas, tpMatriz *MatrizCriada)
{

    tpNoMatriz *VetorDeNosDaMatriz[LinhasEColunas][LinhasEColunas];

    if (MatrizCriada == NULL) /* Memória não pôde ser alocada */
        return MAT_CondRetFaltouMemoria; /* Retorna condição de insufficiência de memória */

    
    for (int i = 0; i<LinhasEColunas; i++) /* Cria vetor bi-dimensional nxn com os nós da matriz */
    {
        for (int j = 0; j<LinhasEColunas; j++)
        {
            VetorDeNosDaMatriz[i][j] = MAT_criaNo();
            if (VetorDeNosDaMatriz[i][j] == NULL) /* Memória não pôde ser alocada */
                return MAT_CondRetFaltouMemoria; /* Retorna condição de insufficiência de memória */
        }
    }

    for (int i = 0; i<LinhasEColunas; i++) /* Interconecta os nós da matriz dentro do vetor bi-dimensional nxn */
    {
        for (int j = 0; j<LinhasEColunas; j++)
        {
            if (i+1<=LinhasEColunas) /* Os condicionais garantem que os limites laterais da matriz não sejam ultrapassados pelos índices */
                VetorDeNosDaMatriz[i][j]->pNoBaixo = VetorDeNosDaMatriz[i+1][j];
            if (i-1>=0)
                VetorDeNosDaMatriz[i][j]->pNoCima = VetorDeNosDaMatriz[i-1][j];
            if (j+1 <= LinhasEColunas)
                VetorDeNosDaMatriz[i][j]->pNoDireita = VetorDeNosDaMatriz[i][j+1];
            if (i+1 <= LinhasEColunas && j+1 <= LinhasEColunas)
                VetorDeNosDaMatriz[i][j]->pNoDireitaInferior = VetorDeNosDaMatriz[i+1][j+1];
            if (i-1 >= 0 && j+1 <= LinhasEColunas)
                VetorDeNosDaMatriz[i][j]->pNoDireitaSuperior = VetorDeNosDaMatriz[i-1][j+1];
            if (j-1 >= 0)
                VetorDeNosDaMatriz[i][j]->pNoEsquerda = VetorDeNosDaMatriz[i][j-1];
            if (i+1 <= LinhasEColunas && j-1 >= 0)
                VetorDeNosDaMatriz[i][j]->pNoEsquerdaInferior = VetorDeNosDaMatriz[i+1][j-1];
            if (i-1 >= 0 && j-1 >= 0)
                VetorDeNosDaMatriz[i][j]->pNoEsquerdaSuperior = VetorDeNosDaMatriz[i-1][j-1];
        }
    }

    MatrizCriada->pNoPrimeiro = VetorDeNosDaMatriz[0][0]; /* Primeiro nó da matriz é o nó mais encima e mais à esquerda possível */
    MatrizCriada->pNoCorr = MatrizCriada->pNoPrimeiro; /* Nó corrente é inicialmente o mesmo que o primeiro nó */

    return MAT_CondRetOK;

}

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       à Esquerda Superior do nó corrente atual.
*
*	$EAE Assertivas de entrada esperadas
*		CabecaDaMatriz != NULL
*
********************************************************/
MAT_tpCondRet MAT_vaiParaEsquerdaSuperior(tpMatriz *CabecaDaMatriz)
{
    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoEsquerdaSuperior;
    return MAT_CondRetOK;

}

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       Acima do nó corrente atual.
*
*	$EAE Assertivas de entrada esperadas
*		CabecaDaMatriz != NULL
*
********************************************************/
MAT_tpCondRet MAT_vaiParaCima(tpMatriz *CabecaDaMatriz)
{
    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoCima;
    return MAT_CondRetOK;

}

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       à Direita Superior do nó corrente atual.
*
*	$EAE Assertivas de entrada esperadas
*		CabecaDaMatriz != NULL
*
********************************************************/
MAT_tpCondRet MAT_vaiParaDireitaSuperior(tpMatriz *CabecaDaMatriz)
{
    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoDireitaSuperior;
    return MAT_CondRetOK;

}

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       à Esquerda do nó corrente atual.
*
*	$EAE Assertivas de entrada esperadas
*		CabecaDaMatriz != NULL
*
********************************************************/
MAT_tpCondRet MAT_vaiParaEsquerda(tpMatriz *CabecaDaMatriz)
{
    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoEsquerda;
    return MAT_CondRetOK;

}

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       à Direita do nó corrente atual.
*
*	$EAE Assertivas de entrada esperadas
*		CabecaDaMatriz != NULL
*
********************************************************/
MAT_tpCondRet MAT_vaiParaDireita(tpMatriz *CabecaDaMatriz)
{
    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoDireita;
    return MAT_CondRetOK;

}

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       à Esquerda Inferior do nó corrente atual.
*
*	$EAE Assertivas de entrada esperadas
*		CabecaDaMatriz != NULL
*
********************************************************/
MAT_tpCondRet MAT_vaiParaEsquerdaInferior(tpMatriz *CabecaDaMatriz)
{
    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoEsquerdaInferior;
    return MAT_CondRetOK;

}

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       Abaixo do nó corrente atual.
*
*	$EAE Assertivas de entrada esperadas
*		CabecaDaMatriz != NULL
*
********************************************************/
MAT_tpCondRet MAT_vaiParaBaixo(tpMatriz *CabecaDaMatriz)
{
    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoBaixo;
    return MAT_CondRetOK;

}

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       à Direita Inferior do nó corrente atual.
*
*	$EAE Assertivas de entrada esperadas
*		CabecaDaMatriz != NULL
*
********************************************************/
MAT_tpCondRet MAT_vaiParaDireitaInferior(tpMatriz *CabecaDaMatriz)
{
    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoDireitaInferior;
    return MAT_CondRetOK;

}

/***************************************************
*
*	$FC Função: MAT Destruir matriz linha por linha.
*
*	$EAE Assertivas de entrada esperadas
*		CabecaDaMatriz != NULL
*
****************************************************/
MAT_tpCondRet MAT_destroe(tpMatriz *CabecaDaMatriz)
{
    tpNoMatriz *PrimeiroNoDaLinhaDeBaixo;

    do
    {
        PrimeiroNoDaLinhaDeBaixo = CabecaDaMatriz->pNoPrimeiro->pNoBaixo;
        CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoPrimeiro->pNoDireita;
        do
        {
            free(CabecaDaMatriz->pNoPrimeiro);
            if (CabecaDaMatriz->pNoCorr == NULL) /* Se sim, então liberou o último nó desta linha da matriz */
                break;
            CabecaDaMatriz->pNoPrimeiro = CabecaDaMatriz->pNoCorr;
            CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoDireita;
        } while (1);

        CabecaDaMatriz->pNoPrimeiro = PrimeiroNoDaLinhaDeBaixo;
    } while (PrimeiroNoDaLinhaDeBaixo != NULL); /* Se a condição for quebrada, então liberou a última linha da matriz */

    free(CabecaDaMatriz);

    return MAT_CondRetOK;

}

/*******************************************************
*
*	$FC Função: MAT Inserir lista no nó atual da matriz
*
*	$EAE Assertivas de entrada esperadas:
*		Lista não nula
*
********************************************************/
tpNoMatriz *MAT_fill(void*);

