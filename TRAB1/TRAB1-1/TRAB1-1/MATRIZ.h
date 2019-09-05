/***********************************************************************
*
*  $TC Tipo de dados: MAT Condicoes de retorno
*
*
***********************************************************************/

typedef enum
{

    MAT_CondRetOK = 0,
    /* Executou correto */

    MAT_CondRetNaoCriouRaiz = 1,
    /* N�o criou n� raiz */

    MAT_CondRetErroEstrutura = 2,
    /* Estrutura da �rvore est� errada */

    MAT_CondRetNaoEhFolha = 3,
    /* N�o � folha relativa � dire��o de inser��o desejada */

    MAT_CondRetArvoreNaoExiste = 4,
    /* �rvore n�o existe */

    MAT_CondRetArvoreVazia = 5,
    /* �rvore est� vazia */

    MAT_CondRetNohEhRaiz = 6,
    /* N� corrente � raiz */

    MAT_CondRetNaoPossuiFilho = 7,
    /* N� corrente n�o possui filho na dire��o desejada */

    MAT_CondRetFaltouMemoria = 8
    /* Faltou mem�ria ao alocar dados */

} MAT_tpCondRet;

/***************************************************
*
*	$FC Função: MAT Criar um nó da matriz
*
****************************************************/
tpNoMatriz *MAT_criaNo(void);

/***************************************************
*
*	$FC Função: MAT Criar a estrutura da matriz,
*       criando os nós com a MAT_criaNo() e os
*       interconectando.
*
****************************************************/
MAT_tpCondRet MAT_cria(char LinhasEColunas, tpMatriz *MatrizCriada);

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       à Esquerda Superior do nó corrente atual.
*
*	$EAE Assertivas de entrada esperadas
*		CabecaDaMatriz != NULL
*
********************************************************/
MAT_tpCondRet MAT_vaiParaEsquerdaSuperior(tpMatriz *CabecaDaMatriz);

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       Acima do nó corrente atual.
*
*	$EAE Assertivas de entrada esperadas
*		CabecaDaMatriz != NULL
*
********************************************************/
MAT_tpCondRet MAT_vaiParaCima(tpMatriz *CabecaDaMatriz);

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       à Direita Superior do nó corrente atual.
*
*	$EAE Assertivas de entrada esperadas
*		CabecaDaMatriz != NULL
*
********************************************************/
MAT_tpCondRet MAT_vaiParaDireitaSuperior(tpMatriz *CabecaDaMatriz);

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       à Esquerda do nó corrente atual.
*
*	$EAE Assertivas de entrada esperadas
*		CabecaDaMatriz != NULL
*
********************************************************/
MAT_tpCondRet MAT_vaiParaEsquerda(tpMatriz *CabecaDaMatriz);

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       à Direita do nó corrente atual.
*
*	$EAE Assertivas de entrada esperadas
*		CabecaDaMatriz != NULL
*
********************************************************/
MAT_tpCondRet MAT_vaiParaDireita(tpMatriz *CabecaDaMatriz);

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       à Esquerda Inferior do nó corrente atual.
*
*	$EAE Assertivas de entrada esperadas
*		CabecaDaMatriz != NULL
*
********************************************************/
MAT_tpCondRet MAT_vaiParaEsquerdaInferior(tpMatriz *CabecaDaMatriz);

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       Abaixo do nó corrente atual.
*
*	$EAE Assertivas de entrada esperadas
*		CabecaDaMatriz != NULL
*
********************************************************/
MAT_tpCondRet MAT_vaiParaBaixo(tpMatriz *CabecaDaMatriz);

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       à Direita Inferior do nó corrente atual.
*
*	$EAE Assertivas de entrada esperadas
*		CabecaDaMatriz != NULL
*
********************************************************/
MAT_tpCondRet MAT_vaiParaDireitaInferior(tpMatriz *CabecaDaMatriz);

/***************************************************
*
*	$FC Função: MAT Destruir matriz linha por linha.
*
*	$EAE Assertivas de entrada esperadas
*		CabecaDaMatriz != NULL
*
****************************************************/
MAT_tpCondRet MAT_destroe(tpMatriz *CabecaDaMatriz);

/*******************************************************
*
*	$FC Função: MAT Inserir lista no nó atual da matriz
*
*	$EAE Assertivas de entrada esperadas:
*		Lista não nula
*
********************************************************/
tpNoMatriz *MAT_fill(void*);

