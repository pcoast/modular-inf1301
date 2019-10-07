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
#include "LABIRINTO.H"

static const char RESET_MATRIZ_CMD[] = "=resetteste";
static const char CRIAR_LAB_CMD[] = "=criarlab";
static const char IMPRIMIR_LAB_CMD[] = "=imprimirlab";
static const char DESTRUIR_LAB_CMD[] = "=destruirmatriz";

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

	MAT_tpCondRet condRetObtida = MAT_CondRetOK;
	MAT_tpCondRet condRetEsperada = MAT_CondRetFaltouMemoria;

	char valorEsperado = '?';
	char valorObtido = '!';
	char valorDado = '\0';

	int intValorDado = 0;
	int outroIntValorDado = 0;
	int  numLidos = -1;

	TST_tpCondRet ret;

	return TST_CondRetNaoConhec;
}