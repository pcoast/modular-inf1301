/***************************************************************************
*   $MCI Módulo de implementação: Módulo Matriz
*
*   Arquivo gerado:              MATRIZ.h
*   Letras identificadoras:      MAT
*
*   Projeto: Disciplina INF1301
*   Departamento: DI/PUC-Rio
*   Autores: avc - Antônio Chaves
*            jpp - João Pedro Paiva
			 pc	 - Pedro Costa
*
*   $HA Histórico de evolução:
*       Versão  Autor   Data        Observações
*       1.00    avc     25/08/2019  Início do desenvolvimento (protótipos de 
*                                   funções)
*       2.00    jpp     29/08/2019  Continuação do desenvolvimento (tpNoMatriz, 
*                                   tpMatriz, MAT_cria e MAT_criaNo)
*       3.00    jpp     03/09/2019  Continuação do desenvolvimento 
*                                   (MAT_vaiPara e MAT_destroi)
*       4.00    jpp     04/09/2019  Uniformização da interface das funções e
*                                   das condições de retorno, comentários no
*                                   código
*		5.00	pc		07/09/2019	Correção de erros e adptação à mudanças no módulo de definição.
*       6.00    pc      08/09/2019  Correção da destrução
*       7.00    jpp     22/09/2019  AE e AS
*       8.00    jpp     24/09/2019  Matriz_cria nova
*		8.01	pc		06/10/2019	Correção de erros
*
***************************************************************************/

#include "MATRIZ.H"
#include <stdlib.h>

/***********************************************************************
*
*   $TC Tipo de dados:
*       MAT Descritor do nó da matriz.
*
*
*   $ED Descrição do tipo:
*       Descreve a organização do nó.
*
***********************************************************************/

typedef struct tgNoMatriz
{

    struct tgNoMatriz *pNoCima;
    /* Ponteiro para nó de cima
		*
		*$EED Assertivas estruturais
		*    */

    struct tgNoMatriz *pNoEsquerda;
    /* Ponteiro para nó da esquerda
			   *
			   *$EED Assertivas estruturais
			   *    */

    void *Elemento;
    /* Elemento contido no nó */

    struct tgNoMatriz *pNoDireita;
    /* Ponteiro para nó da direita
			   *
			   *$EED Assertivas estruturais
			   *    */

    struct tgNoMatriz *pNoBaixo;
    /* Ponteiro para nó de baixo
			   *
			   *$EED Assertivas estruturais
			   *    */

} tpNoMatriz;

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

typedef struct tgMatriz
{

    tpNoMatriz *pNoPrimeiro;
    /* Ponteiro para a o primeiro nó da matriz, escolhido arbitrariamente */

    tpNoMatriz *pNoCorr;
    /* Ponteiro para o nó corrente da matriz */

    void (*destruirElemento)(void *elemento);
    /* Ponteiro para a função de destrução de um elemento */

} tpMatriz;

/***** Protótipos das funções encapsuladas no módulo *****/

static tpNoMatriz *MAT_criaNo(void);

static MAT_tpCondRet MAT_adicionaColuna(MAT_tppMatriz CabecaDaMatriz, char numColunasAAdicionar);

static MAT_tpCondRet MAT_adicionaLinha(MAT_tppMatriz CabecaDaMatriz, char numLinhasAAdicionar);

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***************************************************
*
*   $FC Função:
*       MAT Criar um nó da matriz.
*
*
*	$AE Assertivas de entrada esperadas:
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Nó foi criado.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
****************************************************/
tpNoMatriz *MAT_criaNo(void)
{
    tpNoMatriz *NoMatrizCriado;

    NoMatrizCriado = (tpNoMatriz *)malloc(sizeof(tpNoMatriz));

    if (NoMatrizCriado == NULL)
        return NULL;

    NoMatrizCriado->Elemento = NULL;
    NoMatrizCriado->pNoBaixo = NULL;
    NoMatrizCriado->pNoCima = NULL;
    NoMatrizCriado->pNoDireita = NULL;
    NoMatrizCriado->pNoEsquerda = NULL;

    return NoMatrizCriado;
}

/*******************************************************
*
*	$FC Função:
*       MAT Nó corrente da Matriz se torna o nó
*       Acima do nó corrente atual.
*
*
*	$AE Assertivas de entrada esperadas:
*		Cabeça da matriz != NULL.
*       Nó corrente aponta para o nó abaixo do nó
*       para onde se deseja ir.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Nó corrente aponta para o nó acima do nó
*       corrente anterior.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
********************************************************/
MAT_tpCondRet MAT_vaiParaCima(MAT_tppMatriz CabecaDaMatriz)
{
    if (CabecaDaMatriz->pNoCorr->pNoCima == NULL) /* Não existe nó nesta direção */
        return MAT_CondRetNoNaoExiste;            /* Retorna condição de falha na movimentação do nó corrente */

    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoCima; /* Movimenta nó corrente na direção indicada */
    return MAT_CondRetOK;                                       /* Retorna condição de teste bem sucedido */
}

/*******************************************************
*
*	$FC Função:
*       MAT Nó corrente da Matriz se torna o nó
*       à Esquerda do nó corrente atual.
*
*
*	$AE Assertivas de entrada esperadas:
*		Cabeça da matriz != NULL.
*       Nó corrente aponta para o nó à direita do
*       nó para onde se deseja ir.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Nó corrente aponta para o nó à esquerda do
*       nó corrente anterior.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
********************************************************/
MAT_tpCondRet MAT_vaiParaEsquerda(MAT_tppMatriz CabecaDaMatriz)
{
    if (CabecaDaMatriz->pNoCorr->pNoEsquerda == NULL) /* Não existe nó nesta direção */
        return MAT_CondRetNoNaoExiste;                /* Retorna condição de falha na movimentação do nó corrente */

    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoEsquerda; /* Movimenta nó corrente na direção indicada */
    return MAT_CondRetOK;                                           /* Retorna condição de teste bem sucedido */
}

/*******************************************************
*
*	$FC Função:
*       MAT Nó corrente da Matriz se torna o nó
*       à Direita do nó corrente atual.
*
*
*	$AE Assertivas de entrada esperadas:
*		Cabeça da matriz != NULL.
*       Nó corrente aponta para o nó à esquerda do
*       nó para onde se deseja ir.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Nó corrente aponta para o nó à direita do
*       nó corrente anterior.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
********************************************************/
MAT_tpCondRet MAT_vaiParaDireita(MAT_tppMatriz CabecaDaMatriz)
{
    if (CabecaDaMatriz->pNoCorr->pNoDireita == NULL) /* Não existe nó nesta direção */
        return MAT_CondRetNoNaoExiste;               /* Retorna condição de falha na movimentação do nó corrente */

    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoDireita; /* Movimenta nó corrente na direção indicada */
    return MAT_CondRetOK;                                          /* Retorna condição de teste bem sucedido */
}

/*******************************************************
*
*	$FC Função:
*       MAT Nó corrente da Matriz se torna o nó
*       Abaixo do nó corrente atual.
*
*
*	$AE Assertivas de entrada esperadas:
*		Cabeça da matriz != NULL.
*       Nó corrente aponta para o nó acima do nó
*       para onde se deseja ir.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Nó corrente aponta para o nó abaixo do nó
*       corrente anterior.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
********************************************************/
MAT_tpCondRet MAT_vaiParaBaixo(MAT_tppMatriz CabecaDaMatriz)
{
    if (CabecaDaMatriz->pNoCorr->pNoBaixo == NULL) /* Não existe nó nesta direção */
        return MAT_CondRetNoNaoExiste;             /* Retorna condição de falha na movimentação do nó corrente */

    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoBaixo; /* Movimenta nó corrente na direção indicada */
    return MAT_CondRetOK;                                        /* Retorna condição de teste bem sucedido */
}

/***************************************************
*
*   $FC Função:
*       MAT Adiciona um certo número de colunas
*       à matriz.
*
*
*	$AE Assertivas de entrada esperadas:
*       Nó corrente aponta para algum nó da
*       primeira linha.
*       Cabeça da Matriz != NULL.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       O número desejado de colunas foram
*       adicionadas à matriz.
*       Primeiro nó e nó corrente apontam para o nó
*       mais à esquerda e mais acima possível.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
****************************************************/
MAT_tpCondRet MAT_adicionaColuna(MAT_tppMatriz CabecaDaMatriz, char numColunasAAdicionar)
{
    while (CabecaDaMatriz->pNoCorr->pNoDireita) /* Loop para chegar no primeiro nó da última coluna da matriz */
        MAT_vaiParaDireita(CabecaDaMatriz);

    /* No bloco de código abaixo o nó corrente percorrerá a última coluna da matriz criando nós à
	direita para gerar uma nova coluna */

    while (numColunasAAdicionar--)
    {
        do
        {
            CabecaDaMatriz->pNoCorr->pNoDireita = MAT_criaNo(); /* Cria nó da coluna nova à direita do nó corrente */
            if (!CabecaDaMatriz->pNoCorr->pNoDireita)
                return MAT_CondRetFaltouMemoria; /* Nó criado == NULL. Retorna condição de insufuciência de memória */

            CabecaDaMatriz->pNoCorr->pNoDireita->pNoEsquerda = CabecaDaMatriz->pNoCorr; /* Conecta nó da coluna nova ao nó corrente */
            if (CabecaDaMatriz->pNoCorr->pNoCima)                                       /* Se existir nó acima do nó corrente (não é o primeiro nó da coluna) */
            {
                CabecaDaMatriz->pNoCorr->pNoDireita->pNoCima = CabecaDaMatriz->pNoCorr->pNoCima->pNoDireita;  /* Conecta o nó da coluna nova ao acima dele */
                CabecaDaMatriz->pNoCorr->pNoCima->pNoDireita->pNoBaixo = CabecaDaMatriz->pNoCorr->pNoDireita; /* Conecta o de cima do novo ao novo */
            }

        } while (MAT_vaiParaBaixo(CabecaDaMatriz) != MAT_CondRetNoNaoExiste); /* Nó corrente desce a coluna. Para somente quando caminhar para o NULL */

        MAT_vaiParaDireita(CabecaDaMatriz); /* Vai para nó mais em baixo da coluna nova da matriz */

        while (CabecaDaMatriz->pNoCorr->pNoCima) /* Loop para chegar no nó mais acima da nova coluna da matriz */
            MAT_vaiParaCima(CabecaDaMatriz);
    }

    MAT_vaiParaPos(CabecaDaMatriz, 0, 0); /* Coloca nó corrente na mesma posição que o primeiro nó */

    return MAT_CondRetOK;
}

/***************************************************
*
*   $FC Função:
*       MAT Adiciona um certo número de linhas
*       à matriz.
*
*
*	$AE Assertivas de entrada esperadas:
*       Nó corrente aponta para algum nó da
*       primeira coluna.
*       Cabeça da Matriz != NULL.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       O número desejado de linhas foram
*       adicionadas à matriz.
*       Primeiro nó e nó corrente apontam para o nó
*       mais à esquerda e mais acima possível.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
****************************************************/
MAT_tpCondRet MAT_adicionaLinha(MAT_tppMatriz CabecaDaMatriz, char numLinhasAAdicionar)
{
    while (CabecaDaMatriz->pNoCorr->pNoBaixo) /* Loop para chegar no primeiro nó da última linha da matriz */
        MAT_vaiParaBaixo(CabecaDaMatriz);

    /* No bloco de código abaixo o nó corrente percorrerá a última linha da matriz criando nós
	abaixo para gerar uma nova linha */

    while (numLinhasAAdicionar--)
    {
        do
        {
            CabecaDaMatriz->pNoCorr->pNoBaixo = MAT_criaNo(); /* Cria nó da linha nova abaixo do nó corrente atual */

            if (!CabecaDaMatriz->pNoCorr->pNoBaixo)
                return MAT_CondRetFaltouMemoria; /* Nó criado == NULL. Retorna condição de insufuciência de memória */

            CabecaDaMatriz->pNoCorr->pNoBaixo->pNoCima = CabecaDaMatriz->pNoCorr; /* Conecta nó da linha nova ao nó corrente atual */

            if (CabecaDaMatriz->pNoCorr->pNoEsquerda) /* Se existir nó à esquerda do nó corrente atual (não é o primeiro nó da linha) */
            {
                CabecaDaMatriz->pNoCorr->pNoBaixo->pNoEsquerda = CabecaDaMatriz->pNoCorr->pNoEsquerda->pNoBaixo; /* Conecta o nó da linha nova ao à esquerda dele */
                CabecaDaMatriz->pNoCorr->pNoEsquerda->pNoBaixo->pNoDireita = CabecaDaMatriz->pNoCorr->pNoBaixo;  /* Conecta o de à esquerda do novo ao novo */
            }
        } while (MAT_vaiParaDireita(CabecaDaMatriz) != MAT_CondRetNoNaoExiste); /* Nó corrente anda para a direita na linha. Para somente quando caminhar para o NULL */

        MAT_vaiParaBaixo(CabecaDaMatriz); /* Vai para nó mais a direita da linha nova da matriz */

        while (CabecaDaMatriz->pNoCorr->pNoCima) /* Loop para chegar no nó mais à esquerda da nova linha da matriz */
            MAT_vaiParaEsquerda(CabecaDaMatriz);
    }

    MAT_vaiParaPos(CabecaDaMatriz, 0, 0); /* Coloca nó corrente na mesma posição que o primeiro nó */

    return MAT_CondRetOK;
}

/***************************************************
*
*	$FC Função:
*       MAT Criar a estrutura da matriz. Cria os
*       nós com a MAT_criaNo(), cria as colunas e
*       as interconecta com a MAT_adicionaColuna() 
*       e cria as linhas e as interconecta com a
*       MAT_adicionaLinha().
*
*
*   $AE Assertivas de entrada esperadas:
*       Dimensões da matriz desejada estão entre
*       (2x1 ou 1x2) e 10x10.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Matriz foi criada com as dimensões desejadas.
*       Primeiro nó e nó corrente apontam para o nó
*       mais à esquerda e mais acima possível.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
****************************************************/
MAT_tpCondRet MAT_cria(char Linhas, char Colunas, void (*destruirElemento)(void *elemento), MAT_tppMatriz *MatrizCriada)
{
    tpNoMatriz *PrimeiroNo;

    PrimeiroNo = MAT_criaNo(); /* Cria primeiro nó da Matriz */
    if (!PrimeiroNo)
        return MAT_CondRetFaltouMemoria; /* Primeiro Nó == NULL */

    (*MatrizCriada) = (MAT_tppMatriz)malloc(sizeof(tpMatriz)); /* Malloca cabeça da matriz */
    if (!(*MatrizCriada))
        return MAT_CondRetFaltouMemoria; /* Cabeça da Matriz == NULL, Retorna condição de insufuciência de memória */

    (*MatrizCriada)->pNoPrimeiro = PrimeiroNo;

    (*MatrizCriada)->pNoCorr = PrimeiroNo; /* Primeiro nó e nó corrente são o mesmo nó */

    (*MatrizCriada)->destruirElemento = destruirElemento;

    /* Cria primeira linha da matriz a partir do primeiro nó.
	Subtrai primeiro porque um nó (o primeiro) está feito */
    if (MAT_adicionaColuna((*MatrizCriada), Colunas) == MAT_CondRetFaltouMemoria)
        return MAT_CondRetFaltouMemoria;
    /* Cada chamada cria uma coluna nova na matriz. Já que só há um nó na matriz,
	cada chamada da função só cria novos nós à direita do primeiro, formando a primeira linha */

    /* Cria as linhas da matriz extendendo a primeira linha.
	Subtrai primeiro porque uma linha (a primeira) está feita */

    if (MAT_adicionaLinha((*MatrizCriada), Linhas) == MAT_CondRetFaltouMemoria)
        return MAT_CondRetFaltouMemoria;
    /* Cada chamada cria uma linha nova na matriz. */

    return MAT_CondRetOK; /* Retorna condição de teste bem sucedido */
}

/***************************************************
*
*	$FC Função:
*       MAT Destruir matriz linha por linha.
*
*
*	$AE Assertivas de entrada esperadas:
*       Nó corrente da matriz não aponta necessariamente
*       para a posição inicial (mesma posição que o
*       primeiro nó).
*       Cabeça da matriz != NULL.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Matriz foi destruída.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
****************************************************/
void MAT_destroi(MAT_tppMatriz CabecaDaMatriz)
{
    tpNoMatriz *PrimeiroNoDaLinhaDeBaixo;

    MAT_vaiParaPos(CabecaDaMatriz, 0, 0); /* Coloca nó corrente na mesma posição que o primeiro nó */

    do
    {
        PrimeiroNoDaLinhaDeBaixo = CabecaDaMatriz->pNoPrimeiro->pNoBaixo;  /* Guarda primeiro nó da linha de baixo para deletar a linha seguinte */
        CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoPrimeiro->pNoDireita; /* Nó corrente é o nó à direita do primeiro nó da matriz */
        do
        {
            CabecaDaMatriz->destruirElemento(CabecaDaMatriz->pNoPrimeiro->Elemento); /* Libera o ponteiro do elemento */
            free(CabecaDaMatriz->pNoPrimeiro);                                       /* Libera primeiro nó da matriz */
            if (CabecaDaMatriz->pNoCorr == NULL)                                     /* Liberou o último nó desta linha da matriz */
                break;                                                               /* Segue para próxima linha da matriz */
            CabecaDaMatriz->pNoPrimeiro = CabecaDaMatriz->pNoCorr;                   /* Primeiro nó da matriz é o mesmo que o corrente */
            CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoDireita;           /* Nó corrente é o nó à direita do primeiro nó da matriz */
        } while (1);                                                                 /* Para sempre */

        CabecaDaMatriz->pNoPrimeiro = PrimeiroNoDaLinhaDeBaixo; /* Primeiro nó da matriz se torna o primeiro nó da linha de baixo */
    } while (PrimeiroNoDaLinhaDeBaixo != NULL);                 /* Se a condição for quebrada, então liberou a última linha da matriz */

    free(CabecaDaMatriz); /* Libera cabeça da matriz */
}

/*******************************************************
*
*	$FC Função:
*       MAT Inserir elemento no nó corrente da 
*       matriz
*
*
*	$AE Assertivas de entrada esperadas:
*		Cabeça da matriz != NULL.
*       Nó corrente aponta para o nó
*       onde deseja-se inserir o elemento.
*		elemento != NULL.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Elemento foi inserido no nó corrente da
*       matriz.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
********************************************************/
MAT_tpCondRet MAT_inserir(MAT_tppMatriz CabecaDaMatriz, void *elemento)
{
    if (CabecaDaMatriz->pNoCorr->Elemento != NULL) /* Nó corrente já possui lista */
        return MAT_CondRetNoOcupado;               /* Retorna condição de falha na incorporação da lista no nó corrente */

    CabecaDaMatriz->pNoCorr->Elemento = elemento; /* Incopora ponteiro da lista passado por referência no nó corrente */

    return MAT_CondRetOK; /* Retorna condição de teste bem sucedido */
}

/*******************************************************
*
*	$FC Função:
*       MAT Obtém elemento do nó corrente da matriz
*
*
*	$AE Assertivas de entrada esperadas:
*       Ponteiro corrente aponta para o nó
*       de onde deseja-se obter o conteúdo.
*		Cabeça da matriz != NULL.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Elemento foi obtido do nó corrente da matriz.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
********************************************************/
MAT_tpCondRet MAT_obterElemento(MAT_tppMatriz CabecaDaMatriz, void **elemento)
{
    if (CabecaDaMatriz->pNoCorr->Elemento == NULL) /* Nó corrente não possui lista */
        return MAT_CondRetNoVazio;                 /* Retorna condição de falha na obtenção da lista incorporada no nó corrente */

    *elemento = CabecaDaMatriz->pNoCorr->Elemento; /* Passa, por referência, o ponteiro da lista incorporado no nó corrente */

    return MAT_CondRetOK; /* Retorna condição de teste bem sucedido */
}

/*******************************************************
*
*	$FC Função:
*       MAT No corrente da matriz vai para direção
*       específica.
*
*
*	$AE Assertivas de entrada esperadas:
*       Nó corrente não vai para uma posição Nula.
*		Cabeça da matriz != NULL.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Nó corrente da matriz foi para a direção desejada.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
********************************************************/
MAT_tpCondRet MAT_vaiParaDir(MAT_tppMatriz CabecaDaMatriz, MAT_tpDir Direcao)
{
    switch (Direcao)
    {
    case MAT_DirCima:
        MAT_vaiParaCima(CabecaDaMatriz);
        break;

    case MAT_DirDireita:
        MAT_vaiParaDireita(CabecaDaMatriz);
        break;

    case MAT_DirBaixo:
        MAT_vaiParaBaixo(CabecaDaMatriz);
        break;

    case MAT_DirEsquerda:
        MAT_vaiParaEsquerda(CabecaDaMatriz);
        break;

    default:
        break;
    }

    return MAT_CondRetOK; /* Retorna condição de teste bem sucedido */
}

/*******************************************************
*
*	$FC Função:
*       MAT Leva nó corrente da matriz para posição
*       específica
*
*
*	$AE Assertivas de entrada esperadas:
*       A posição desejada pertence à matriz. 
*       Nó corrente da matriz não aponta necessariamente
*       para a posição inicial (mesma posição que o
*       primeiro nó).
*		Cabeça da matriz != NULL.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Nó corrente da matriz está na posição desejada.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
********************************************************/
MAT_tpCondRet MAT_vaiParaPos(MAT_tppMatriz CabecaDaMatriz, char Coluna, char Linha)
{
    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoPrimeiro; /* Coloca nó corrente na mesma posição que o primeiro nó */

    while (Coluna--)
        MAT_vaiParaDireita(CabecaDaMatriz);

    while (Linha--)
        MAT_vaiParaBaixo(CabecaDaMatriz);

    return MAT_CondRetOK; /* Retorna condição de teste bem sucedido */
}