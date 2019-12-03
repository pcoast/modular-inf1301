/***************************************************************************
*  $MCI M�dulo de implementa��o: CNT  Contadores de passagem
*
*  Arquivo gerado:              CONTA.c
*  Letras identificadoras:      CNT
*
*  Nome da base de software:    Arcabou�o para a automa��o de testes de programas redigidos em C
*  Arquivo da base de software: C:\AUTOTEST\PROJETOS\ARCABOUC.BSW
*
*  Projeto: INF 1301 / 1628 Automatiza��o dos testes de m�dulos C
*  Gestor:  LES/DI/PUC-Rio
*  Autores: avs
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*     4       avs   01/fev/2006 criar linguagem script simb�lica
*     3       avs   08/dez/2004 uniformiza��o dos exemplos
*     2       avs   07/jul/2003 unifica��o de todos os m�dulos em um s� projeto
*     1       avs   16/abr/2003 in�cio desenvolvimento
*
***************************************************************************/

#include <string.h>
#include <stdio.h>
#include <time.h>
#include <malloc.h>

#include "Generico.h"
#include "TabSimb.h"

#ifdef _DEBUG
#include "CEspDin.h"
#endif

#define CONTA_OWN
#include "CONTA.H"
#undef CONTA_OWN

#define ARQUIVO_ACUMULADOR 1
#define ARQUIVO_DEFINICAO 2

#define FALSE 0
#define TRUE 1

#define DIM_NOME_CONTADOR 100
#define STR_DIM_NOME_CONTADOR "98"
#define DIM_NOME_ARQUIVO 120
#define DIM_BUFFER 150
#define DIM_TABELA 317

#define TRIM_CHAR " \n\r\t\x1A"

#define ERRO_CONTADOR "cc>"

#define EXT_ACUMULADOR "count"
#define EXT_DEFINICAO "count"

#define COMENTARIO "//"
#define OP_ATRIBUICAO '='

const char ContadorNaoInicializado[] = "M�dulo contadores ainda n�o foi inicializado.";

/***********************************************************************
*
*  $TC Tipo de dados: CNT Descritor de contador
*
*
*  $ED Descri��o do tipo
*     Cada contador � identificado por um nome.
*     O acesso ao nome se dar� atrab�s de uma tabela de s�mbolos.
*     Al�m disso ser� mantida uma lista linear de contadores, ordenada
*     em ordem crescente pelo nome do contador.
*
***********************************************************************/

typedef struct tgContador
{

    struct tgContador *pProxContador;
    /* Pr�ximo contador na lista ordenada de contadores */

    long Contagem;
    /* Valor de contagem do contador
               *
               *$ED Descri��o
               *
               *   Conta o n�mero de vezes que a fun��o CNT_Contar
               *   foi ativada com rela��o ao contador */

    char NomeContador[DIM_NOME_CONTADOR];
    /* Nome do contador, string C padr�o */

    int Fonte;
    /* Fonte do nome
               *
               *$ED Descri��o
               *   O atributo cont�m "ARQUIVO_ACUMULADOR" se o contador
               *   tiver sido lido de um arquivo acumulador ou
               *   "ARQUIVO_DEFINICAO" se tiver sido lido de um arquivo
               *   de definicao. Se ao ler de um arquivo de definicao
               *   o contador estiver marcado "ARQUIVO_ACUMULADOR",
               *   o indicador � mudado para "ARQUIVO_DEFINICAO".
               *   Se j� era "ARQUIVO_DEFINICAO", � emitida uma mensagem
               *   de erro de duplicidade de defini��o. */

} tpContador;

/*****  Dados encapsulados no m�dulo  *****/

static int Inicializado = FALSE;
/* Controle de inicializa��o de contadores
               *
               *$ED Descri��o
               *   Controla se o sistema de contagem foi ou n�o iniciado */

static int EfetuaContagem = FALSE;
/* Controle de contagem
               *
               *$ED Descri��o
               *   Contagem � realizada somente se EfetuaContagem == TRUE */

static TBS_tppTabela pTabela = NULL;
/* Ponteiro para a tabela de s�mbolos em uso */

static tpContador *pContadorCorr = NULL;
/* Ponteiro para contador corrente do iterador */

static tpContador *pOrgListaContadores = NULL;
/* Origem da lista ordenada de contadores */

static int numContadores = 0;
/* N�mero total de contadores */

static int numTotalErros = 0;
/* N�mero total de erros encontrados at� o momento */

static long ContagemTotal = 0;
/* Contagem total
               *
               *$ED Descri��o
               *     Cont�m a somat�ria de todas as contagens */

static char NomeArquivoAcumulador[DIM_NOME_ARQUIVO];
/* Nome do arquivo acumulador de contadores */

static int numLinha = 0;
/* N�mero da linha do arquivo sendo lido */

static char BufferLeitura[DIM_BUFFER];
/* Buffer de leitura de contadores */

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

static char *ObterNomeContador(void *pDado);

static void MontarNomeArquivo(char *NomeArquivo,
                              char *NomeExtensao);

static int LerLinha(FILE *pArquivo);

static int LerContadores(char *NomeArquivo, int Fonte);

static void ExibirPrefixo(int numLinha);

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***************************************************************************
*
*  Fun��o: CNT  &Inicializar contadores
*  ****/

CNT_tpCondRet CNT_InicializarContadores(char *NomeArquivo)
{

    if (Inicializado == TRUE)
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), "Contadores j� foram inicializados.");
        return CNT_CondRetInicializado;
    } /* if */

    /* Inicializar todas as constantes globais do contador */

    EfetuaContagem = FALSE;
    pOrgListaContadores = NULL;
    pContadorCorr = NULL;
    numContadores = 0;
    ContagemTotal = 0;
    numLinha = -1;
    memset(BufferLeitura, '?', DIM_BUFFER);
    BufferLeitura[DIM_BUFFER - 1] = 0;

    /* Criar tabela de simbolos dos contadores */

    pTabela = TBS_CriarTabela(DIM_TABELA, ObterNomeContador, NULL);

    TST_ASSERT(pTabela != NULL);

    /* Registrar o arquivo de contagem acumulado */

    Inicializado = TRUE;

    if (NomeArquivo == NULL)
    {
        NomeArquivoAcumulador[0] = 0;
    }
    else
    {
        if (numContadores != 0)
        {
            ExibirPrefixo(-1);
            fprintf(TST_ObterArqLog(), "\nError: O arquivo acumulador deve ser lido antes de todos os outros.");
            return CNT_CondRetErro;
        }
        else
        {
            strcpy(NomeArquivoAcumulador, NomeArquivo);
            MontarNomeArquivo(NomeArquivoAcumulador, EXT_ACUMULADOR);
            if (LerContadores(NomeArquivoAcumulador, ARQUIVO_ACUMULADOR) != 0)
            {
                return CNT_CondRetErro;
            } /* if */
        }     /* if */
    }         /* if */
    return CNT_CondRetOK;

} /* Fim fun��o: CNT  &Inicializar contadores */

/***************************************************************************
*
*  Fun��o: CNT  &Terminar contadores
*  ****/

CNT_tpCondRet CNT_TerminarContadores()
{

    int numErros = 0;

    if (Inicializado == FALSE)
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), ContadorNaoInicializado);
        return CNT_CondRetErro;
    } /* if */

    /* Gravar a contagem acumulada */

    if (NomeArquivoAcumulador[0] != 0)
    {
        numErros = CNT_GravarContadores(NomeArquivoAcumulador);
    } /* if */

    /* Inicializar as vari�veis de controle e a tabela */

    TBS_DestruirTabela(pTabela);
    pTabela = NULL;

    Inicializado = FALSE;

    if (numErros != 0)
    {
        return CNT_CondRetErro;
    } /* if */

    return CNT_CondRetOK;

} /* Fim fun��o: CNT  &Terminar contadores */

/***************************************************************************
*
*  Fun��o: CNT  &Iniciar a contagem
*  ****/

void CNT_IniciarContagem()
{

    if (Inicializado == FALSE)
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), "Contadores ainda n�o foram inicializados.");
        return;
    } /* if */

    EfetuaContagem = TRUE;

} /* Fim fun��o: CNT  &Iniciar a contagem */

/***************************************************************************
*
*  Fun��o: CNT  &Parar contagem
*  ****/

void CNT_PararContagem()
{

    if (Inicializado == FALSE)
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), ContadorNaoInicializado);
        return;
    } /* if */

    EfetuaContagem = FALSE;

} /* Fim fun��o: CNT  &Parar contagem */

/***************************************************************************
*
*  Fun��o: CNT  &Registrar arquivo acumulador
*  ****/

void CNT_RegistrarAcumulador(char *NomeArquivo)
{

    if (Inicializado == FALSE)
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), ContadorNaoInicializado);
        return;
    } /* if */

    if (NomeArquivo == NULL)
    {
        NomeArquivoAcumulador[0] = 0;
    }
    else
    {
        strcpy(NomeArquivoAcumulador, NomeArquivo);
        MontarNomeArquivo(NomeArquivoAcumulador, EXT_ACUMULADOR);
    } /* if */

} /* Fim fun��o: CNT  &Registrar arquivo acumulador */

/***************************************************************************
*
*  Fun��o: CNT  &Ler arquivo de defini��o de contadores
*  ****/

int CNT_LerContadores(char *NomeArquivoDefinicao)
{

    char NomeArquivo[DIM_NOME_ARQUIVO];

    if (Inicializado == FALSE)
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), ContadorNaoInicializado);
        return 1;
    } /* if */

    strcpy(NomeArquivo, NomeArquivoDefinicao);

    MontarNomeArquivo(NomeArquivo, EXT_DEFINICAO);

    return LerContadores(NomeArquivo, ARQUIVO_DEFINICAO);

} /* Fim fun��o: CNT  &Ler arquivo de defini��o de contadores */

/***************************************************************************
*
*  Fun��o: CNT  &Gravar arquivo de contagem acumulada
*  ****/

int CNT_GravarContadores(char *NomeArquivo)
{

    FILE *pArq;

    char CharTime[25];

    time_t StartTime;
    struct tm *pTime;

    tpContador *pContador;

    /* Abrir arquivo acumulador para grava��o */

    pArq = fopen(NomeArquivo, "w");
    if (pArq == NULL)
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), "N�o abriu o arquivo de acumuladores %s",
                NomeArquivo);
        return 1;
    } /* if */

    /* Gerar arquivo acumulador */

    /* Gravar cabe�alho de arquivo de acumuladores */

    time(&StartTime);
    pTime = localtime(&StartTime);

    sprintf(CharTime, "%04i/%02i/%02i-%02i:%02i:%02i",
            pTime->tm_year + 1900, pTime->tm_mon + 1,
            pTime->tm_mday, pTime->tm_hour,
            pTime->tm_min, pTime->tm_sec);

    fprintf(pArq, "//////////////////////////////////////////////////////////////////////");
    fprintf(pArq, "\n// Contadores acumulados, gerado: %s", CharTime);
    fprintf(pArq, "\n//////////////////////////////////////////////////////////////////////");
    fprintf(pArq, "\n");

    /* Gravar todos contadores e respectivas contagens */

    pContador = pOrgListaContadores;
    while (pContador != NULL)
    {
        fprintf(pArq, "\n%s\\%c%ld", pContador->NomeContador,
                OP_ATRIBUICAO, pContador->Contagem);
        pContador = pContador->pProxContador;
    } /* while */

    /* Gravar final de arquivo acumulador */

    fprintf(pArq, "\n");
    fprintf(pArq, "\n//////////////////////////////////////////////////////////////////////");
    fprintf(pArq, "\n// Fim arquivo de contadores acumulados");
    fprintf(pArq, "\n//////////////////////////////////////////////////////////////////////");
    fprintf(pArq, "\n");

    /* Fechar arquivo acumulador */

    fclose(pArq);

    return 0;

} /* Fim fun��o: CNT  &Gravar arquivo de contagem acumulada */

/***************************************************************************
*
*  Fun��o: CNT  &Contar
*  ****/

CNT_tpCondRet CNT_Contar(char *NomeContador, int numLinha)
{

    tpContador *pContador;

#ifdef _DEBUG
    TST_ASSERT(NomeContador != NULL);
#endif

    if (Inicializado == FALSE)
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), "Contadores ainda n�o foram inicializados.");
        return CNT_CondRetNaoInicializado;
    } /* if */

    if (EfetuaContagem)
    {
        pContador = (tpContador *)TBS_ProcurarSimbolo(pTabela, NomeContador);
        if (pContador != NULL)
        {
            if (pContador->Contagem == -1)
            {
                pContador->Contagem = 1;
            }
            else if (pContador->Contagem >= 0)
            {
                pContador->Contagem++;
                ContagemTotal++;
            }
            else if (pContador->Contagem == -2)
            {
                ExibirPrefixo(numLinha);
                fprintf(TST_ObterArqLog(), "Contador \"%s\" n�o deve ser contado.",
                        NomeContador);
                return CNT_CondRetProibido;
            }
            else
            {
                ExibirPrefixo(numLinha);
                fprintf(TST_ObterArqLog(), "Contador \"%s\" cont�m valor ilegal: %d.",
                        NomeContador, pContador->Contagem);
                return CNT_CondRetNaoContador;
            } /* if */
        }
        else
        {
            ExibirPrefixo(numLinha);
            fprintf(TST_ObterArqLog(), "Contador \"%s\" n�o existe.",
                    NomeContador);
            return CNT_CondRetNaoContador;
        } /* if */
    }     /* if */

    return CNT_CondRetOK;

} /* Fim fun��o: CNT  &Contar */

/***************************************************************************
*
*  Fun��o: CNT  &Verificar contagem
*  ****/

int CNT_VerificarContagem()
{

    tpContador *pContador;
    int numErros = 0;

    if (Inicializado == FALSE)
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), ContadorNaoInicializado);
        return -1;
    } /* if */

    pContador = pOrgListaContadores;
    while (pContador != NULL)
    {
        if (pContador->Contagem == 0)
        {
            numErros++;
            ExibirPrefixo(0);
            fprintf(TST_ObterArqLog(), "Contagem em \"%s\" � zero.",
                    pContador->NomeContador);
        } /* if */
        pContador = pContador->pProxContador;
    } /* while */

    return numErros;

} /* Fim fun��o: CNT  &Verificar contagem */

/***************************************************************************
*
*  Fun��o: CNT  &Obter valor de contagem
*  ****/

long CNT_ObterContagem(char *NomeContador)
{

    tpContador *pContador;

    if (Inicializado == FALSE)
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), ContadorNaoInicializado);
        return -1;
    } /* if */

    pContador = (tpContador *)TBS_ProcurarSimbolo(pTabela, NomeContador);
    if (pContador == NULL)
    {
        return CNT_CondRetNaoContador;
    } /* if */

    return pContador->Contagem;

} /* Fim fun��o: CNT  &Obter valor de contagem */

/***************************************************************************
*
*  Fun��o: CNT  &Obter n�mero de contadores
*  ****/

int CNT_ObterNumeroContadores()
{

    if (Inicializado == FALSE)
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), ContadorNaoInicializado);
        return -1;
    } /* if */

    return numContadores;

} /* Fim fun��o: CNT  &Obter n�mero de contadores */

/***************************************************************************
*
*  Fun��o: CNT  &Obter total de contagem
*  ****/

long CNT_ObterContagemTotal()
{

    if (Inicializado == FALSE)
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), ContadorNaoInicializado);
        return -1;
    } /* if */

    return ContagemTotal;

} /* Fim fun��o: CNT  &Obter total de contagem */

/***************************************************************************
*
*  Fun��o: CNT  &Iterador: obter contagem corrente
*  ****/

long CNT_ObterContagemCorrente()
{

    if (Inicializado == FALSE)
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), ContadorNaoInicializado);
        return -1;
    } /* if */

    if (pContadorCorr != NULL)
    {
        return pContadorCorr->Contagem;
    } /* if */

    return CNT_CondRetNaoIterador;

} /* Fim fun��o: CNT  &Iterador: obter contagem corrente */

/***************************************************************************
*
*  Fun��o: CNT  &Iterator: obter nome de contador corrente
*  ****/

char *CNT_ObterContadorCorrente()
{

    if (Inicializado == FALSE)
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), ContadorNaoInicializado);
        return NULL;
    } /* if */

    if (pContadorCorr != NULL)
    {
        return pContadorCorr->NomeContador;
    } /* if */

    return NULL;

} /* Fim fun��o: CNT  &Iterator: obter nome de contador corrente */

/***************************************************************************
*
*  Fun��o: CNT  &Iterador: avan�ar para o pr�ximo contador
*  ****/

void CNT_IrProximoContador()
{

    if (Inicializado == FALSE)
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), ContadorNaoInicializado);
        return;
    } /* if */

    if (pContadorCorr != NULL)
    {
        pContadorCorr = pContadorCorr->pProxContador;
    } /* if */

} /* Fim fun��o: CNT  &Iterador: avan�ar para o pr�ximo contador */

/***************************************************************************
*
*  Fun��o: CNT  &Iterator: iniciar percorrimento da lista em ordem alfab�tica
*  ****/

void CNT_IniciarIterador()
{

    if (Inicializado == FALSE)
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), ContadorNaoInicializado);
        return;
    } /* if */

    pContadorCorr = pOrgListaContadores;

} /* Fim fun��o: CNT  &Iterator: iniciar percorrimento da lista em ordem alfab�tica */

/***************************************************************************
*
*  Fun��o: CNT  &Iterator: verificar se o iterador est� ativo
*  ****/

int CNT_EhAtivoIterador()
{

    if (Inicializado == FALSE)
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), ContadorNaoInicializado);
        return FALSE;
    } /* if */

    if (pContadorCorr != NULL)
    {
        return TRUE;
    } /* if */
    return FALSE;

} /* Fim fun��o: CNT  &Iterator: verificar se o iterador est� ativo */

/***************************************************************************
*
*  Fun��o: CNT  &Zerar todos contadores
*  ****/

CNT_tpCondRet CNT_ZerarContadores()
{

    tpContador *pContadorCorr;

    if (Inicializado == FALSE)
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), ContadorNaoInicializado);
        return CNT_CondRetNaoInicializado;
    } /* if */

    for (pContadorCorr = pOrgListaContadores;
         pContadorCorr != NULL;
         pContadorCorr = pContadorCorr->pProxContador)
    {
        if (pContadorCorr->Contagem > 0)
        {
            pContadorCorr->Contagem = 0;
        } /* if */
    }     /* for */

    return CNT_CondRetOK;

} /* Fim fun��o: CNT  &Zerar todos contadores */

/***************************************************************************
*
*  Fun��o: CNT  &Zerar contador dado
*  ****/

CNT_tpCondRet CNT_ZerarContador(char *NomeContador)
{

    tpContador *pContador;

    if (Inicializado == FALSE)
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), ContadorNaoInicializado);
        return CNT_CondRetNaoInicializado;
    } /* if */

    pContador = (tpContador *)TBS_ProcurarSimbolo(pTabela, NomeContador);

    if (pContador != NULL)
    {
        if (pContador->Contagem > 0)
        {
            pContador->Contagem = 0;
        } /* if */
    }
    else
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), "Contador \"%s\" n�o existe.",
                NomeContador);
        return CNT_CondRetNaoContador;

    } /* if */

    return CNT_CondRetOK;

} /* Fim fun��o: CNT  &Zerar contador dado */

#ifdef _DEBUG

/***************************************************************************
*
*  Fun��o: CNT  &Verificar a estrutura dos contadores
*  ****/

int CNT_VerificarEstruturaContadores()
{

    tpContador *pContador;
    tpContador *pContadorAnt;

    int numErros = 0;
    int numSimbolos = 0;

    if (Inicializado == FALSE)
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), ContadorNaoInicializado);
        return 1;
    } /* if */

    /* Verificar se contadores est� inicializado */

    if (Inicializado == FALSE)
    {
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), ContadorNaoInicializado);
        return 1;
    } /* if */

    /* Verificar tabela de simbolos contadores */

    if (TBS_ValidarTabela(pTabela) != TBS_CondRetOK)
    {
        numErros++;
        ExibirPrefixo(-1);
        fprintf(TST_ObterArqLog(), "Tabela de s�mbolos (contadores) em erro.");
    } /* if */

    /* Verificar a lista de contadores */

    if (pOrgListaContadores != NULL)
    {

        numSimbolos = 0;

        pContadorAnt = NULL;
        pContador = pOrgListaContadores;

        while (pContador != NULL)
        {

            /* Verificar conte�do de contagem */

            numSimbolos++;

            if (pContador->Contagem < -2)
            {
                numErros++;
                ExibirPrefixo(0);
                fprintf(TST_ObterArqLog(), "Valor de contagem ilegal: %d  no contador %s",
                        pContador->Contagem, pContador->NomeContador);
            } /* if */

            /* Verificar nome de contador */

            if (pContador->NomeContador[0] == 0)
            {
                numErros++;
                ExibirPrefixo(0);
                fprintf(TST_ObterArqLog(), "Nome de contador � vazio.");
            } /* if */

            /* Verificar ordena��o do nome de contadores */

            if (pContadorAnt != NULL)
            {
                if (strcmp(pContadorAnt->NomeContador, pContador->NomeContador) >= 0)
                {
                    numErros++;
                    ExibirPrefixo(0);
                    fprintf(TST_ObterArqLog(), "Erro de ordena��o na lsiat de contadores.");
                    fprintf(TST_ObterArqLog(), "\n                    Primeiro: %s",
                            pContadorAnt->NomeContador);
                    fprintf(TST_ObterArqLog(), "\n                    Segundo:  %s",
                            pContador->NomeContador);
                } /* if */
            }     /* if */

            /* Avan�ar para o pr�ximo contador */

            pContadorAnt = pContador;
            pContador = pContador->pProxContador;

        } /* fim repete: Verificar a lista de contadores */

        if (numSimbolos != numContadores)
        {
            numErros++;
            ExibirPrefixo(0);
            fprintf(TST_ObterArqLog(), "N�mero de contadores errado. Tabela �: %d  Lista �: %d",
                    numContadores, numSimbolos);
        } /* if */

    } /* fim ativa: Verificar a lista de contadores */

    return numErros;

} /* Fim fun��o: CNT  &Verificar a estrutura dos contadores */

#endif

/*****  C�digo das fun��es encapsuladas no m�dulo  *****/

/***********************************************************************
*
*  $FC Fun��o: CNT  -Obter nome do contador
*
*  $ED Descri��o da fun��o
*     Obt�m o ponteiro para o nome do contador
*
***********************************************************************/

char *ObterNomeContador(void *pDado)
{

    tpContador *pContador;

    pContador = (tpContador *)pDado;
    return pContador->NomeContador;

} /* Fim fun��o: CNT  -Obter nome do contador */

/***********************************************************************
*
*  $FC Fun��o: CNT  -Montar nome de arquivo
*
*  $ED Descri��o da fun��o
*     Adiciona o nome de extens�o caso o nome de arquivo n�o o contenha
*
***********************************************************************/

void MontarNomeArquivo(char *NomeArquivo,
                       char *NomeExtensao)
{

    int i;

    for (i = strlen(NomeArquivo); i > 0; i--)
    {
        if (NomeArquivo[i] == '.')
        {
            return;
        }
        else if (NomeArquivo[i] == '\\')
        {
            break;
        } /* if */
    }     /* for */

    strcat(NomeArquivo, ".");
    strcat(NomeArquivo, NomeExtensao);

} /* Fim fun��o: CNT  -Montar nome de arquivo */

/***********************************************************************
*
*  $FC Fun��o: CNT  -Ler linha limpa
*
*  $ED Descri��o da fun��o
*     Le linha acumulador limpa
*
*  $FV Valor retornado
*     Retorna o n�mero de linhas lidas
*       -1 se fim de arquivo
*       -2 se erro de leitura
*
***********************************************************************/

int LerLinha(FILE *pArquivo)
{

    char *pLido;

    int i;

    /* Tratar fim de arquivo em contadores */

    if (feof(pArquivo))
    {
        BufferLeitura[0] = 0;
        return -1;
    } /* if */

    /* Ler a linha de contadores */

    pLido = fgets(BufferLeitura, DIM_BUFFER, pArquivo);

    if (pLido != NULL)
    {
        numLinha++;
    }
    else
    {
        BufferLeitura[0] = 0;
        if (feof(pArquivo))
        {
            return -1;
        }
        else
        {
            ExibirPrefixo(numLinha);
            fprintf(TST_ObterArqLog(), "Erro de leitura.");
            return -2;
        } /* if */
    }     /* if */

    /* Limpar a linha de contadores */

    for (i = strlen(BufferLeitura) - 1; i >= 0; i--)
    {
        if (strchr(TRIM_CHAR, BufferLeitura[i]) == NULL)
        {
            break;
        } /* if */
    }     /* for */

    BufferLeitura[i + 1] = 0;

    return i + 1;

} /* Fim fun��o: CNT  -Ler linha limpa */

/***********************************************************************
*
*  $FC Fun��o: CNT  -Ler arquivo de defini��o de contadores
*
*  $ED Descri��o da fun��o
*     L� o conjunto de contadores definidos em um arquivo
*
*  $EP Par�metros
*     NomeArquivo
*     Fonte       - identifica o tipo de arquivo
*                      ARQUIVO_DEFINICAO
*                      ARQUIVO_ACUMULADOR
*                   A fonte � utilizada para controlar duplicidade de
*                   declara��o de contadores
*
*  $FV Valor retornado
*     Retorna o n�mero de erros encontrados
*
*  $FA Arquivos manipulados
*     Formato do arquivo de contadores
*
*     Cada linha poder� ser uma de
*       //              - coment�rio, ignorado
*       linha em branco - ignorada
*       nome            - nome de um contador, inicializado para 0
*       nome\=vv        - nome de um contador inicializado para vv
*                    Valores especiais de vv:
*                      vv == -1 contador opcional
*                      vv == -2 contador proibido.
*                      vv <= -3 erro de dado
*     Nomes podem ser quaisquer strings, podendo conter brancos.
*     Os caracteres em branco finais ser�o eliminados
*
*  $EIU Interface com usu�rio pessoa
*     Erro - arquivo n�o existe e � de defini��o
*     Erro - nome definido mais de uma vez
*
***********************************************************************/

int LerContadores(char *NomeArquivo, int Fonte)
{

    FILE *pArq;
    int numErros;
    int numLidos;

    tpContador *pContador;
    char NomeContador[DIM_NOME_CONTADOR + 1];

    char OpAtribuicao;
    int numCampos;
    long ValorContagem;

    tpContador *pContadorCorr;
    tpContador *pContadorAnt;

    numErros = 0;
    numLinha = 0;
    pArq = NULL;

    /* Abrir arquivo de contadores para leitura */

    pArq = fopen(NomeArquivo, "r");
    if (pArq == NULL)
    {
        if (Fonte == ARQUIVO_DEFINICAO)
        {
            ExibirPrefixo(-1);
            fprintf(TST_ObterArqLog(), "N�o abriu o arquivo \"%s\"",
                    NomeArquivo);
            numErros++;
        } /* if */

        return numErros;
    } /* if */

    /* Interpretar todas as linhas do arquivo de contadores */

    numLidos = LerLinha(pArq);

    while (numLidos >= 0)
    {

        /* Tratar linha de contadores em branco */

        if (numLidos == 0)
        {

        } /* fim ativa: Tratar linha de contadores em branco */

        /* Tratar linha de contadores coment�rio */

        else if (memcmp(BufferLeitura, COMENTARIO, strlen(COMENTARIO)) == 0)
        {

        } /* fim ativa: Tratar linha de contadores coment�rio */

        /* Tratar linha de declara��o de contador */

        else
        {

            /* Extrair nome e inicializa��o de linha */

            ValorContagem = 0;

            numCampos = sscanf(BufferLeitura, " %" STR_DIM_NOME_CONTADOR "[^\\]\\%c%ld",
                               NomeContador, &OpAtribuicao, &ValorContagem);

            if (numCampos == 3)
            {
                if (OpAtribuicao != OP_ATRIBUICAO)
                {
                    ExibirPrefixo(numLinha);
                    fprintf(TST_ObterArqLog(), "\nOperador atribui��o incorreto: %s",
                            BufferLeitura);
                    numErros++;
                } /* if */

                if (ValorContagem <= -3)
                {
                    ExibirPrefixo(numLinha);
                    fprintf(TST_ObterArqLog(), "\nValor inicial da contagem ilegal: %s",
                            BufferLeitura);
                    numErros++;
                } /* if */
            }
            else if (numCampos != 1)
            {
                ExibirPrefixo(numLinha);
                fprintf(TST_ObterArqLog(), "\nFormato correto: nome\\=valor. �: %s",
                        BufferLeitura);
                numErros++;
            } /* if */

            /* Criar estrutura contador */

            pContador = (tpContador *)malloc(sizeof(tpContador));
            pContador->pProxContador = NULL;
            pContador->Contagem = ValorContagem;
            pContador->Fonte = Fonte;
            strcpy(pContador->NomeContador, NomeContador);

            /* Inserir contador na tabela */

            if (TBS_InserirSimbolo(pTabela, pContador) != TBS_CondRetOK)
            {

                free(pContador);

                pContador = (tpContador *)TBS_ProcurarSimbolo(pTabela, NomeContador);
                TST_ASSERT(pContador != NULL);

                if (Fonte == ARQUIVO_ACUMULADOR)
                {
                    ExibirPrefixo(numLinha);
                    fprintf(TST_ObterArqLog(), "Nome de contador duplicado em acumulador: %s",
                            NomeContador);
                    numErros++;
                }
                else
                {
                    if (pContador->Fonte == ARQUIVO_DEFINICAO)
                    {
                        ExibirPrefixo(numLinha);
                        fprintf(TST_ObterArqLog(), "Nome de contador duplicado em arquivo de defini��o: %s",
                                NomeContador);
                        numErros++;
                    }
                    else
                    {
                        pContador->Fonte = ARQUIVO_DEFINICAO;
                    } /* if */
                }     /* if */

            } /* fim ativa: Inserir contador na tabela */

            /* Inserir contador em lista ordenada */

            else
            {

                numContadores++;

                pContadorCorr = pOrgListaContadores;
                pContadorAnt = NULL;

                while (pContadorCorr != NULL)
                {
                    if (strcmp(NomeContador,
                               pContadorCorr->NomeContador) < 0)
                    {
                        break;
                    } /* if */
                    pContadorAnt = pContadorCorr;
                    pContadorCorr = pContadorCorr->pProxContador;
                } /* while */

                if (pContadorAnt == NULL)
                {
                    pContador->pProxContador = pOrgListaContadores;
                    pOrgListaContadores = pContador;
                }
                else
                {
                    pContador->pProxContador = pContadorCorr;
                    pContadorAnt->pProxContador = pContador;
                } /* if */

            } /* fim ativa: Inserir contador em lista ordenada */

        } /* fim ativa: Tratar linha de declara��o de contador */

        /* Ler nova linha de contador */

        numLidos = LerLinha(pArq);

    } /* fim repete: Interpretar todas as linhas do arquivo de contadores */

    /* Fechar o arquivo de contadores lido */

    if (pArq != NULL)
    {
        fclose(pArq);
    } /* if */

    return numErros;

} /* Fim fun��o: CNT  -Ler arquivo de defini��o de contadores */

/***********************************************************************
*
*  $FC Fun��o: CNT  -Exibir prefixo de mensagem
*
*  $ED Descri��o da fun��o
*     Exibe o prefixo de mensagens de erro geradas pelo m�dulo de contagem
*
***********************************************************************/

void ExibirPrefixo(int numLinha)
{

    char Msg[DIM_BUFFER];

    Msg[0] = 0;

    if (numLinha > 0)
    {
        sprintf(Msg, "  Linha: %5d ", numLinha);
    } /* if */

    TST_ContarFalhas();

    TST_ExibirPrefixo(ERRO_CONTADOR, Msg);

} /* Fim fun��o: CNT  -Exibir prefixo de mensagem */

/********** Fim do m�dulo de implementa��o: CNT  Contadores de passagem **********/
