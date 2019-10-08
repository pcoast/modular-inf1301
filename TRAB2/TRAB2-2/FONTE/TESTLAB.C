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

#include "LABIRINTO.H"

LAB_tppLabirinto labirinto;

static const char RESET_LAB_CMD[] = "=resetteste";
static const char CRIAR_LAB_CMD[] = "=criarlab";
static const char IMPRIMIR_LAB_CMD[] = "=imprimirlab";

/*
	Interface de Testes do Módulo de Matrizes com Listas.

	uso: comando <argumentos> <condiçãoDeRetornoEsperada>

	Comandos:
	=resetteste - limpa o vetor de matrizes
	=criarlab - cria o labirinto
	=imprimirlab - imprime o labirinto
	=destruirlab - destroi o labirinto
*/

TST_tpCondRet TST_EfetuarComando(char * ComandoTeste)
{
	int i;

	LAB_tpCondRet condRetObtida = LAB_CondRetOK;
	LAB_tpCondRet condRetEsperada = LAB_CondRetFaltouMemoriaNaMatriz;


	char* string = (char*)malloc(sizeof(char) * 12);
	char valorEsperado = '?';
	char valorObtido = '!';
	char valorDado = '\0';

	int intValorDado = 0;
	int outroIntValorDado = 0;
	int  numLidos = -1;

	TST_tpCondRet ret;

	if (strcmp(ComandoTeste, RESET_LAB_CMD) == 0) {

		LAB_destroiLabirinto(labirinto);
		labirinto = NULL;
		return TST_CondRetOK;
	}
	else if (strcmp(ComandoTeste, CRIAR_LAB_CMD) == 0) {
		numLidos = LER_LerParametros("si", string, &condRetEsperada);

		if (numLidos != 2)
			return TST_CondRetParm;

		condRetObtida = LAB_criaLabirinto(string, &labirinto);

		return TST_CompararInt(condRetEsperada, condRetObtida, "Condicao de retorno errada ao criar labirinto.");
	}
	else if (strcmp(ComandoTeste, IMPRIMIR_LAB_CMD) == 0)
	{
		numLidos = LER_LerParametros("i", &condRetEsperada);

		if (numLidos != 1)
			return TST_CondRetParm;

		condRetObtida = LAB_imprimeLabirinto(labirinto);

		return TST_CompararInt(condRetEsperada, condRetObtida, "Condicao de retorno errada ao imprimir labirinto.");
	}

	return TST_CondRetNaoConhec;
}