/***************************************************************************
*  $MCI Módulo de implementação: TMAT Teste da matriz de listas
*
*  Arquivo gerado:              TESTMAT.C
*  Letras identificadoras:      TMAT
*
*  Projeto: INF 1301
*  Departamento:  DI/PUC-Rio
*  Autores: pc - Pedro Costa
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*     1       pc    06/set/2019 Início do desenvolvimento
*	  2		  pc	07/set/2019	Implementa a funcao EfetuarComando
*
***************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "TST_ESPC.H"
#include "GENERICO.H"
#include "LERPARM.H"

#include "LISTA.H"
#include "MATRIZ.h"

#define TAM_VETOR 10

int cursorVetor = 0;
MAT_tppMatriz matrizesCriadas[] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

static const char RESET_MATRIZ_CMD[] = "=resetteste";
static const char CRIAR_MATRIZ_CMD[] = "=criarmatriz";
static const char SELECIONAR_MATRIZ[] = "=selectmatriz";
static const char DESTRUIR_MATRIZ_CMD[] = "=destruirmatriz";
static const char ESVAZIAR_MATRIZ_CMD[] = "=esvaziarmatriz";
static const char INS_ELEM_CMD[] = "=inserirelem";
static const char OBTER_VALOR_CMD[] = "=obterelem";
static const char EXC_ELEM_CMD[] = "=excluirelem";
static const char IR_CIMA_CMD[] = "=ircima";
static const char IR_BAIXO_CMD[] = "=irbaixo";
static const char IR_DIREITA_CMD[] = "=irdireita";
static const char IR_ESQUERDA_CMD[] = "=iresquerda";
static const char IR_ESQUERDASUP_CMD[] = "=iresqierdasup";
static const char IR_DIREITASUP_CMD[] = "=irdireitasup";
static const char IR_ESQUERDAINF_CMD[] = "=iresquerdainf";
static const char IR_DIREITAINF_CMD[] = "=irdireitainf";

/*
	Interface de Testes do Módulo de Matrizes com Listas.

	uso: comando <argumentos> <condiçãoDeRetornoEsperada>

	Comandos:
	=resetteste - limpa o vetor de matrizes
	=selectmatriz <i> - seleciona a matriz com qual se quer trabalhar
		Essas duas primeiras não esperam condição de retorno.
	
	=criarmatriz <i> - cria uma nova matriz i x i
	=destruirmatriz - destrói a matriz atual
	=esvaziarmatriz - esvazia a matriz atual
	=inserirelem <c> - insere o elemento <c> numa lista que é inserida na matriz atual
	=obterelem <c> - checa se o valor obtido da matriz atual é <c>
	=excluirelem - exclui o elemento do nó corrente
	=ir<direção> - move o "cursor" para o nó na direção 
		(ex.: ircima, irbaixo, irdireita, irdireitasup, iresquerdainf, ...)
*/

TST_tpCondRet TST_EfetuarComando(char * ComandoTeste)
{
	int i;

	MAT_tpCondRet condRetObtida = MAT_CondRetOK;
	MAT_tpCondRet condRetEsperada = MAT_CondRetFaltouMemoria;

	char valorEsperado = '?';
	char valorObtido = '!';
	char valorDado = '\0';

	int intValorDado = 0;
	int  numLidos = -1;

	TST_tpCondRet ret;

	if (strcmp(ComandoTeste, RESET_MATRIZ_CMD) == 0) {
		for (i = 0; i < TAM_VETOR; i++) {
			MAT_destroi(matrizesCriadas[i]);
			matrizesCriadas[i] = NULL;
		}
		
		return TST_CondRetOK;
	}
	else if (strcmp(ComandoTeste, CRIAR_MATRIZ_CMD) == 0) {
		numLidos = LER_LerParametros("ii", &intValorDado, &condRetEsperada);

		if (numLidos != 2)
			return TST_CondRetParm;

		condRetObtida = MAT_cria(intValorDado, destruirLista,&matrizesCriadas[cursorVetor]);

		return TST_CompararInt(condRetEsperada, condRetObtida, "Condicao de retorno errada ao criar matriz.");
	}
	else if (strcmp(ComandoTeste, SELECIONAR_MATRIZ) == 0) {
		numLidos = LER_LerParametros("i", &intValorDado);

		if (numLidos != 1 || intValorDado >= TAM_VETOR || intValorDado < 0)
			return TST_CondRetParm;

		cursorVetor = intValorDado;

		return TST_CondRetOK;
	}
	else if (strcmp(ComandoTeste, DESTRUIR_MATRIZ_CMD) == 0) {
		MAT_destroi(matrizesCriadas[cursorVetor]);
		matrizesCriadas[cursorVetor] = NULL;

		return TST_CondRetOK;
	}
	else if (strcmp(ComandoTeste, ESVAZIAR_MATRIZ_CMD) == 0) {
		/*TODO*/
	}
	else if (strcmp(ComandoTeste, INS_ELEM_CMD) == 0) {
		LIS_tppLista lista = LIS_CriarLista(free);
		LIS_tpCondRet condRetLista;
		char *elemento = (char *) malloc(sizeof(char));

		numLidos = LER_LerParametros("ci", elemento, &condRetEsperada);

		if (numLidos != 2)
			return TST_CondRetParm;
		condRetLista = LIS_InserirElementoApos(lista, elemento);
		condRetObtida = MAT_inserir(matrizesCriadas[cursorVetor], lista);

		ret = TST_CompararInt(condRetLista, LIS_CondRetOK, "Retorno errado ao criar lista.");

		if (ret != TST_CondRetOK)
			return ret;

		return TST_CompararInt(condRetEsperada, condRetObtida, "Retorno errado ao inserir elemento");
	}
	else if (strcmp(ComandoTeste, OBTER_VALOR_CMD) == 0) {
		LIS_tppLista elemento = NULL;
		char *valor;
		numLidos = LER_LerParametros("ci", &valorDado, &condRetEsperada);

		if (numLidos != 2)
			return TST_CondRetParm;

		condRetObtida = MAT_obterElemento(matrizesCriadas[cursorVetor], &elemento);

		if (elemento != NULL) {
			valor = LIS_ObterValor(elemento);
			ret = TST_CompararChar(valorDado, *valor, "Valor obtido não bate com o esperado");

			if (ret != TST_CondRetOK)
				return ret;
		}

		return TST_CompararInt(condRetEsperada, condRetObtida, "Retorno errado ao obter elemento.");
	}
	else if (strcmp(ComandoTeste, EXC_ELEM_CMD) == 0) {
		/*TODO*/
	}
	else if (strcmp(ComandoTeste, IR_CIMA_CMD) == 0) {
		numLidos = LER_LerParametros("i", &condRetEsperada);

		if (numLidos != 1)
			return TST_CondRetParm;

		condRetObtida = MAT_vaiParaCima(matrizesCriadas[cursorVetor]);

		return TST_CompararInt(condRetEsperada, condRetObtida, "Retorno errado ao ir para cima.");
	}
	else if (strcmp(ComandoTeste, IR_BAIXO_CMD) == 0) {
		numLidos = LER_LerParametros("i", &condRetEsperada);

		if (numLidos != 1)
			return TST_CondRetParm;

		condRetObtida = MAT_vaiParaBaixo(matrizesCriadas[cursorVetor]);

		return TST_CompararInt(condRetEsperada, condRetObtida, "Retorno errado ao ir para baixo.");
	}
	else if (strcmp(ComandoTeste, IR_DIREITA_CMD) == 0) {
		numLidos = LER_LerParametros("i", &condRetEsperada);

		if (numLidos != 1)
			return TST_CondRetParm;

		condRetObtida = MAT_vaiParaDireita(matrizesCriadas[cursorVetor]);

		return TST_CompararInt(condRetEsperada, condRetObtida, "Retorno errado ao ir para a direita.");
	}
	else if (strcmp(ComandoTeste, IR_ESQUERDA_CMD) == 0) {
		numLidos = LER_LerParametros("i", &condRetEsperada);

		if (numLidos != 1)
			return TST_CondRetParm;

		condRetObtida = MAT_vaiParaEsquerda(matrizesCriadas[cursorVetor]);

		return TST_CompararInt(condRetEsperada, condRetObtida, "Retorno errado ao ir para a esquerda.");
	}
	else if (strcmp(ComandoTeste, IR_ESQUERDASUP_CMD) == 0) {
		numLidos = LER_LerParametros("i", &condRetEsperada);

		if (numLidos != 1)
			return TST_CondRetParm;

		condRetObtida = MAT_vaiParaEsquerdaSuperior(matrizesCriadas[cursorVetor]);

		return TST_CompararInt(condRetEsperada, condRetObtida, "Retorno errado ao ir para a esquerda superior.");
	}
	else if (strcmp(ComandoTeste, IR_DIREITASUP_CMD) == 0) {
		numLidos = LER_LerParametros("i", &condRetEsperada);

		if (numLidos != 1)
			return TST_CondRetParm;

		condRetObtida = MAT_vaiParaDireitaSuperior(matrizesCriadas[cursorVetor]);

		return TST_CompararInt(condRetEsperada, condRetObtida, "Retorno errado ao ir para a direita superior.");
	}
	else if (strcmp(ComandoTeste, IR_ESQUERDAINF_CMD) == 0) {
		numLidos = LER_LerParametros("i", &condRetEsperada);

		if (numLidos != 1)
			return TST_CondRetParm;

		condRetObtida = MAT_vaiParaEsquerdaInferior(matrizesCriadas[cursorVetor]);

		return TST_CompararInt(condRetEsperada, condRetObtida, "Retorno errado ao ir para a esquerda inferior.");
	}
	else if (strcmp(ComandoTeste, IR_DIREITAINF_CMD) == 0) {
		numLidos = LER_LerParametros("i", &condRetEsperada);

		if (numLidos != 1)
			return TST_CondRetParm;

		condRetObtida = MAT_vaiParaDireitaInferior(matrizesCriadas[cursorVetor]);

		return TST_CompararInt(condRetEsperada, condRetObtida, "Retorno errado ao ir para a direita inferior.");
	}

	return TST_CondRetNaoConhec;
}

void destruirLista (void *lista) {
	LIS_DestruirLista((LIS_tppLista)lista);
}

void excluiElemento (MAT_tppMatriz cabecaDaMatriz) {
	LIS_tppLista elemento;

	MAT_tpCondRet condRetObter = MAT_obterElemento(cabecaDaMatriz, &elemento);
	MAT_tpCondRet condRetInserir = MAT_inserir(cabecaDaMatriz, NULL);

	if (condRetObter == MAT_CondRetOK)
		LIS_DestruirLista(elemento);
	
	/* TOOD */
}