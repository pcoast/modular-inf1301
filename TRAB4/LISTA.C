/***************************************************************************
*   $MCI Módulo de implementação: Módulo Lista
*
*   Arquivo gerado:              LISTA.C
*   Letras identificadoras:      LIS
*
*   Projeto: Disciplina INF1301
*   Departamento: DI/PUC-Rio
*   Autores: avc - Antônio Chaves
*            jpp - João Pedro Paiva
*            pc	 - Pedro Costa
*
*   $HA Histórico de evolução:
*       Versão  Autor   Data        Observações
*       1.00    jpp     26/11/2019  Início do desenvolvimento
*
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "LISTA.H"

/***********************************************************************
*
*   $TC Tipo de dados:
*       LIS Descritor do nó da lista.
*
*
*   $ED Descrição do tipo:
*       Descreve a organização do nó.
*
***********************************************************************/

typedef struct LIS_tagNoLista
{

    void *pConteudo;
    /* Ponteiro para conteúdo do nó */

    struct LIS_tagNoLista *pNoAnterior;
    /* Ponteiro para o nó predecessor */

    struct LIS_tagNoLista *pNoProximo;
    /* Ponteiro para o nó sucessor */

#ifdef _DEBUG
    LIS_tpCabecaLista *pCabeca; /* Ponteiro para a cabeca da estrutura */
    int tamLista;               /* Tamanho (em bytes) da lista */
#endif

} LIS_tpNoLista;

/***********************************************************************
*
*   $TC Tipo de dados:
*       LIS Descritor da cabeça de uma lista.
*
*
*   $ED Descrição do tipo:
*       A cabeça da lista é o ponto de acesso para uma determinada
*       lista.
*       Por intermédio da referência para o nó corrente e dos
*       ponteiros adjascentes pode-se navegar a lista sem necessitar
*       de uma pilha.
*
***********************************************************************/

typedef struct LIS_tagLista
{

    LIS_tpNoLista *pNoPrimeiro;
    /* Ponteiro para o primeiro nó da lista */

    LIS_tpNoLista *pNoCorrente;
    /* Ponteiro para o nó corrente da lista */

    void (*ExcluirValor)(void *pConteudo);
    /* Ponteiro para a função de destruição do valor contido em um nó */

#ifdef _DEBUG
    char tipoEstrutura; /* Campo que assinala o tipo da estrutura */
    int numNos;         /* Número de nós da lista */
#endif

} LIS_tpCabecaLista;

/***** Variáveis encapuladas no módulo *****/

int tamLista; /* Tamanho (em bytes) da lista */

/***** Protôtipos das funções encapuladas no módulo *****/

static void LIS_liberarNo(LIS_tppCabecaLista pCabecaDaLista, LIS_tpNoLista *pNo);

static LIS_tpNoLista *LIS_criaNo(LIS_tppCabecaLista pCabecaDaLista, void *pConteudo);

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************
*
*	$FC Função:
*       LIS Criar a cabeça da lista.
*
*
*   $AE Assertivas de entrada esperadas:
*		Função de excluir valor não é nula.
*
*
*	$AS Assertivas de saída esperadas:
*       Cabeça da lista foi criada.
*       Primeiro nó e nó corrente são nulos.
*		Valem as assertivas estruturais da lista
*       duplamente encadeada com cabeça.
*
****************************************************/
LIS_tpCondRet LIS_criarLista(void (*ExcluirValor)(void *pDado), LIS_tppCabecaLista *pCabecaDaLista)
{

#ifdef _DEBUG
    if (!ExcluirValor)
        return LIS_CondRetFuncaoDeExclusaoNaoExiste;
#endif

    *pCabecaDaLista = (LIS_tpCabecaLista *)malloc(sizeof(LIS_tpCabecaLista));

    if (!pCabecaDaLista)
        return LIS_CondRetFaltouMemoria;

    (*pCabecaDaLista)->pNoPrimeiro = NULL;
    (*pCabecaDaLista)->pNoCorrente = NULL;

#ifdef _DEBUG
    pCabecaDaLista->tipoEstrutura = 'l';
    pCabecaDaLista->numNos = 0;
#endif

    (*pCabecaDaLista)->ExcluirValor = ExcluirValor;

    return LIS_CondRetOK; /* Retorna condição de teste bem sucedido */
}

/***************************************************
*
*	$FC Função:
*       LIS Destruir lista nó por nó e liberar
*       cabeça da lista.
*
*
*	$AE Assertivas de entrada esperadas:
*       Função independe da posição do nó corrente.
*       Cabeça da lista != NULL.
*		Valem as assertivas estruturais da lista
*       duplamente encadeada com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Lista foi destruída.
*		Valem as assertivas estruturais da lista
*       duplamente encadeada com cabeça.
*
****************************************************/
LIS_tpCondRet LIS_DestruirLista(LIS_tppCabecaLista pCabecaDaLista)
{

    LIS_tpNoLista *pNo;
    LIS_tpNoLista *pNoProximo;

#ifdef _DEBUG
    if (!pCabecaDaLista)
        return LIS_CondRetListaNaoExiste;
#endif

    pNo = pCabecaDaLista->pNoPrimeiro;
    while (pNo)
    {
        pNoProximo = pNo->pNoProximo;
        LIS_liberarNo(pCabecaDaLista, pNo);
        pNo = pNoProximo;
    }

    pCabecaDaLista->pNoPrimeiro = NULL;
    pCabecaDaLista->pNoCorrente = NULL;

    free(pCabecaDaLista);

    return LIS_CondRetOK; /* Retorna condição de teste bem sucedido */
}

/*******************************************************
*
*	$FC Função:
*       LIS Insere novo nó antes do nó corrente.
*
*
*	$AE Assertivas de entrada esperadas:
*       Nó corrente aponta para o nó posterior à
*       posição onde desejamos inserir um nó novo
*       ou lista é vazia.
*		Cabeça da lista != NULL.
*		Valem as assertivas estruturais da lista
*       duplamente encadeada com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Um novo nó foi inserido antes do nó corrente ou
*       nó novo é o único nó da lista.
*       Nó corrente agora aponta para o nó novo.
*		Valem as assertivas estruturais da lista
*       duplamente encadeada com cabeça.
*
********************************************************/
LIS_tpCondRet LIS_InserirNoAntes(LIS_tppCabecaLista pCabecaDaLista, void *pConteudo)
{

    LIS_tpNoLista *pNo;

#ifdef _DEBUG
    if (!pCabecaDaLista)
        return LIS_CondRetListaNaoExiste;
#endif

    /* Criar nó a inerir antes */

    pNo = LIS_criaNo(pCabecaDaLista, pConteudo);

    if (!pNo)
        return LIS_CondRetFaltouMemoria;

    /* Encadear o nó antes do nó corrente */

    if (!pCabecaDaLista->pNoCorrente)      /* Lista não possui nós */
        pCabecaDaLista->pNoPrimeiro = pNo; /* Nó novo é o primeiro nó */

    else /* Lista possui nós. Vamos inserir antes do nó corrente */
    {
        if (pCabecaDaLista->pNoCorrente->pNoAnterior) /* Nó corrente possui anterior */
        {
            pNo->pNoAnterior = pCabecaDaLista->pNoCorrente->pNoAnterior;
            /* Nó anterior do nó novo é o anterior do corrente */
            pCabecaDaLista->pNoCorrente->pNoAnterior->pNoProximo = pNo;
            /* Nó posterior ao anterior do corrente é o novo nó */
        }
        else /* Nó corrente é o primeiro nó */
            pCabecaDaLista->pNoPrimeiro = pNo;
        /* Primeiro nó da lista é o nó novo */

        pNo->pNoProximo = pCabecaDaLista->pNoCorrente;
        /* Nó posterior ao nó novo é o corrente */
        pCabecaDaLista->pNoCorrente->pNoAnterior = pNo;
        /* Nó anterior ao corrente é o nó novo */
    }

    pCabecaDaLista->pNoCorrente = pNo;
    /* Nó corrente é o nó novo */

    return LIS_CondRetOK; /* Retorna condição de teste bem sucedido */
}

/*******************************************************
*
*	$FC Função:
*       LIS Insere novo nó após nó corrente da lista.
*
*
*	$AE Assertivas de entrada esperadas:
*       Nó corrente aponta para o nó anterior à
*       posição onde desejamos inserir um nó novo
*       ou lista é vazia.
*		Cabeça da lista != NULL.
*		Valem as assertivas estruturais da lista
*       duplamente encadeada com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Um novo nó foi inserido após o corrente ou
*       nó novo é o único nó da lista.
*       Nó corrente agora aponta para o nó novo.
*		Valem as assertivas estruturais da lista
*       duplamente encadeada com cabeça.
*
********************************************************/
LIS_tpCondRet LIS_InserirNoApos(LIS_tppCabecaLista pCabecaDaLista, void *pConteudo)

{
    LIS_tpNoLista *pNo;

#ifdef _DEBUG
    if (!pCabecaDaLista)
        return LIS_CondRetListaNaoExiste;
#endif

    /* Criar nó a inerir antes */

    pNo = LIS_criaNo(pCabecaDaLista, pConteudo);

    if (!pNo)
        return LIS_CondRetFaltouMemoria;

    /* Encadear o nó antes do nó corrente */

    if (!pCabecaDaLista->pNoCorrente)      /* Lista não possui nós */
        pCabecaDaLista->pNoPrimeiro = pNo; /* Nó novo é o primeiro nó */

    else /* Lista possui nós. Vamos inserir depois do nó corrente */
    {

        pNo->pNoProximo = pCabecaDaLista->pNoCorrente->pNoProximo;
        /* Nó posterior do nó novo é o posterior do corrente */
        pCabecaDaLista->pNoCorrente->pNoProximo->pNoAnterior = pNo;
        /* Nó anterior ao posterior do corrente é o novo nó */
        pNo->pNoAnterior = pCabecaDaLista->pNoCorrente;
        /* Nó posterior ao nó novo é o corrente */
        pCabecaDaLista->pNoCorrente->pNoProximo = pNo;
        /* Nó anterior ao corrente é o nó novo */
    }

    pCabecaDaLista->pNoCorrente = pNo;
    /* Nó corrente é o nó novo */

    return LIS_CondRetOK; /* Retorna condição de teste bem sucedido */
}

/*******************************************************
*
*	$FC Função:
*       LIS Exclui nó apontado pelo nó corrente
*       da lista.
*
*
*	$AE Assertivas de entrada esperadas:
*       Ponteiro corrente aponta para o nó
*       que desejamos excluir da lista.
*       Conteúdo do nó corrente não é nulo.
*		Cabeça da lista != NULL.
*		Valem as assertivas estruturais da lista
*       duplamente encadeada com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Nó corrente da lista foi excluído ou
*       lista é vazia (nó corrente é nulo).
*       Novo nó corrente é o nó anterior ao nó
*       excluído, ou, caso o nó excluído foi o
*       primeiro nó da lista, o novo nó corrente é o
*       nó posterior ao nó excluído (atualmente o
*       primeiro nó da lista).
*		Valem as assertivas estruturais da lista
*       duplamente encadeada com cabeça.
*
********************************************************/
LIS_tpCondRet LIS_ExcluirNo(LIS_tppCabecaLista pCabecaDaLista)
{

    LIS_tpNoLista *pNo;

#ifdef _DEBUG
    if (!pConteudo)
        return LIS_CondRetConteudoNaoExiste
    if (!pCabecaDaLista)
        return LIS_CondRetListaNaoExiste;
#endif

    if (!pCabecaDaLista->pNoCorrente) /* Lista não possui nós */
        return LIS_CondRetListaVazia; /* Retorna condição de lista vazia */

    pNo = pCabecaDaLista->pNoCorrente;

    /* Desencadeia à esquerda */

    if (pNo->pNoAnterior) /* Nó a ser excluído possui anterior */
    {
        pNo->pNoAnterior->pNoProximo = pNo->pNoProximo;
        /* Conecta nó anterior ao nó posterior do nó a ser excluído */
        pCabecaDaLista->pNoCorrente = pNo->pNoAnterior;
        /* Nó corrente passa a ser o nó anterior do que será excluído */
    }
    else /* Queremos excluir o primeiro nó da lista */
    {
        pCabecaDaLista->pNoCorrente = pNo->pNoProximo;
        /* Nó corrente passa a ser o nó posterior ao que será excluído */
        pCabecaDaLista->pNoPrimeiro = pCabecaDaLista->pNoCorrente;
        /* Primeiro nó da lista passa a ser o nó posterior ao que será 
        excluído */
    }

    /* Desencadeia à direita */

    if (pNo->pNoProximo) /* Nó a ser excluído não é o último nó da lista */
        pNo->pNoProximo->pNoAnterior = pNo->pNoAnterior;
    /* Conecta nó posterior ao nó anterior do nó a ser excluído */

    LIS_liberarNo(pCabecaDaLista, pNo);

    return LIS_CondRetOK; /* Retorna condição de teste bem sucedido */
}

/*******************************************************
*
*	$FC Função:
*       LIS Obtém conteúdo do nó corrente da lista.
*
*
*	$AE Assertivas de entrada esperadas:
*       Nó corrente aponta para o nó
*       de onde deseja-se obter o conteúdo.
*       Conteúdo do nó corrente não é nulo.
*		Cabeça da lista != NULL.
*		Valem as assertivas estruturais da lista
*       duplamente encadeada com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Conteudo do nó corrente da lista foi obtido
*		Valem as assertivas estruturais da lista
*       duplamente encadeada com cabeça.
*
********************************************************/
LIS_tpCondRet LIS_obterConteudo(LIS_tppCabecaLista pCabecaDaLista, void **ppConteudo)
{

#ifdef _DEBUG
    if (!pCabecaDaLista)
        return LIS_CondRetListaNaoExiste;
#endif

    if (!pCabecaDaLista->pNoCorrente->pConteudo) /* Nó corrente não possui lista */
        return LIS_CondRetNoVazio;            /* Retorna condição de falha na obtenção 
        do conteúdo do nó corrente */

    *ppConteudo = pCabecaDaLista->pNoCorrente->pConteudo;
    /* Passa, por referência, o ponteiro do conteúdo do nó corrente */

    return LIS_CondRetOK; /* Retorna condição de teste bem sucedido */
}

/*****  Código das funções encapsuladas no módulo  *****/

/*******************************************************
*
*	$FC Função:
*       LIS Libera espaço alocado apontado pelo nó
*       corrente da lista.
*
*
*	$AE Assertivas de entrada esperadas:
*       Ponteiro corrente aponta para o nó
*       que deseja-se liberar.
*       Conteúdo do nó corrente não é nulo.
*		Cabeça da lista != NULL.
*		Valem as assertivas estruturais da lista
*       duplamente encadeada com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Espaço alocado apontado pelo nó corrente da
*       lista foi liberado.
*		Valem as assertivas estruturais da lista
*       duplamente encadeada com cabeça.
*
********************************************************/
void LIS_liberarNo(LIS_tppCabecaLista pCabecaDaLista, LIS_tpNoLista *pNo)
{

#ifdef _DEBUG
    if (!pConteudo)
        return LIS_CondRetConteudoNaoExiste
    if (!pCabecaDaLista)
        return LIS_CondRetListaNaoExiste;
#endif

    if (pNo->pConteudo)                               /* Nó possui conteúdo */
        pCabecaDaLista->ExcluirValor(pNo->pConteudo); /* Libera conteúdo do nó */

    free(pNo); /* Libera espaço apontado pelo endereço do nó */

#ifdef _DEBUG
    pCabecaDaLista->numNos--;
#endif
}

/*******************************************************
*
*	$FC Função:
*       LIS Cria nó da lista.
*
*
*	$AE Assertivas de entrada esperadas:
*       Conteúdo a ser inserido no nó não é nulo. 
*		Valem as assertivas estruturais da lista
*       duplamente encadeada com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Nó foi criado.
*		Valem as assertivas estruturais da lista
*       duplamente encadeada com cabeça.
*
********************************************************/
LIS_tpNoLista *LIS_criaNo(void *pConteudo
#ifdef _DEBUG,
                              LIS_tppCabecaLista pCabecaDaLista
#endif
)
{

    LIS_tpNoLista *pNo;

#ifdef _DEBUG
    if (!pConteudo)
        return LIS_CondRetConteudoNaoExiste
    if (!pCabecaDaLista)
        return LIS_CondRetListaNaoExiste;
#endif

    pNo = (LIS_tpNoLista *)malloc(sizeof(LIS_tpNoLista));
    /* Malloc do nó novo */

    if (!pNo) /* Malloc do nó falhou. Retorna nulo */
        return NULL;

    pNo->pConteudo = pConteudo;
    pNo->pNoAnterior = NULL;
    pNo->pNoProximo = NULL;

#ifdef _DEBUG
    pCabecaDaLista->numNos++;      /* Incrementa número de nós da lista */
    pNo->pCabeca = pCabecaDaLista; /* Ponteiro para a cabeca da estrutura */
// pNo->tamLista +=  /* Tamanho (em bytes) da lista */
#endif

    return pNo;
}