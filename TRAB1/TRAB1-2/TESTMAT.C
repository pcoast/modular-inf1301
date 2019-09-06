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
*
***************************************************************************/

#include    <string.h>
#include    <stdio.h>
#include    <stdlib.h>

#include    "TST_Espc.h"
#include    "Generico.h"
#include    "LerParm.h"

#include    "Lista.h"
#include    "MATRIZ.h"

static const char RESET_MATRIZ_CMD[] = "=resetteste";
static const char CRIAR_MATRIZ_CMD[] = "=criarlista";
static const char DESTRUIR_MATRIZ_CMD[] = "=destruirlista";
static const char ESVAZIAR_MATRIZ_CMD[] = "=esvaziarlista";
static const char INS_ELEM_CMD[] = "=inselemapos";
static const char OBTER_VALOR_CMD[] = "=obtervalorelem";
static const char EXC_ELEM_CMD[] = "=excluirelem";
static const char IR_INICIO_CMD[] = "=irinicio";
static const char IR_FIM_CMD[] = "=irfinal";
static const char IR_N_CMD[] = "=irnorte";
static const char IR_S_CMD[] = "=irsul";
static const char IR_E_CMD[] = "=irleste";
static const char IR_W_CMD[] = "=iroeste";
static const char IR_NW_CMD[] = "=irne";
static const char IR_NE_CMD[] = "=irno";
static const char IR_SW_CMD[] = "=irso";
static const char IR_SE_CMD[] = "=irse";