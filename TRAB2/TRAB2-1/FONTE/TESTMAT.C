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
*	  3		  pc	06/out/2019	Atualiza implementação para nova matriz.
*
***************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "TST_ESPC.H"
#include "GENERICO.H"
#include "LERPARM.H"

#include "MATRIZ.h"

#define TAM_VETOR 10

int cursorVetor = 0;
MAT_tppMatriz matrizesCriadas[] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

static const char RESET_MATRIZ_CMD[] = "=resetteste";
static const char CRIAR_MATRIZ_CMD[] = "=criarmatriz";
static const char SELECIONAR_MATRIZ[] = "=selectmatriz";
static const char DESTRUIR_MATRIZ_CMD[] = "=destruirmatriz";
static const char INS_ELEM_CMD[] = "=inserirelem";
static const char OBTER_VALOR_CMD[] = "=obterelem";
static const char IR_CIMA_CMD[] = "=ircima";
static const char IR_BAIXO_CMD[] = "=irbaixo";
static const char IR_DIREITA_CMD[] = "=irdireita";
static const char IR_ESQUERDA_CMD[] = "=iresquerda";
/*
	Interface de Testes do Módulo de Matrizes com Listas.

	uso: comando <argumentos> <condiçãoDeRetornoEsperada>

	Comandos:
	=resetteste - limpa o vetor de matrizes
	=selectmatriz <i> - seleciona a matriz com qual se quer trabalhar
		Essas duas primeiras não esperam condição de retorno.
	
	=criarmatriz <i> <j> - cria uma nova matriz i x j
	=destruirmatriz - destrói a matriz atual
	=esvaziarmatriz - esvazia a matriz atual
	=inserirelem <c> - insere o elemento <c> numa lista que é inserida na matriz atual
	=obterelem <c> - checa se o valor obtido da matriz atual é <c>
	=ir<direção> - move o "cursor" para o nó na direção 
		(ex.: ircima, irbaixo, irdireita, iresquerda)
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
	int outroIntValorDado = 0;
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
		numLidos = LER_LerParametros("iii", &intValorDado, &outroIntValorDado, &condRetEsperada);

		if (numLidos != 3)
			return TST_CondRetParm;

		condRetObtida = MAT_cria(intValorDado, outroIntValorDado, free,&matrizesCriadas[cursorVetor]);

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
	else if (strcmp(ComandoTeste, INS_ELEM_CMD) == 0) {
		char *elemento = (char *) malloc(sizeof(char));

		numLidos = LER_LerParametros("ci", elemento, &condRetEsperada);

		if (numLidos != 2)
			return TST_CondRetParm;

		condRetObtida = MAT_inserir(matrizesCriadas[cursorVetor], elemento);

		return TST_CompararInt(condRetEsperada, condRetObtida, "Retorno errado ao inserir elemento");
	}
	else if (strcmp(ComandoTeste, OBTER_VALOR_CMD) == 0) {
		char** valor = malloc(sizeof(char*));
		numLidos = LER_LerParametros("ci", &valorDado, &condRetEsperada);

		if (numLidos != 2)
			return TST_CondRetParm;

		condRetObtida = MAT_obterElemento(matrizesCriadas[cursorVetor], valor);

		if (valor != NULL) {
			ret = TST_CompararChar(valorDado, **valor, "Valor obtido não bate com o esperado");

			if (ret != TST_CondRetOK)
				return ret;
		}

		return TST_CompararInt(condRetEsperada, condRetObtida, "Retorno errado ao obter elemento.");
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

	return TST_CondRetNaoConhec;
}