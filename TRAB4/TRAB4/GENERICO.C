/***************************************************************************
*  $MCI M�dulo de implementa��o: TSTG Controlador de teste gen�rico
*
*  Arquivo gerado:              GENERICO.c
*  Letras identificadoras:      TST
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
#include    "process.h"

#define GENERICO_OWN
#include "GENERICO.H"
#undef GENERICO_OWN

#include    "LERPARM.H"
#include    "INTRPCNT.H"
#include    "INTRPCED.H"
#include    "CESPDIN.H"

/* Constantes internas */

#define     DIM_NOME_ARQ        250
#define     DIM_COMANDO_TESTE   100
#define     DIM_LINHA            16
#define     DIM_MSG             800

#define     DEFAULT_EXT_LOG     ".log"
#define     DEFAULT_EXT_SCRIPT  ".script"
#define     DEFAULT_EXT_ACUM    ".estat"

#define     CHAR_EXT            '.'
#define     CHAR_SEP            '\\'


#define     TRIM_CHAR           " \n\r\t\x026"

/* Comandos gen�ricos do script de teste */

static const char COMMENT_CMD[ ]           = "//" ;
static const char INICIO_CMD[ ]            = "==" ;
static const char RECUPERA_CMD[ ]          = "=recuperar" ;
static const char BKPT_CMD[ ]              = "=breakpoint" ;
static const char CANCELA_CMD[ ]           = "=cancelar" ;
static const char ACEITA_ERRO_CMD[ ]       = "=AceitaFalhasAcumuladas" ;

static const char InicializarEspacosCmd[ ] = "=inicializarespacos" ;

/***********************************************************************
*
*  $TC Tipo de dados: TST Cond ret m�dulo de teste gen�rico
*
*
***********************************************************************/

   typedef enum {

         CondOK ,
               /* Generico executou correto */

         CondNaoAbriu
               /* Gen�rico n�o abriu arquivo */

   } tpCond ;

/*****  Dados encapsulados no m�dulo  *****/

      static char NomeArqLog[ DIM_NOME_ARQ ] = "" ;
            /* Nome do arquivo log */

      static FILE * pArqLog = NULL ;
            /* Ponteiro para o descritor do arquivo log */

      static char NomeArquivoAcumulador[ DIM_NOME_ARQ ] = "" ;
            /* Nome do arquivo acumulador */

      static int ContaCasosTeste = 0 ;
            /* N�mero de casos de teste efetuados */

      static int ContaFalhas = 0 ;
            /* N�mero de falhas encontradas */

      static int ContaComandosExecutados = 0 ;
            /* N�mero de comandos espec�ficos interpretados corretamente */

      #define  DIM_LINHA     16
      #define  TITULO_CHAR   "0123456789ABCDEF"
            /* Cabe�alho de exibi��o de espa�o �til */

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

   static void InicializarTeste( void ) ;

   static void TerminarTeste( void ) ;

   static tpCond AbrirArquivoLog( char * NomeArqParm ) ;

   static TST_tpCondRet RealizarTeste( char * Construto ) ;

   static TST_tpCondRet InterpretarComandos( char * ComandoTeste ) ;

   static void ImprimirHexadecimal( void * pValorParm ,
                                    int    Offset     ,
                                    int    TamValor    ) ;

   static void ExibirResultados( FILE * pArq ) ;

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***************************************************************************
*
*  Fun��o: TSTG &Controlar uma sess�o de teste
*  ****/

   int TST_ControlarSessaoTeste( char * NomeConstrutoParm ,
                                 char * NomeArqScriptParm ,
                                 char * NomeArqLogParm    ,
                                 char * NomeArqAcumParm    )
   {

      TST_tpCondRet CondRet ;

      /* Inicializar a sess�o de teste */

         InicializarTeste( ) ;

      /* Abrir o arquivo script */

         if ( LER_AbrirArquivoScript( NomeArqScriptParm ) != LER_CondRetOK )
         {
            printf( "\n>>>   Nao foi possivel abrir arquivo script: %s"  ,
                      NomeArqScriptParm ) ;
            return 4 ;
         } /* if */

      /* Abrir o arquivo log */

         if ( NomeArqLogParm[ 0 ] != 0 )
         {
            if ( AbrirArquivoLog( NomeArqLogParm ) != CondOK )
            {
               printf( "\n>>>   Nao foi possivel abrir arquivo log: %s" ,
                         NomeArqLogParm ) ;
               printf( "\n>>>   A saida sera gerada na console." ) ;

            } /* if */
         } /* if */

      /* Registrar o arquivo acumulador */

         strcpy( NomeArquivoAcumulador , NomeArqAcumParm ) ;

      /* Efetuar o teste */

         CondRet = RealizarTeste( NomeConstrutoParm ) ;

      /* Concluir a execu��o do teste */

         TerminarTeste( ) ;

         if ( CondRet == TST_CondRetOK )
         {
            return 0 ;
         } /* if */

         return 4 ;

   } /* Fim fun��o: TSTG &Controlar uma sess�o de teste */

/***************************************************************************
*
*  Fun��o: TSTG &Comparar booleano
*  ****/

   TST_tpCondRet TST_CompararBool( int    ValorEsperado ,
                                   int    ValorObtido   ,
                                   char * pMensagem      )
   {

      if ( ( ValorObtido   != 0 )
        && ( ValorEsperado == 0 ))
      {
         ContaFalhas ++ ;
         TST_ExibirPrefixo( SINALIZA_ERRO , pMensagem ) ;
         fprintf( pArqLog , "Deveria ser: FALSE �: TRUE" ,
                  ValorEsperado , ValorObtido ) ;
         return TST_CondRetErro ;
      } /* if */

      if ( ( ValorObtido   == 0 )
        && ( ValorEsperado != 0 ))
      {
         ContaFalhas ++ ;
         TST_ExibirPrefixo( SINALIZA_ERRO , pMensagem ) ;
         fprintf( pArqLog , "Deveria ser: TRUE �: FALSE" ,
                  ValorEsperado , ValorObtido ) ;
         return TST_CondRetErro ;
      } /* if */

      return TST_CondRetOK ;

   } /* Fim fun��o: TSTG &Comparar booleano */

/***************************************************************************
*
*  Fun��o: TSTG &Comparar caractere
*  ****/

   TST_tpCondRet TST_CompararChar( char   ValorEsperado ,
                                   char   ValorObtido   ,
                                   char * pMensagem       )
   {

      if ( ValorObtido != ValorEsperado )
      {
         ContaFalhas ++ ;
         TST_ExibirPrefixo( SINALIZA_ERRO , pMensagem ) ;
         fprintf( pArqLog , "Deveria ser: '%c'  �: '%c'" ,
                  ValorEsperado , ValorObtido ) ;
         return TST_CondRetErro ;
      } /* if */
      return TST_CondRetOK ;

   } /* Fim fun��o: TSTG &Comparar caractere */

/***************************************************************************
*
*  Fun��o: TSTG &Comparar inteiro
*  ****/

   TST_tpCondRet TST_CompararInt( long   ValorEsperado ,
                                  long   ValorObtido   ,
                                  char * pMensagem       )
   {

      if ( ValorObtido != ValorEsperado )
      {
         ContaFalhas ++ ;
         TST_ExibirPrefixo( SINALIZA_ERRO , pMensagem ) ;
         fprintf( pArqLog , "Deveria ser: %ld  �: %ld" ,
                  ValorEsperado , ValorObtido ) ;
         return TST_CondRetErro ;
      } /* if */
      return TST_CondRetOK ;

   } /* Fim fun��o: TSTG &Comparar inteiro */

/***************************************************************************
*
*  Fun��o: TSTG &Comparar flutuante
*  ****/

   TST_tpCondRet TST_CompararFloat( double   ValorEsperado ,
                                    double   ValorObtido   ,
                                    double   Tolerancia    ,
                                    char *   pMensagem      )
   {

      double Razao ;

      Razao = ValorEsperado / ValorObtido ;
         
      if ( ( Razao < 1.0 - Tolerancia )
        || ( Razao > 1.0 + Tolerancia ))
      {
         ContaFalhas ++ ;
         TST_ExibirPrefixo( SINALIZA_ERRO , pMensagem ) ;
         fprintf( pArqLog , "Toler�ncia %lg, esperado: %lg , obtido: %lg" ,
                  Tolerancia , ValorEsperado , ValorObtido ) ;
         return TST_CondRetErro ;
      } /* if */
      return TST_CondRetOK ;
         

   } /* Fim fun��o: TSTG &Comparar flutuante */

/***************************************************************************
*
*  Fun��o: TSTG &Comparar string
*  ****/

   TST_tpCondRet TST_CompararString( char * ValorEsperado ,
                                     char * ValorObtido   ,
                                     char * pMensagem       )
   {

      if ( strcmp( ValorObtido , ValorEsperado ) != 0 )
      {
         ContaFalhas ++ ;
         TST_ExibirPrefixo( SINALIZA_ERRO , pMensagem ) ;
         fprintf( pArqLog , "Deveria ser: >%s<  �: >%s<" ,
                  ValorEsperado , ValorObtido ) ;
         return TST_CondRetErro ;
      } /* if */
      return TST_CondRetOK ;

   } /* Fim fun��o: TSTG &Comparar string */

/***************************************************************************
*
*  Fun��o: TSTG &Comparar espa�o
*  ****/

   TST_tpCondRet TST_CompararEspaco( void * ValorEsperado ,
                                     void * ValorObtido   ,
                                     int    TamEspaco     ,
                                     char * pMensagem       )
   {

      int i ;

      /* Tratar espa�os n�o iguais */

         if ( memcmp( ValorObtido , ValorEsperado , TamEspaco ) != 0 )
         {

            ContaFalhas ++ ;
            TST_ExibirPrefixo( SINALIZA_ERRO , pMensagem ) ;
            fprintf( pArqLog , "\n" ) ;

            for( i = 0 ; i < TamEspaco ; i += DIM_LINHA )
            {
               fprintf( pArqLog , "\n   %05d Nao �: " , i ) ;
               ImprimirHexadecimal( ValorEsperado , i , TamEspaco ) ;

               fprintf( pArqLog , "\n         �    : " ) ;
               ImprimirHexadecimal( ValorObtido , i , TamEspaco ) ;

               fprintf( pArqLog , "\n" ) ;
            } /* for */

            return TST_CondRetErro ;

         } /* fim ativa: Tratar espa�os n�o iguais */

      return TST_CondRetOK ;

   } /* Fim fun��o: TSTG &Comparar espa�o */

/***************************************************************************
*
*  Fun��o: TSTG &Comparar ponteiro
*  ****/

   TST_tpCondRet TST_CompararPonteiro( void * PonteiroEsperado ,
                                       void * PonteiroObtido   ,
                                       char * pMensagem       )
   {

      if ( PonteiroObtido != PonteiroEsperado )
      {
         ContaFalhas ++ ;
         TST_ExibirPrefixo( SINALIZA_ERRO , pMensagem ) ;
         fprintf( pArqLog , "Deveria ser: %p  �: %p" ,
                  PonteiroEsperado , PonteiroObtido ) ;
         return TST_CondRetErro ;
      } /* if */
      return TST_CondRetOK ;

   } /* Fim fun��o: TSTG &Comparar ponteiro */

/***************************************************************************
*
*  Fun��o: TSTG &Comparar ponteiro nulo
*  ****/

   TST_tpCondRet TST_CompararPonteiroNulo( int    ModoEsperado ,
                                           void * PonteiroObtido   ,
                                           char * pMensagem       )
   {

      if ( (( ModoEsperado == 0 ) && ( PonteiroObtido != NULL ))
        || (( ModoEsperado != 0 ) && ( PonteiroObtido == NULL )) )
      {
         ContaFalhas ++ ;
         TST_ExibirPrefixo( SINALIZA_ERRO , pMensagem ) ;
         if ( ModoEsperado )
         {
            fprintf( pArqLog , "Deveria ser: n�o NULL  �: NULL" ) ;
         } else
         {
            fprintf( pArqLog , "Deveria ser: NULL  �: %p" ,
                     PonteiroObtido ) ;
         } /* if */
         return TST_CondRetErro ;
      } /* if */
      return TST_CondRetOK ;

   } /* Fim fun��o: TSTG &Comparar ponteiro nulo */

/***************************************************************************
*
*  Fun��o: TSTG &Assertiva controlada
*  ****/

   void TST_Assert( int Expressao , int Linha , char * NomeArq )
   {

      char Msg[ DIM_MSG ] ;

      if ( Expressao )
      {
         return ;
      } /* if */

      sprintf( Msg , "Assertiva falhou:  linha %d  arquivo: %s" ,
                          Linha , NomeArq ) ;

      TST_ExibirPrefixo( "\n   " , "************************************\n" ) ;
      TST_NotificarFalha( Msg ) ;

      TST_ExibirPrefixo( "\n   " , "Assertiva  Assertiva  Assertiva\n" ) ;

      if ( pArqLog != stdout )
      {
         fclose( pArqLog ) ;
         pArqLog = stdout ;

         TST_ExibirPrefixo( "\n   " , "************************************\n" ) ;
         TST_NotificarFalha( Msg ) ;

         TST_ExibirPrefixo( "\n   " , "Assertiva  Assertiva  Assertiva\n" ) ;

      } /* if */

      exit( 4 ) ;

   } /* Fim fun��o: TSTG &Assertiva controlada */

/***************************************************************************
*
*  Fun��o: TSTG &Exibir prefixo da mensagem
*  ****/

   void TST_ExibirPrefixo( char * Indicador , char * pMensagem )
   {

      fprintf( pArqLog , "\n%s %4d  Linha: %5d  %s " ,
               Indicador , ContaFalhas , LER_ObterNumLinhas( ) , pMensagem ) ;

   } /* Fim fun��o: TSTG &Exibir prefixo da mensagem */

/***************************************************************************
*
*  Fun��o: TSTG &Notificar falha
*  ****/

   TST_tpCondRet TST_NotificarFalha( char * pMensagem )
   {

      ContaFalhas ++ ;
      TST_ExibirPrefixo( SINALIZA_ERRO , pMensagem ) ;
      return TST_CondRetErro ;

   } /* Fim fun��o: TSTG &Notificar falha */

/***************************************************************************
*
*  Fun��o: TSTG &Incrementar contador de falhas
*  ****/

   void TST_ContarFalhas( )
   {

      ContaFalhas ++ ;

   } /* Fim fun��o: TSTG &Incrementar contador de falhas */

/***************************************************************************
*
*  Fun��o: TSTG &Obter n�mero de casos de teste efetuados
*  ****/

   int TST_ObterNumCasosTeste( )
   {

      return ContaCasosTeste ;

   } /* Fim fun��o: TSTG &Obter n�mero de casos de teste efetuados */

/***************************************************************************
*
*  Fun��o: TSTG &Obter n�mero de comandos espec�ficos corretamente interpretados
*  ****/

   int TST_ObterNumComandosExecutados( void )
   {

      return ContaComandosExecutados ;

   } /* Fim fun��o: TSTG &Obter n�mero de comandos espec�ficos corretamente interpretados */

/***************************************************************************
*
*  Fun��o: TSTG &Obter n�mero de falhas encontradas
*  ****/

   int TST_ObterNumFalhas( void )
   {

      return ContaFalhas ;

   } /* Fim fun��o: TSTG &Obter n�mero de falhas encontradas */


/***********************************************************************
*
*  $FC Fun��o: TSTG &Obter origem do nome da extens�o de arquivo
*
*  $ED Descri��o da fun��o
*     Procura o in�cio do nome da extens�o contido no nome do arquivo
*     dado por par�metro
*
*     Evite o uso de nomes de arquivos com dois ou mais caracteres ".".
*     Isto pode confundir no caso de se desejar a adi��o de algum nome
*     de extens�o padr�o.
*
*  $EP Par�metros
*     $P NomeArqParm - nome de arquivo a ser analisado
*
*  $FV Valor retornado
*     Ponteiro para o caractere '.' que separa a extens�o.
*     Ser� NULL caso o nome do arquivo n�o contenha extens�o.
*
***********************************************************************/

   char * TST_ObterInicioExtensao( char * NomeArqParm )
   {

      int i ;

      for( i = ( int ) strlen( NomeArqParm ) - 1 ; i >= 0 ; i-- )
      {
         if ( NomeArqParm[ i ] == CHAR_EXT )
         {
            return NomeArqParm + i ;
         } /* if */
         if ( NomeArqParm[ i ] == CHAR_SEP )
         {
            return NULL ;
         } /* if */
      } /* for */

      return NULL ;

   } /* Fim fun��o: TSTG &Obter origem do nome da extens�o de arquivo */

/***************************************************************************
*
*  Fun��o: TSTG &Obter ponteiro para arquivo LOG
*  ****/

   FILE * TST_ObterArqLog( void )
   {

      if ( pArqLog == NULL )
      {
         return stdout ;
      } else {
         return pArqLog ;
      } /* if */

   } /* Fim fun��o: TSTG &Obter ponteiro para arquivo LOG */

/***************************************************************************
*
*  Fun��o: TSTG &Exibir um espa�o qualquer em formato hexadecimal e char
*  ****/

   void TST_ExibirEspacoHexa( int    tamEspaco ,
                              void * Ponteiro   )
   {

      char * pEspaco ;

      int inxLinha ,
          i ;

      pEspaco = ( char * ) Ponteiro ;

      /* Exibir cabe�alho */

         fprintf( pArqLog , "\n        " ) ;

         for( i = 0  ; i < DIM_LINHA ; i++ )
         {
            fprintf( pArqLog , "%3X" , i ) ;
         } /* for */

         fprintf( pArqLog , "   " TITULO_CHAR ) ;

      /* Exibir todas as linhas */

         for ( inxLinha = 0 ; inxLinha < tamEspaco ; inxLinha += DIM_LINHA )
         {
            fprintf( pArqLog , "\n    %03X " , inxLinha ) ;
            ImprimirHexadecimal( pEspaco , inxLinha , tamEspaco ) ;
         } /* for */

         fprintf( pArqLog , "\n" ) ;

   } /* Fim fun��o: TSTG &Exibir um espa�o qualquer em formato hexadecimal e char */


/*****  C�digo das fun��es encapsuladas no m�dulo  *****/


/***********************************************************************
*
*  $FC Fun��o: TSTG -Inicializar o m�dulo de teste
*
*  $ED Descri��o da fun��o
*     Inicializa todas as vari�veis globais exportadas e encapsuladas
*     do m�dulo de teste gen�rico.
*
*     Embora pouco recomend�vel (ver descri��o do m�dulo TesteSpc)
*     esta fun��o pode ser alterada para assegurar o estabelecimento
*     do contexto inicial do m�dulo TesteSpc. Este deve conter uma
*     fun��o de inicializa��o a ser chamada pela presente fun��o.
*
***********************************************************************/

   void InicializarTeste( void )
   {

      NomeArqLog[ 0 ]    = 0 ;
      pArqLog            = stdout ;

      ContaComandosExecutados = 0 ;
      ContaFalhas        = 0 ;
      ContaCasosTeste    = 0 ;

      TST_ASSERT( LER_InicializarLeitura( ) ) ;

   } /* Fim fun��o: TSTG -Inicializar o m�dulo de teste */


/***********************************************************************
*
*  $FC Fun��o: TSTG -Terminar a execu��o do m�dulo de teste
*
*  $ED Descri��o da fun��o
*     Fecha todos os arquivos e desaloca todos os espa�os de dados
*     alocados sob controle do m�dulo de teste gen�rico
*
***********************************************************************/

   void TerminarTeste( void )
   {

      FILE * pArqAcumula   = NULL ;
      int numMassasTeste   = 0 ;
      int numCasosTeste    = 0 ;
      int numCmdExecutados = 0 ;
      int numLinhasLidas   = 0 ;
      int numFalhas        = 0 ;

      /* Fechar o arquivo de script */

         LER_TerminarLeitura( ) ;

      /* Acumular os resultados dos testes */

         if ( NomeArquivoAcumulador[ 0 ] != 0 )
         {

            if ( TST_ObterInicioExtensao( NomeArquivoAcumulador ) == NULL )
            {
               strcat( NomeArquivoAcumulador , DEFAULT_EXT_ACUM ) ;
            } /* if */

            pArqAcumula = fopen( NomeArquivoAcumulador , "r" ) ;
            if ( pArqAcumula != 0 )
            {
               fscanf( pArqAcumula , " %d %d %d %d %d" ,
                            &numMassasTeste , &numCmdExecutados ,
                            &numCasosTeste  , &numLinhasLidas   ,
                            &numFalhas ) ;
               fclose( pArqAcumula ) ;
            } /* if */

            numMassasTeste   ++ ;
            numCasosTeste    += ContaCasosTeste ;
            numCmdExecutados += ContaComandosExecutados ;
            numLinhasLidas   += LER_ObterNumLinhas( ) ;
            numFalhas        += ContaFalhas ;

            pArqAcumula = fopen( NomeArquivoAcumulador , "w" ) ;
            if ( pArqAcumula != 0 )
            {
               fprintf( pArqAcumula , "  %d  %d  %d  %d  %d" ,
                            numMassasTeste  , numCmdExecutados ,
                            numCasosTeste   , numLinhasLidas   ,
                            numFalhas ) ;
               fclose( pArqAcumula ) ;
            } /* if */

         } /* fim ativa: Acumular os resultados dos testes */

      /* Exibir os resultados do teste */

         ExibirResultados( pArqLog ) ;

         if ( pArqLog != stdout )
         {
            ExibirResultados( stdout ) ;
         } /* if */

         fprintf( pArqLog , "\n\n!!! Numero de espacos ainda alocados: %d",
                  CED_ObterNumeroEspacosAlocados( ) ) ;

      /* Fechar o arquivo de log */

         if ( pArqLog != stdout )
         {

            fprintf( pArqLog , "\n" ) ;
            fclose(  pArqLog ) ;
            pArqLog = stdout ;
            NomeArqLog[ 0 ] = 0 ;

         } /* fim ativa: Fechar o arquivo de log */

   } /* Fim fun��o: TSTG -Terminar a execu��o do m�dulo de teste */


/***********************************************************************
*
*  $FC Fun��o: TSTG -Abrir arquivo log
*
*  $ED Descri��o da fun��o
*     Abre o arquivo log a ser utilizado como sa�da durante os testes.
*     Todas as sa�das geradas pelo m�dulo de teste s�o dirigidos para
*     este arquivo. Caso n�o tenha sido aberto, ser� utilizado o
*     arquivo stdout.
*
*     A extens�o default do arquivo log � ".log" e ser� acrescida ao nome
*     do arquivo caso este n�o contenha nome de extens�o.
*
*  $EP Par�metros
*     $P NomeArqParm - nome do arquivo. Pode (deve) ser um nome relativo
*                      ao diret�rio corrente. Caso o arquivo
*                      exista, ser� destruido sem perguntar se pode.
*
*  $FV Valor retornado
*     TST_CondOK       - abriu corretamente
*     TST_CondNaoAbriu - ocorreu algum problema ao tentar abrir.
*                        O problema n�o � descriminado.
*                        O arquivo de sa�da do log volta a ser stdout
*
*  $EIU Interface com usu�rio pessoa
*     Esta fun��o deve ser chamada antes de iniciar a execu��o dos testes.
*
***********************************************************************/

   tpCond AbrirArquivoLog( char * NomeArqParm )
   {

      strcpy( NomeArqLog , NomeArqParm ) ;
      if ( TST_ObterInicioExtensao( NomeArqLog ) == NULL )
      {
         strcat( NomeArqLog , DEFAULT_EXT_LOG ) ;
      } /* if */

      pArqLog = fopen( NomeArqLog , "w" ) ;
      if ( pArqLog != NULL )
      {
         return CondOK ;
      } /* if */

      pArqLog = stdout ;
      NomeArqLog[ 0 ] = 0 ;

      return CondNaoAbriu ;

   } /* Fim fun��o: TSTG -Abrir arquivo log */


/***********************************************************************
*
*  $FC Fun��o: TSTG -Interpretar toda a massa de teste
*
*  $ED Descri��o da fun��o
*     Interpreta o arquivo de diretivas (script) de teste j� aberto
*
*     A fun��o de teste � dividida em tr�s classes de comandos:
*
*     - os de teste gen�rico, que coordenam e controlam a realiza��o do
*       teste. Estes comandos s�o interpretados por esta fun��o.
*     - os de teste e controle de m�dulos componentes do arcabou�o.
*       Estes comandos s�o interpretados por fun��es de teste espec�ficas
*       para cada um dos m�dulos componentes.
*     - os de teste espec�fico do m�dulo (ou m�dulos) a ser testado.
*       O m�dulo de teste espec�fico interpreta os comandos projetados
*       para realizar os testes do m�dulo a testar.
*
*     A fun��o de teste gen�rico l� um arquivo de diretivas de teste.
*     Cada linha deste arquivo � um comando de teste.
*     A fun��o de teste l� uma a uma as linhas do arquivo, limpa os
*     caracteres em branco no final da linha e procura interpretar o
*     comando.
*
*     As fun��es de teste espec�ficas comunicam os resultados da execu��o
*     de cada comando por meio de uma condi��o de retorno.
*
*     Cada linha do arquivo script de teste corresponde a um comando de
*     teste.
*
*     Caso seja encontrada uma falha n�o esperada (ver comando =recuperar)
*     os restantes comandos do caso de teste ser�o ignorados e contados
*     como problemas.
*
*     A sintaxe utilizada pelos comandos de teste � semelhante a assembler.
*     Esta sintaxe facilita a an�lise:
*
*        =<cmd> <par�metros>
*
*     em que:
*        =            - todos os comandos a serem interpretados iniciam
*                       com este caractere
*        <cmd>        - � o string que identifica o comando
*        <par�metros> - s�o zero ou mais itens a serem utilizados
*                       pelo comando de teste. Os itens devem, de prefer�ncia,
*                       ser separados um do outro por um caractere "espa�o"
*
*     Deve ser adotado o seguinte padr�o de uso:
*
*     - procure sempre utlizar uma das fun��es TST_CompararXXX, na forma:
*          return TST_CompararXXX( valoresperado, funcao a testar , mensagem )
*
*     - as fun��es de compara��o fornecidas sempre geram as mensagens no
*       padr�o estipulado
*
*     - quando n�o for poss�vel utilizar uma fun��o de compara��o, reporte
*       a falha, esperadas ou n�o, atrav�s da fun��o
*          TST_ImprimirPrefixo( Mensagem )
*
*     - a seguir mostre o valor esperado e o obtido
*
*     - Informa��es relativas a falhas dos testes s�o sinalizadas por
*       uma mensagem iniciando com ">>>" o n�mero da linha do comando de teste
*       e o n�mero de falhas encontradas at� o momento.
*
*     - Informa��es relativas � execu��o dos testes s�o sinalizadas por
*       uma mensagem iniciando com "!!!".
*
*  $FV Valor retornado
*     TST_CondRetErro   - caso tenha sido encontrado um ou mais problemas
*     TST_CondRetOK     - caso o teste tenha terminado com zero problemas
*                         n�o esperados.
*
*  $FGP Tipos e Vari�veis globais externas do pr�prio m�dulo
*     pArqScript - deve referenciar o arquivo script aberto
*     pArqLog    - deve referenciar o arquivo log aberto
*
***********************************************************************/

   TST_tpCondRet RealizarTeste( char * Construto )
   {

      int NumLidos ,
          SaltaComandos ;

      int EhCasoTesteVazio = 0 ;

      char ComandoTeste[ DIM_COMANDO_TESTE ] ;

      int Dummy ;

      TST_tpCondRet CondRetornada = TST_CondRetErro ;

      if ( pArqLog == stdout )
      {
         fprintf( pArqLog , "\n" ) ;

      } else
      {
         fprintf( pArqLog , "\n%s Inicio dos testes" ,
                  SINALIZA_COMENTARIO ) ;
         fprintf( pArqLog , "\n%s      Construto sob teste: %s" ,
                  SINALIZA_COMENTARIO , Construto ) ;
         fprintf( pArqLog , "\n%s      Script de teste:     %s\n" ,
                  SINALIZA_COMENTARIO , LER_ObterNomeArqScript( )) ;
      } /* if */

      SaltaComandos = 1 ;
      NumLidos = LER_LerLinhaScript( ) ;

      CED_InicializarControlador( ) ;

      while ( NumLidos >= 0 ) {

      /* Obter comando de teste */

         if ( LER_TamBuffer > 0 )
         {

            ComandoTeste[ 0 ] = 0 ;
            ComandoTeste[ DIM_COMANDO_TESTE - 1 ] = 0 ;
            ComandoTeste[ DIM_COMANDO_TESTE - 2 ] = '\xA9' ; /* controles de extravas�o do buffer de leitura */

            if ( sscanf( LER_Buffer , "%s" , ComandoTeste ) != 1 )
            {
               ContaFalhas ++ ;
               TST_ExibirPrefixo( SINALIZA_ERRO , "Linha n�o cont�m comando:" ) ;
               fprintf( pArqLog , " %s" , LER_Buffer ) ;
               LER_TamBuffer = -1 ;

            } else if ( ( ComandoTeste[ DIM_COMANDO_TESTE - 1 ] != 0 )
                     && ( ComandoTeste[ DIM_COMANDO_TESTE - 2 ] != '\xA9' ))
            {
               ContaFalhas ++ ;
               TST_ExibirPrefixo( SINALIZA_ERRO , "Comando extravasou o espa�o para leitura:" ) ;
               fprintf( pArqLog , " %s" , LER_Buffer ) ;
               LER_TamBuffer = -1 ;
               break ;
            } /* if */

         } /* fim ativa: Obter comando de teste */

      /* Tratar linha em branco */

         if ( LER_TamBuffer <= 0 )
         {

         } /* fim ativa: Tratar linha em branco */

      /* Tratar coment�rio */

         else if ( memcmp( ComandoTeste , COMMENT_CMD , strlen( COMMENT_CMD )) == 0 )
         {

            TST_ExibirPrefixo( SINALIZA_COMENTARIO , LER_Buffer ) ;

         } /* fim ativa: Tratar coment�rio */

      /* Tratar comando breakpoint */

         else if ( strcmp( ComandoTeste , BKPT_CMD ) == 0 )
         {

            Dummy = 0 ;

         } /* fim ativa: Tratar comando breakpoint */

      /* Tratar de comando de cancelamento */

         else if ( strcmp( ComandoTeste , CANCELA_CMD ) == 0 )
         {

            ContaFalhas ++ ;
            TST_ExibirPrefixo( SINALIZA_COMENTARIO ,
                      "Solicitado o cancelamento da execu��o do script de teste." ) ;
            break ;

         } /* fim ativa: Tratar de comando de cancelamento */

      /* Tratar in�cio de caso de teste */

         else if ( memcmp( ComandoTeste , INICIO_CMD , strlen( INICIO_CMD )) == 0 )
         {

            if ( EhCasoTesteVazio != 0 )
            {
               ContaFalhas ++ ;
               TST_ExibirPrefixo( SINALIZA_ERRO , "O caso de teste anterior est� vazio." ) ;
            } /* if */

            fprintf( pArqLog , "\n%5d  %s" , LER_ObterNumLinhas( ) , LER_Buffer ) ;
            ContaCasosTeste ++ ;
            SaltaComandos    = 0 ;

            EhCasoTesteVazio = 1 ;

         } /* fim ativa: Tratar in�cio de caso de teste */

      /* Interpretar comandos de teste espec�ficos */

         else if ( SaltaComandos == 0 )
         {

            EhCasoTesteVazio = 0 ;
            CondRetornada = InterpretarComandos( ComandoTeste ) ;

            switch ( CondRetornada ) {

            /* Tratar retorno OK */

               case TST_CondRetOK :
               {

                  ContaComandosExecutados ++ ;

                  break ;

               } /* fim ativa: Tratar retorno OK */

            /* Tratar retorno fun��o testada n�o retornou OK */

               case TST_CondRetErro :
               {

                  NumLidos = LER_LerLinhaScript( ) ;

                  if ( memcmp( LER_Buffer , RECUPERA_CMD , strlen( RECUPERA_CMD )) == 0 )
                  {
                     ContaFalhas -- ;
                     TST_ExibirPrefixo( SINALIZA_RECUPERA , "Falha esperada foi recuperada." ) ;
                  } else {
                     if ( LER_Buffer[ 0 ] != 0 )
                     {
                        ContaFalhas ++ ;
                        TST_ExibirPrefixo( SINALIZA_ERRO , "N�o � comando recuperar:" ) ;
                        fprintf( pArqLog , " %s" , LER_Buffer ) ;
                        SaltaComandos = 1 ;
                     } /* if */
                  } /* if */

                  break ;

               } /* fim ativa: Tratar retorno fun��o testada n�o retornou OK */

            /* Tratar retorno erro de par�metro em comando */

               case TST_CondRetParm :
               {

                  ContaFalhas ++ ;
                  TST_ExibirPrefixo( SINALIZA_ERRO , "Erro nos par�metros do comando:" ) ;
                  fprintf( pArqLog , " %s" , LER_Buffer ) ;

                  NumLidos = LER_LerLinhaScript( ) ;

                  if ( memcmp( LER_Buffer , RECUPERA_CMD , strlen( RECUPERA_CMD )) == 0 )
                  {
                     ContaFalhas -- ;
                     TST_ExibirPrefixo( SINALIZA_RECUPERA , "Falha esperada foi recuperada." ) ;
                  } else {
                     SaltaComandos = 1 ;
                  } /* if */

                  break ;

               } /* fim ativa: Tratar retorno erro de par�metro em comando */

            /* Tratar comando ainda n�o implementado */

               case TST_CondRetNaoImplementado :
               {

                  ContaFalhas ++ ;
                  TST_ExibirPrefixo( SINALIZA_ERRO , "Comando de teste ainda n�o implementado:" ) ;
                  fprintf( pArqLog , " %s" , LER_Buffer ) ;
                  SaltaComandos = 1 ;

                  break ;

               } /* fim ativa: Tratar comando ainda n�o implementado */

            /* Tratar retorno comando desconhecido */

               case TST_CondRetNaoConhec :
               case TST_CondRetNaoExecutou :
               {

                  ContaFalhas ++ ;
                  TST_ExibirPrefixo( SINALIZA_ERRO , "Comando n�o conhecido:" ) ;
                  fprintf( pArqLog , " %s" , LER_Buffer ) ;
                  SaltaComandos = 1 ;

                  break ;

               } /* fim ativa: Tratar retorno comando desconhecido */

            /* Tratar retorno faltou mem�ria */

               case TST_CondRetMemoria :
               {

                  ContaFalhas ++ ;
                  TST_ExibirPrefixo( SINALIZA_ERRO , "Faltou mem�ria para o comando:" ) ;
                  fprintf( pArqLog , " %s" , LER_Buffer ) ;
                  SaltaComandos = 1 ;

                  break ;

               } /* fim ativa: Tratar retorno faltou mem�ria */

            /* Tratar retorno desconhecido */

               default :
               {

                  ContaFalhas ++ ;
                  TST_ExibirPrefixo( SINALIZA_ERRO , "Condi��o de retorno n�o conhecida:" ) ;
                  fprintf( pArqLog , " %d" , CondRetornada ) ;
                  SaltaComandos = 1 ;

                  break ;

               } /* fim ativa: Tratar retorno desconhecido */

            } /* fim seleciona: Interpretar comandos de teste espec�ficos */

         } /* fim ativa: Interpretar comandos de teste espec�ficos */

      /* Tratar salto de comandos */

         else
         {

            ContaFalhas ++ ;
            TST_ExibirPrefixo( SINALIZA_ERRO , "Comando de teste ignorado:" ) ;
            fprintf( pArqLog , " %s" , LER_Buffer ) ;

         } /* fim ativa: Tratar salto de comandos */

      /* Avan�ar para a pr�xima linha */

         NumLidos = LER_LerLinhaScript( ) ;

      } /* fim repete: Raiz de TSTG -Interpretar toda a massa de teste */

      if ( ContaFalhas > 0 )
      {
         return TST_CondRetErro ;
      } else {
         return TST_CondRetOK ;
      } /* if */

   } /* Fim fun��o: TSTG -Interpretar toda a massa de teste */


/***********************************************************************
*
*  $FC Fun��o: TSTG -Efetuar os comandos de teste espec�ficos
*
*  $ED Descri��o da fun��o
*     Esse m�dulo implementa a seq��ncia (daisy chain) de interpreta��o
*     de comandos de teste.
*     Todos os m�dulos que implementam instrumenta��o pertencente ao
*     arcabou�o devem retornar TST_CondRetNaoExecutou  caso o
*     comando a interpretar n�o possa ser interpretado pelo m�dulo
*     em quest�o.
*
*     O interpretador de comandos espec�ficos do m�dulo a testar
*     deve ser sempre o �ltimo a ser chamado. Caso n�o consiga
*     identificar o comando a interpretar, deve retornar
*     TST_CondRetNaoConhec
*
***********************************************************************/

   TST_tpCondRet InterpretarComandos( char * ComandoTeste )
   {

      TST_tpCondRet CondRetornada = TST_CondRetErro ;

      int IntEsp   = -1 ,
          numLidos = -1  ;

      /* Tratar comando de recupera��o de falhas acumuladas */

         if ( strcmp( ComandoTeste , ACEITA_ERRO_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "i" , &IntEsp ) ;
            if ( numLidos != 1 )
            {
               return TST_CondRetParm ;
            } /* if */

            if ( IntEsp == ContaFalhas )
            {
               ContaFalhas = 0 ;
               TST_ExibirPrefixo( "<<<" , "Falhas registradas eram esperadas e foram recuperadas." ) ;
               return TST_CondRetOK ;
            } /* if */

            return TST_CompararInt( IntEsp , ContaFalhas ,
                      "N�mero de falhas esperadas errado." ) ;

         } /* fim ativa: Tratar comando de recupera��o de falhas acumuladas */

      /* Interpretar comandos de leitura */

         CondRetornada = LER_InterpretarComandos( ComandoTeste ) ;
         if ( CondRetornada != TST_CondRetNaoExecutou )
         {
            return CondRetornada ;
         } /* if */

      /* Interpretar comandos de contagem */

         CondRetornada = ICNT_EfetuarComadoContagem( ComandoTeste ) ;
         if ( CondRetornada != TST_CondRetNaoExecutou )
         {
            return CondRetornada ;
         } /* if */

      /* Interpretar comandos de controle de acesso */

         CondRetornada = ICED_InterpretarTesteEspaco( ComandoTeste ) ;
         if ( CondRetornada != TST_CondRetNaoExecutou )
         {
            return CondRetornada ;
         } /* if */

      /* Interpretar comandos espec�ficos do m�dulo a testar */

         return TST_EfetuarComando( ComandoTeste ) ;

   } /* Fim fun��o: TSTG -Efetuar os comandos de teste espec�ficos */


/***********************************************************************
*
*  $FC Fun��o: TSTG -Exibir linha hexadecimal
*
*  $ED Descri��o da fun��o
*     Imprime um string de valores hexadecimais a partir de um offseta
*     de um espa�o indicado por ponteiro.
*
***********************************************************************/

   void ImprimirHexadecimal( void * pValorParm ,
                             int    Offset     ,
                             int    TamValor    )
   {

      char * pValor ;
      char   Ch ;
      int    numBytes ;
      int    ValChar ;
      int    i ;

      pValor = ( char * ) pValorParm ;
      numBytes   = TamValor - Offset ;
      if ( numBytes > DIM_LINHA )
      {
         numBytes = DIM_LINHA ;
      } /* if */

      /* Imprime a parte hexadecimal */

         for( i = 0 ; i < numBytes ; i ++ )
         {
            ValChar = *( pValor + Offset + i ) ;
            fprintf( pArqLog , " %02X" , ValChar ) ;
         } /* for */

      /* Imprime espa�os para alinhar a parte ASCII */

         for( ; i < DIM_LINHA ; i ++ )
         {
            fprintf( pArqLog , "   " ) ;
         } /* for */

      /* Imprime a parte ASCII */

         fprintf( pArqLog , "   " ) ;
         for( i = 0 ; i < numBytes ; i ++ )
         {
            Ch = *( pValor + Offset + i ) ;
            if ( ( Ch <   32 )
              || ( Ch == 127 )
              || ( Ch == 255 ))
            {
               Ch = '.' ;
            } /* if */
            fprintf( pArqLog , "%c" , Ch ) ;
         } /* for */

   } /* Fim fun��o: TSTG -Exibir linha hexadecimal */


/***********************************************************************
*
*  $FC Fun��o: TSTG -Exibir resultados do teste
*
*  $ED Descri��o da fun��o
*     Exibe os contadores e a condi��o final dos testes
*
***********************************************************************/

   void ExibirResultados( FILE * pArq )
   {

      /* Exibir contagens */

         fprintf( pArq , "\n\n%s N�mero de linhas lidas:       %5i" ,
                  SINALIZA_COMENTARIO , LER_ObterNumLinhas( ) ) ;
         fprintf( pArq ,   "\n%s N�mero de casos de teste:     %5i" ,
                  SINALIZA_COMENTARIO , ContaCasosTeste ) ;
         fprintf( pArq ,   "\n%s N�mero comandos corretos:     %5i" ,
                  SINALIZA_COMENTARIO , ContaComandosExecutados ) ;
         fprintf( pArq ,   "\n%s N�mero de falhas encontradas: %5i" ,
                  SINALIZA_COMENTARIO , ContaFalhas ) ;

      /* Exibir laudo final */

         if ( ContaFalhas == 0 )
         {
            fprintf( pArq ,   "\n\n%s N�o foram encontrados problemas\n" ,
                     SINALIZA_COMENTARIO ) ;
         } else {
            fprintf( pArq ,   "\n\n%s Foram encontrados problemas\n" ,
                     SINALIZA_COMENTARIO ) ;
         } /* if */

   } /* Fim fun��o: TSTG -Exibir resultados do teste */

/********** Fim do m�dulo de implementa��o: TSTG Controlador de teste gen�rico **********/

