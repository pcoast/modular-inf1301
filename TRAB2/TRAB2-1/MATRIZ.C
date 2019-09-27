/***************************************************************************
*   $MCI Módulo de implementação: Módulo Matriz
*
*   Arquivo gerado:              MATRIZ.C
*   Letras identificadoras:      MAT
*
*   Projeto: Disciplina INF1301
*   Departamento: DI/PUC-Rio
*   Autores: avc - Antônio Chaves
*            jpp - João Pedro Paiva
*            pc	 - Pedro Costa
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
    /* Lista contida no nó */

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

static void MAT_criaColuna(tpNoMatriz *PrimeiroNo);

static void MAT_criaLinha(tpNoMatriz *PrimeiroNo);

/*****  Código das funções exportadas pelo módulo  *****/

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

/***************************************************
*
*   $FC Função:
*       MAT Adiciona uma coluna à matriz.
*
*
*	$AE Assertivas de entrada esperadas:
*       Cabeça da Matriz != NULL.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Coluna foi adicionada à matriz.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
****************************************************/
void MAT_adicionaColuna(MAT_tppMatriz *Matriz)
{
    tpNoMatriz *Caminha = (*Matriz)->pNoPrimeiro;

    while (Caminha->pNoDireita != NULL) /* Loop para chegar no primeiro nó da última coluna da matriz */
        Caminha = Caminha->pNoDireita; 
        
        /* Caminha percorrerá a última coluna da matriz criando nós à
        direita para gerar uma nova coluna */

    do
    {
        Caminha->pNoDireita = MAT_criaNo(); /* Cria nó da coluna nova à direita do Caminha atual */
        Caminha->pNoDireita->pNoEsquerda = Caminha; /* Conecta nó da coluna nova ao Caminha atual */
        if (Caminha->pNoCima) /* Se existir nó acima do Caminha atual (não é o primeiro nó da coluna) */
        {
            Caminha->pNoDireita->pNoCima = Caminha->pNoCima->pNoDireita; /* Conecta o nó da coluna nova ao acima dele */
            Caminha->pNoCima->pNoDireita->pNoBaixo = Caminha->pNoDireita; /* Conecta o de cima do novo ao novo */
        }
        Caminha = Caminha->pNoBaixo; /* Caminha desce a coluna */
    } while (Caminha); /* Quando caminha estiver no último nó da coluna e descer, ele se tornará NULL (equivalente a zero) */
}

/***************************************************
*
*   $FC Função:
*       MAT Adiciona uma linha à matriz.
*
*
*	$AE Assertivas de entrada esperadas:
*       Cabeça da Matriz != NULL.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Linha foi adicionada à matriz.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
****************************************************/
void MAT_adicionaLinha(MAT_tppMatriz *Matriz)
{
    tpNoMatriz *Caminha = (*Matriz)->pNoPrimeiro;

    while (Caminha->pNoBaixo != NULL) /* Loop para chegar no primeiro nó da última linha da matriz */
        Caminha = Caminha->pNoBaixo;

        /* Caminha percorrerá a última linha da matriz criando nós
        abaixo para gerar uma nova linha */

    do
    {
        Caminha->pNoBaixo = MAT_criaNo(); /* Cria nó da linha nova abaixo do Caminha atual */
        Caminha->pNoBaixo->pNoCima = Caminha; /* Conecta nó da linha nova ao Caminha atual */

        if (Caminha->pNoEsquerda) /* Se existir nó à esquerda do Caminha atual (não é o primeiro nó da linha) */
        {
            Caminha->pNoBaixo->pNoEsquerda = Caminha->pNoEsquerda->pNoBaixo; /* Conecta o nó da linha nova ao à esquerda dele */
            Caminha->pNoEsquerda->pNoBaixo->pNoDireita = Caminha->pNoBaixo; /* Conecta o de à esquerda do novo ao novo */
        }
        Caminha = Caminha->pNoDireita; /* Caminha anda para a direita na linha */
    } while (Caminha); /* Quando caminha estiver no último nó da linha e for para a direita, ele se tornará NULL (equivalente a zero) */
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
*       2x1 ou 1x2 e 10x10.
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

    (*MatrizCriada) = (MAT_tppMatriz)malloc(sizeof(tpMatriz)); /* Malloca cabeça da matriz */

    (*MatrizCriada)->pNoPrimeiro = PrimeiroNo;

    (*MatrizCriada)->pNoCorr = PrimeiroNo; /* Primeiro nó e nó corrente são o mesmo */

    while (--Linhas) /* Cria primeira linha da matriz a partir do primeiro nó */
        MAT_adicionaLinha(MatrizCriada); /* Cada chamada cria uma linha nova na matriz.
        Já que só há um nó na matriz, cada chamada da função só cria novos nós à direita do primeiro */

    while (--Colunas) /* Cria as colunas da matriz extendendo a primeira linha */
        MAT_adicionaColuna(MatrizCriada); /* Cada chamada cria uma coluna nova na matriz. */

    return MAT_CondRetOK; /* Retorna condição de teste bem sucedido */
}

/*******************************************************
*
*	$FC Função:
*       MAT Nó corrente da Matriz se torna o nó
*       à Esquerda Superior do nó corrente atual.
*
*
*	$AE Assertivas de entrada esperadas:
*       Nó corrente aponta para o nó à direita e
*       abaixo do nó para onde se deseja ir.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Nó corrente aponta para o nó à esquerda e
*       acima do nó corrente anterior.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
********************************************************/
MAT_tpCondRet MAT_vaiParaEsquerdaSuperior(MAT_tppMatriz CabecaDaMatriz)
{
    if (CabecaDaMatriz->pNoCorr->pNoEsquerdaSuperior == NULL) /* Não existe nó nesta direção */
        return MAT_CondRetNoNaoExiste;                        /* Retorna condição de falha na movimentação do nó corrente */

    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoEsquerdaSuperior; /* Movimenta nó corrente na direção indicada */
    return MAT_CondRetOK;                                                   /* Retorna condição de teste bem sucedido */
}

/*******************************************************
*
*	$FC Função:
*       MAT Nó corrente da Matriz se torna o nó
*       Acima do nó corrente atual.
*
*
*	$AE Assertivas de entrada esperadas:
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
*       à Direita Superior do nó corrente atual.
*
*
*	$AE Assertivas de entrada esperadas:
*       Nó corrente aponta para o nó à esquerda e
*       abaixo do nó para onde se deseja ir.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Nó corrente aponta para o nó à direita e
*       acima do nó corrente anterior.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
********************************************************/
MAT_tpCondRet MAT_vaiParaDireitaSuperior(MAT_tppMatriz CabecaDaMatriz)
{
    if (CabecaDaMatriz->pNoCorr->pNoDireitaSuperior == NULL) /* Não existe nó nesta direção */
        return MAT_CondRetNoNaoExiste;                       /* Retorna condição de falha na movimentação do nó corrente */

    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoDireitaSuperior; /* Movimenta nó corrente na direção indicada */
    return MAT_CondRetOK;                                                  /* Retorna condição de teste bem sucedido */
}

/*******************************************************
*
*	$FC Função:
*       MAT Nó corrente da Matriz se torna o nó
*       à Esquerda do nó corrente atual.
*
*
*	$AE Assertivas de entrada esperadas:
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
*       à Esquerda Inferior do nó corrente atual.
*
*
*	$AE Assertivas de entrada esperadas:
*       Nó corrente aponta para o nó à direita e
*       acima do nó para onde se deseja ir.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Nó corrente aponta para o nó à esquerda e
*       abaixo do nó corrente anterior.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
********************************************************/
MAT_tpCondRet MAT_vaiParaEsquerdaInferior(MAT_tppMatriz CabecaDaMatriz)
{
    if (CabecaDaMatriz->pNoCorr->pNoEsquerdaInferior == NULL) /* Não existe nó nesta direção */
        return MAT_CondRetNoNaoExiste;                        /* Retorna condição de falha na movimentação do nó corrente */

    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoEsquerdaInferior; /* Movimenta nó corrente na direção indicada */
    return MAT_CondRetOK;                                                   /* Retorna condição de teste bem sucedido */
}

/*******************************************************
*
*	$FC Função:
*       MAT Nó corrente da Matriz se torna o nó
*       Abaixo do nó corrente atual.
*
*
*	$AE Assertivas de entrada esperadas:
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

/*******************************************************
*
*	$FC Função:
*       MAT Nó corrente da Matriz se torna o nó
*       à Direita Inferior do nó corrente atual.
*
*
*	$AE Assertivas de entrada esperadas:
*       Nó corrente aponta para o nó à esquerda e
*       acima do nó para onde se deseja ir.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Nó corrente aponta para o nó à direita e
*       abaixo do nó corrente anterior.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
********************************************************/
MAT_tpCondRet MAT_vaiParaDireitaInferior(MAT_tppMatriz CabecaDaMatriz)
{
    if (CabecaDaMatriz->pNoCorr->pNoDireitaInferior == NULL) /* Não existe nó nesta direção */
        return MAT_CondRetNoNaoExiste;                       /* Retorna condição de falha na movimentação do nó corrente */

    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoDireitaInferior; /* Movimenta nó corrente na direção indicada */
    return MAT_CondRetOK;                                                  /* Retorna condição de teste bem sucedido */
}

/***************************************************
*
*	$FC Função:
*       MAT Destruir matriz linha por linha.
*
*
*	$AE Assertivas de entrada esperadas:
*       Primeiro nó != NULL
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
*		CabecaDaMatriz != NULL.
*		Valem as assertivas estruturais da matriz
*       com cabeça.
*
*
*	$AS Assertivas de saída esperadas:
*       Elemento != NULL.
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
