/***************************************************************************
*  $MCI M�dulo de implementa��o: ICED Interpretador controle de acesso a espacos de dados din�micos
*
*  Arquivo gerado:              INTRPCED.C
*  Letras identificadoras:      ICED
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

#include    <string.h>
#include    <stdio.h>

#define INTRPCED_OWN
#include "INTRPCED.H"
#undef INTRPCED_OWN

#include    "GENERICO.H"
#include    "CESPDIN.H"
#include    "LERPARM.H"

#define     COMENTARIO  "!!!"

typedef     int      BOOL ;

/* Tabela dos nomes dos comandos de teste espec�ficos */

static const char ExibirTodosEspacosCmd[ ]      = "=exibirtodosespacos" ;
static const char VerificarTodosEspacosCmd[ ]   = "=verificartodosespacos" ;
static const char ObterNumeroEspacosAtivosCmd[ ]= "=obternumeroespacosalocados" ;
static const char ObterNumeroTotalEspacosCmd[ ] = "=obternumerototalespacos" ;
static const char ObterNumeroRegraCmd[ ]        = "=obternumeroregra" ;
static const char ObterTotalEspacosAlocadoCmd[ ]= "=obtertotalespacosalocado" ;
static const char ObterNumNULLCmd[ ]            = "=obternumeroNULLgerado" ;
static const char ExibirNumNULLCmd[ ]           = "=exibirnumeroNULLgerado" ;
static const char LimitarMemoriaDisponivelCmd[ ]= "=limitarmemoriadisponivel" ;
static const char LimitarNumeroEspacosCmd[ ]    = "=limitarnumeroespacos" ;
static const char LimitarFrequenciaNULLCmd[ ]   = "=limitarfrequenciaNULL" ;
static const char EliminarLimiteEspacoCmd[ ]    = "=eliminarlimitesespaco" ;

      #define  TRUE     1
      #define  FALSE    0
            /* Constantes booleanas */

      #define     DIM_VT_ESPACO  5
            /* Dimens�o do vetor de ponteiros para estruturas do contexto */

      static void * vtEspaco[ DIM_VT_ESPACO ] ;
            /* Vetor de ponteiros para estruturas do contexto */

      #define  MSG_ERRO    ">>>"
            /* Prefixo sinalizador de mensagem de erro */

      static int EhPrimeiraVez = TRUE ;
            /* Controle de primeira execu��o */

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***************************************************************************
*
*  Fun��o: ICED &Interpretar comandos controle de espa�o
*  ****/

   TST_tpCondRet ICED_InterpretarTesteEspaco( char * ComandoTeste )
   {

      int  numLidos    = -1 ;
      int  valEsperado = -1 ;
      int  valObtido   = -1 ;

      CED_tpModoExibir Regra ;

      char Msg[ 40 ] ;

      /* Tratar: CED &Exibir todos os espa�os */

         if ( strcmp( ComandoTeste , ExibirTodosEspacosCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &Regra ) ;

            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            CED_ExibirTodosEspacos( Regra ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Tratar: CED &Exibir todos os espa�os */

      /* Tratar: CED &Verificar a integridade de toda a lista */

         else if ( strcmp( ComandoTeste , VerificarTodosEspacosCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &valEsperado ) ;

            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CED_VerificarTudo( NULL ) ;

            return TST_CompararBool( valEsperado , valObtido ,
                   "Condi��o de validade errada." ) ;

         } /* fim ativa: Tratar: CED &Verificar a integridade de toda a lista */

      /* Tratar: CED &Obter n�mero de espa�os ativos */

         else if ( strcmp( ComandoTeste , ObterNumeroEspacosAtivosCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &valEsperado ) ;

            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CED_ObterNumeroEspacosAlocados( ) ;

            return TST_CompararInt( valEsperado , valObtido ,
                   "N�mero de espa�os alocados errado." ) ;

         } /* fim ativa: Tratar: CED &Obter n�mero de espa�os ativos */

      /* Tratar: CED &Obter espa�o total alocado */

         else if ( strcmp( ComandoTeste , ObterTotalEspacosAlocadoCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &valEsperado ) ;

            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CDE_ObterEspacoTotalAlocado( ) ;

            if ( valEsperado < 0 )
            {
               sprintf( Msg , "Total espa�o alocado �: %d bytes" , valObtido ) ;
               TST_ExibirPrefixo( COMENTARIO , Msg ) ;
               return TST_CondRetOK ;
            } /* if */

            return TST_CompararInt( valEsperado , valObtido ,
                   "Espa�o total alocado errado." ) ;

         } /* fim ativa: Tratar: CED &Obter espa�o total alocado */

      /* Tratar: CED &Obter total de espa�os alocados */

         else if ( strcmp( ComandoTeste , ObterNumeroTotalEspacosCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &valEsperado ) ;

            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CED_ObterTotalAlocacoes( ) ;

            return TST_CompararInt( valEsperado , valObtido ,
                   "N�mero total de espa�os alocados errado." ) ;

         } /* fim ativa: Tratar: CED &Obter total de espa�os alocados */

      /* Tratar: CED &Obter n�meros de espa�os */

         else if ( strcmp( ComandoTeste , ObterNumeroRegraCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "ii" ,
                               &Regra , &valEsperado ) ;

            if ( numLidos != 2 )
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CED_ObterNumeroEspacos( Regra ) ;

            return TST_CompararInt( valEsperado , valObtido ,
                   "N�mero de espa�os segundo regra errado." ) ;

         } /* fim ativa: Tratar: CED &Obter n�meros de espa�os */

      /* Tratar: CED &Obter n�mero total de NULLs gerados */

         else if ( strcmp( ComandoTeste , ObterNumNULLCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &valEsperado ) ;

            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararInt( valEsperado ,
                                    CED_ObterNumNULL( ) ,
                   "N�mero de NULLs gerados est� errado." ) ;

         } /* fim ativa: Tratar: CED &Obter n�mero total de NULLs gerados */

      /* Tratar: Exibir n�mero total de NULLs gerados */

         else if ( strcmp( ComandoTeste , ExibirNumNULLCmd ) == 0 )
         {

            TST_ExibirPrefixo( SINALIZA_COMENTARIO ,
                      "O n�mero total de NULLs simulados �: " ) ;

            fprintf( TST_ObterArqLog( ) , " %d" , CED_ObterNumNULL( )) ;

            return TST_CondRetOK ;

         } /* fim ativa: Tratar: Exibir n�mero total de NULLs gerados */

      /* Tratar: CED &Limitar mem�ria dispon�vel */

         else if ( strcmp( ComandoTeste , LimitarMemoriaDisponivelCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &valEsperado ) ;

            if ( ( numLidos != 1 )
              || ( valEsperado < 0    ))
            {
               return TST_CondRetParm ;
            } /* if */

            CED_LimitarMemoriaDisponivel( valEsperado ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Tratar: CED &Limitar mem�ria dispon�vel */

      /* Tratar: CED &Limitar n�mero de espa�os alocados */

         else if ( strcmp( ComandoTeste , LimitarNumeroEspacosCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &valEsperado ) ;

            if ( ( numLidos != 1 )
              || ( valEsperado < 0    ))
            {
               return TST_CondRetParm ;
            } /* if */

            CED_LimitarNumeroTotalEspacos( valEsperado ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Tratar: CED &Limitar n�mero de espa�os alocados */

      /* Tratar: CED &Limitar freq�encia de NULL */

         else if ( strcmp( ComandoTeste , LimitarFrequenciaNULLCmd ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &valEsperado ) ;

            if ( ( numLidos != 1 )
              || ( valEsperado < 0    ))
            {
               return TST_CondRetParm ;
            } /* if */

            valObtido = CED_LimitarFrequencia( valEsperado ) ;
            if ( !valObtido )
            {
               return TST_NotificarFalha( "Freq��ncia limite de malloc em erro." ) ;
            } /* if */

            return TST_CondRetOK ;

         } /* fim ativa: Tratar: CED &Limitar freq�encia de NULL */

      /* Testar: CED &Eliminar limites */

         else if ( strcmp( ComandoTeste , EliminarLimiteEspacoCmd ) == 0 )
         {

            CED_EliminarLimites( ) ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar: CED &Eliminar limites */

      return TST_CondRetNaoExecutou ;

   } /* Fim fun��o: ICED &Interpretar comandos controle de espa�o */

/********** Fim do m�dulo de implementa��o: ICED Interpretador controle de acesso a espacos de dados din�micos **********/

