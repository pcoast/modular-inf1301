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
*
***************************************************************************/

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

    void *Elemento;
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

    void (*destruirElemento) (void *elemento);
    /* Ponteiro para a função de destrução de um elemento */
} tpMatriz;

/***** Protótipos das funções encapsuladas no módulo *****/

   static tpNoMatriz *MAT_criaNo(void);

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

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

    NoMatrizCriado->Elemento = NULL;
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
MAT_tpCondRet MAT_cria(char LinhasEColunas, void (*destruirElemento)(void *elemento), MAT_tppMatriz *MatrizCriada)
{
	int i;
	int nNos = LinhasEColunas * LinhasEColunas;
	tpNoMatriz **VetorDeNosDaMatriz = (tpNoMatriz **) malloc(sizeof(tpNoMatriz *) * nNos); /* Cria vetor nxn para armazenar os nós da matriz e interconectá-los posteriormente */

	for (i = 0; i < nNos; i++)
	{
		VetorDeNosDaMatriz[i] = MAT_criaNo();

		if (VetorDeNosDaMatriz[i] == NULL) /* Memória não pôde ser alocada */
			return MAT_CondRetFaltouMemoria; /* Retorna condição de insufficiência de memória */
	}

	for (i = 0; i < nNos; i++) /* Interconecta os nós da matriz dentro do vetor nxn */
	{
		if (i % LinhasEColunas != 0) { /* Checa se o nó não está "encostado" no canto esquerdo da matriz */
			VetorDeNosDaMatriz[i]->pNoEsquerda = VetorDeNosDaMatriz[i - 1];
			
			if (i + LinhasEColunas < nNos) {
				VetorDeNosDaMatriz[i]->pNoEsquerdaInferior = VetorDeNosDaMatriz[i + LinhasEColunas - 1];
				VetorDeNosDaMatriz[i]->pNoBaixo = VetorDeNosDaMatriz[i + LinhasEColunas];
			}
			if (i - LinhasEColunas > 0) {
				VetorDeNosDaMatriz[i]->pNoEsquerdaSuperior = VetorDeNosDaMatriz[i - LinhasEColunas - 1];
				VetorDeNosDaMatriz[i]->pNoCima = VetorDeNosDaMatriz[i - LinhasEColunas];
			}
		}		

		if (i % LinhasEColunas != (LinhasEColunas -1)) { /* Checa se o nó não está "encostado" no canto direito da matriz */
			VetorDeNosDaMatriz[i]->pNoDireita = VetorDeNosDaMatriz[i + 1];

			if (i + LinhasEColunas < nNos) {
				VetorDeNosDaMatriz[i]->pNoDireitaInferior = VetorDeNosDaMatriz[i + LinhasEColunas + 1];
				VetorDeNosDaMatriz[i]->pNoBaixo = VetorDeNosDaMatriz[i + LinhasEColunas];
			}
			if (i - LinhasEColunas > 0) {
				VetorDeNosDaMatriz[i]->pNoDireitaSuperior = VetorDeNosDaMatriz[i - LinhasEColunas + 1];
				VetorDeNosDaMatriz[i]->pNoCima = VetorDeNosDaMatriz[i - LinhasEColunas];
			}
		}
		
	}
	(*MatrizCriada) = (MAT_tppMatriz) malloc(sizeof(tpMatriz));

	if ((*MatrizCriada) == NULL)
		return MAT_CondRetFaltouMemoria;

    (*MatrizCriada)->pNoPrimeiro = VetorDeNosDaMatriz[0]; /* Primeiro nó da matriz é o nó mais encima e mais à esquerda possível */
	(*MatrizCriada)->pNoCorr = (*MatrizCriada)->pNoPrimeiro; /* Nó corrente é inicialmente o mesmo que o primeiro nó */
    (*MatrizCriada)->destruirElemento = destruirElemento;

	free(VetorDeNosDaMatriz);

    return MAT_CondRetOK; /* Retorna condição de teste bem sucedido */

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
MAT_tpCondRet MAT_vaiParaEsquerdaSuperior(MAT_tppMatriz CabecaDaMatriz)
{
    if (CabecaDaMatriz->pNoCorr->pNoEsquerdaSuperior == NULL) /* Não existe nó nesta direção */
        return MAT_CondRetNoNaoExiste; /* Retorna condição de falha na movimentação do nó corrente */

    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoEsquerdaSuperior; /* Movimenta nó corrente na direção indicada */
    return MAT_CondRetOK; /* Retorna condição de teste bem sucedido */

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
MAT_tpCondRet MAT_vaiParaCima(MAT_tppMatriz CabecaDaMatriz)
{
    if (CabecaDaMatriz->pNoCorr->pNoCima == NULL) /* Não existe nó nesta direção */
        return MAT_CondRetNoNaoExiste; /* Retorna condição de falha na movimentação do nó corrente */

    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoCima; /* Movimenta nó corrente na direção indicada */
    return MAT_CondRetOK; /* Retorna condição de teste bem sucedido */

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
MAT_tpCondRet MAT_vaiParaDireitaSuperior(MAT_tppMatriz CabecaDaMatriz)
{
    if (CabecaDaMatriz->pNoCorr->pNoDireitaSuperior == NULL) /* Não existe nó nesta direção */
        return MAT_CondRetNoNaoExiste; /* Retorna condição de falha na movimentação do nó corrente */

    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoDireitaSuperior; /* Movimenta nó corrente na direção indicada */
    return MAT_CondRetOK; /* Retorna condição de teste bem sucedido */

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
MAT_tpCondRet MAT_vaiParaEsquerda(MAT_tppMatriz CabecaDaMatriz)
{
    if (CabecaDaMatriz->pNoCorr->pNoEsquerda == NULL) /* Não existe nó nesta direção */
        return MAT_CondRetNoNaoExiste; /* Retorna condição de falha na movimentação do nó corrente */

    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoEsquerda; /* Movimenta nó corrente na direção indicada */
    return MAT_CondRetOK; /* Retorna condição de teste bem sucedido */

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
MAT_tpCondRet MAT_vaiParaDireita(MAT_tppMatriz CabecaDaMatriz)
{
    if (CabecaDaMatriz->pNoCorr->pNoDireita == NULL) /* Não existe nó nesta direção */
        return MAT_CondRetNoNaoExiste; /* Retorna condição de falha na movimentação do nó corrente */

    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoDireita; /* Movimenta nó corrente na direção indicada */
    return MAT_CondRetOK; /* Retorna condição de teste bem sucedido */

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
MAT_tpCondRet MAT_vaiParaEsquerdaInferior(MAT_tppMatriz CabecaDaMatriz)
{
    if (CabecaDaMatriz->pNoCorr->pNoEsquerdaInferior == NULL) /* Não existe nó nesta direção */
        return MAT_CondRetNoNaoExiste; /* Retorna condição de falha na movimentação do nó corrente */

    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoEsquerdaInferior; /* Movimenta nó corrente na direção indicada */
    return MAT_CondRetOK; /* Retorna condição de teste bem sucedido */

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
MAT_tpCondRet MAT_vaiParaBaixo(MAT_tppMatriz CabecaDaMatriz)
{
    if (CabecaDaMatriz->pNoCorr->pNoBaixo == NULL) /* Não existe nó nesta direção */
        return MAT_CondRetNoNaoExiste; /* Retorna condição de falha na movimentação do nó corrente */

    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoBaixo; /* Movimenta nó corrente na direção indicada */
    return MAT_CondRetOK; /* Retorna condição de teste bem sucedido */

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
MAT_tpCondRet MAT_vaiParaDireitaInferior(MAT_tppMatriz CabecaDaMatriz)
{
    if (CabecaDaMatriz->pNoCorr->pNoDireitaInferior == NULL) /* Não existe nó nesta direção */
        return MAT_CondRetNoNaoExiste; /* Retorna condição de falha na movimentação do nó corrente */

    CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoDireitaInferior; /* Movimenta nó corrente na direção indicada */
    return MAT_CondRetOK; /* Retorna condição de teste bem sucedido */

}

/***************************************************
*
*	$FC Função: MAT Destruir matriz linha por linha.
*
*	$EAE Assertivas de entrada esperadas
*		CabecaDaMatriz != NULL
*
****************************************************/
void MAT_destroi(MAT_tppMatriz CabecaDaMatriz)
{
    tpNoMatriz *PrimeiroNoDaLinhaDeBaixo;

    do
    {
        PrimeiroNoDaLinhaDeBaixo = CabecaDaMatriz->pNoPrimeiro->pNoBaixo; /* Guarda primeiro nó da linha de baixo para deletar a linha seguinte */
        CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoPrimeiro->pNoDireita; /* Nó corrente é o nó à direita do primeiro nó da matriz */
        do
        {
            CabecaDaMatriz->destruirElemento(CabecaDaMatriz->pNoPrimeiro->Elemento); /* Libera o ponteiro do elemento */
            free(CabecaDaMatriz->pNoPrimeiro); /* Libera primeiro nó da matriz */
            if (CabecaDaMatriz->pNoCorr == NULL) /* Liberou o último nó desta linha da matriz */
                break; /* Segue para próxima linha da matriz */
            CabecaDaMatriz->pNoPrimeiro = CabecaDaMatriz->pNoCorr; /* Primeiro nó da matriz é o mesmo que o corrente */
            CabecaDaMatriz->pNoCorr = CabecaDaMatriz->pNoCorr->pNoDireita; /* Nó corrente é o nó à direita do primeiro nó da matriz */
        } while (1); /* Para sempre */

        CabecaDaMatriz->pNoPrimeiro = PrimeiroNoDaLinhaDeBaixo; /* Primeiro nó da matriz se torna o primeiro nó da linha de baixo */
    } while (PrimeiroNoDaLinhaDeBaixo != NULL); /* Se a condição for quebrada, então liberou a última linha da matriz */

    free(CabecaDaMatriz); /* Libera cabeça da matriz */
}

/*******************************************************
*
*	$FC Função: MAT Inserir elemento no nó corrente da 
*       matriz
*
*	$EAE Assertivas de entrada esperadas:
*		Lista não nula
*
********************************************************/
MAT_tpCondRet MAT_inserir(MAT_tppMatriz CabecaDaMatriz, void *elemento)
{
    if (CabecaDaMatriz->pNoCorr->Elemento != NULL) /* Nó corrente já possui lista */
        return MAT_CondRetNoOcupado; /* Retorna condição de falha na incorporação da lista no nó corrente */

    CabecaDaMatriz->pNoCorr->Elemento = elemento; /* Incopora ponteiro da lista passado por referência no nó corrente */

    return MAT_CondRetOK; /* Retorna condição de teste bem sucedido */

}

/*******************************************************
*
*	$FC Função: MAT Obtém elemento do nó corrente da matriz
*
*	$EAE Assertivas de entrada esperadas:
*		Lista não nula
*
********************************************************/
MAT_tpCondRet MAT_obterElemento(MAT_tppMatriz CabecaDaMatriz, void **elemento)
{
    if (CabecaDaMatriz->pNoCorr->Elemento == NULL) /* Nó corrente não possui lista */
        return MAT_CondRetNoVazio; /* Retorna condição de falha na obtenção da lista incorporada no nó corrente */

    *elemento = CabecaDaMatriz->pNoCorr->Elemento; /* Passa, por referência, o ponteiro da lista incorporado no nó corrente */

    return MAT_CondRetOK; /* Retorna condição de teste bem sucedido */

}


