/***************************************************************************
*  $MCI M�dulo de implementa��o: PRNC M�dulo principal
*
*  Arquivo gerado:              PRINCIPc
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
*  $ED Descri��o do m�dulo
*     Este m�dulo cont�m o programa principal do arcabou�o ("framework")
*     de apoio ao teste automatizado de m�dulos escritos em C.
*
***************************************************************************/

#include   <stdio.h>
#include   <string.h>

#include   "GENERICO.H"

#define  DIM_PARM_BUFFER   250
#define  DIM_NOME_ARQUIVO  250

      static char NomeArqScript[ DIM_NOME_ARQUIVO ] = "" ;
            /* Nome do arquivo script linha de comando */

      static char NomeArqLog[ DIM_NOME_ARQUIVO ] = "" ;
            /* Nome do arquivo log linha de comando */

      static char NomeArqAcum[ DIM_NOME_ARQUIVO ] = "" ;
            /* Nome do arquivo de acumula��o */

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/


/***********************************************************************
*
*  $FC Fun��o: PRNC &Programa principal
*
*  $ED Descri��o da fun��o
*
*     O programa principal pode receber os per�metros de linha de comando:
*
*     /s<arqscript> - obrigat�rio, � o nome do arquivo contendo o
*                     script de teste. A sintaxe do script depende
*                     do m�dulo espec�fico sendo testado.
*                     Veja especifica��o do m�dulo GENERICO para saber
*                     a extens�o default de arquivos script.
*     /l<arqlog>    - opcional, � o nome do arquivo log no qual devem
*                     ser escritas todas as mensagens geradas durante
*                     o teste. Se este nome de arquivo n�o for
*                     fornecido a sa�da ser� dirigida para a tela
*                     (arquivo stdout).
*                     Veja especifica��o do m�dulo GENERICO para saber
*                     a extens�o default de arquivos log.
*     /a<arqacum>   - opcional, se fornecido as estat�sticas da execu��o
*                     do programa ser�o adicionadas ao conte�do do arquivo
*                     arqacum. Caso este n�o exista, ser� criado.
*                     A extens�o default do arquivo � .estat
*                     Use este par�metro em batches de teste envolvendo um
*                     conjunto de massas de teste. O total acumulado
*                     pode ser exibido com o programa exbestat.exe
*     /h ou /?      - opcional, se fornecido imprime, � guisa de aux�lio,
*                     um resumo de como usar o programa e, ap�s, p�ra.
*                     Este aux�lio tamb�m � exibido no caso de erro de
*                     par�metro de linha de comando.
*
*     O programa retorna:
*
*     - 0 caso o teste tenha conclu�do corretamente
*     - 2 caso tenha sido solicitado aux�lio.
*     - 4 caso:
*           - os parâmetros de linha de comando estejam em erro
*           - um ou mais casos de teste identificaram alguma falha.
*
*  $EP parâmetros
*     $P NumParms     - n�mero de parâmetros de linha de comando
*                       recebidos
*     $P vtValorParms - vetor de strings, cada item do vetor
*                       corresponde ao string de um par�metro.
*                       O string de �ndice zero � o nome do programa.
*
***********************************************************************/

   int main( int NumParm , char ** vtValorParm )
   {

      int  ScriptDefinido ,
           LogDefinido    ,
           Erro           ;

      char ParmBuffer[ DIM_PARM_BUFFER ] ;

      int inxParm ;

      /* Exibir logo do programa */

         printf( "\n\nLES - Laboratório de Engenharia de Software DI/PUC-Rio" ) ;
         printf( "\n      Arcabouço de teste de módulos C  (c) 2006\n" ) ;
         printf( "\n---------------------------------------------" ) ;
         printf( "\n      Construto sob teste: %s\n" , vtValorParm[ 0 ] ) ;

      /* Inicializar contexto */

         ScriptDefinido = 0 ;
         LogDefinido    = 0 ;
         Erro           = 0 ;

      /* Ler os parâmetros da linha de comando */

         for ( inxParm = 1 ; inxParm < NumParm ; inxParm ++ ) {

         /* Tratar par�metro sintaticamente correto */

            strcpy( ParmBuffer , vtValorParm[ inxParm ] ) ;

            if ( ( ParmBuffer[ 0 ] == '/' )
              || ( ParmBuffer[ 0 ] == '-' ))
            {

               switch ( ParmBuffer[ 1 ] ) {

               /* Tratar par�metro nome de arquivo script de teste */

                  case 's' :
                  case 'S' :
                  {

                     if ( ! ScriptDefinido )
                     {
                        ScriptDefinido = 1 ;
                        strcpy( NomeArqScript , ParmBuffer + 2 ) ;
                     } else
                     {
                        printf( "\n>>>   Arquivo de script j� foi definido." ) ;
                        Erro = 4 ;
                     } /* if */

                     break ;

                  } /* fim ativa: Tratar par�metro nome de arquivo script de teste */

               /* Tratar par�metro nomde de arquivo log de mensagens */

                  case 'l' :
                  case 'L' :
                  {

                     if ( ! LogDefinido )
                     {
                        LogDefinido = 1 ;
                        strcpy( NomeArqLog , ParmBuffer + 2 ) ;

                     } else {
                        printf( "\n>>>   Arquivo log já foi definido." ) ;
                        Erro = 4 ;
                     } /* if */

                     break ;

                  } /* fim ativa: Tratar par�metro nomde de arquivo log de mensagens */

               /* Tratar par�metro arquivo de acumula��o */

                  case 'a':
                  case 'A':
                  {

                     if ( NomeArqAcum[ 0 ] == 0 )
                     {
                        strcpy( NomeArqAcum , ParmBuffer + 2 ) ;
                     } else {
                        printf( "\n>>>   Arquivo de acumulação já foi definido." ) ;
                        Erro = 4 ;
                     } /* if */

                     break ;

                  } /* fim ativa: Tratar par�metro arquivo de acumula��o */

               /* Tratar solicita��o de aux�lio */

                  case '?' :
                  case 'h' :
                  case 'H' :
                  {

                     Erro = 1 ;

                     break ;

                  } /* fim ativa: Tratar solicita��o de aux�lio */

               /* Tratar par�metro ilegal */

                  default :
                  {

                     printf( "\n>>>   Parâmetro desconhecido: %s" , ParmBuffer ) ;
                     Erro = 4 ;

                     break ;

                  } /* fim ativa: Tratar par�metro ilegal */

               } /* fim seleciona: Tratar par�metro sintaticamente correto */

            } /* fim ativa: Tratar par�metro sintaticamente correto */

         /* Tratar par�metro sintaticamente errado. */

            else
            {

               printf( "\n>>>   Sintaxe de parâmetro errada: %s" , ParmBuffer ) ;
               Erro = 4 ;

            } /* fim ativa: Tratar par�metro sintaticamente errado. */

         } /* fim repete: Ler os parâmetros da linha de comando */

      /* Controlar parâmetros */

         /* Validar parâmetros */

            if ( ( Erro != 1       )
              && ( !ScriptDefinido ))
            {
               printf( "\n>>>   Faltou arquivo de diretivas de teste" ) ;
               Erro = 4 ;
            } /* if */

         /* Terminar em caso de erro */

            if ( Erro != 0 )
            {

               printf( "\n\n!!!  Modo de uso do programa" ) ;
               printf(   "\n  TestXXX   /s<ArqScript> {/l<ArqLog>} {/a<ArqAcum> {/? | /h }" ) ;
               printf( "\n\n  <ArqScript>   - nome do arquivo contendo o script de teste. " ) ;
               printf(   "\n                  Parâmetro obrigatório, exceto se solicitado auxílio." ) ;
               printf(   "\n  <ArqLog>      - nome do arquivo contendo o script de teste. " ) ;
               printf(   "\n                  Parâmetro opcional. Se não fornecido exibe" ) ;
               printf(   "\n                  o resultado do teste na tela (stdout)" ) ;
               printf(   "\n  <ArqAcum>     - nome do arquivo acumulador." ) ;
               printf(   "\n                  Parâmetro opcional. Se fornecido as" ) ;
               printf(   "\n                  estatísticas do teste serão acumuladas" ) ;
               printf(   "\n                  no arquivo ArqAcum. Caso não exista," ) ;
               printf(   "\n                  será criado." ) ;
               printf(   "\n  /? ou /h      - Solicitação de auxílio." ) ;

               if ( Erro == 1 )
               {
                  Erro = 2 ;
               } else
               {
                  Erro = 4 ;
               } /* if */
               printf( "\n\n>>>  Teste não executado\n" ) ;

               return Erro ;

            } /* fim ativa: Terminar em caso de erro */

      /* Executar os testes */

         if ( TST_ControlarSessaoTeste( vtValorParm[ 0 ] , NomeArqScript ,
                                        NomeArqLog       , NomeArqAcum    ) != 0 )
         {
            return 4 ;
         } /* if */

         return 0 ;

   } /* Fim função: PRNC &Programa principal */

/********** Fim do módulo de implementação: PRNC Módulo principal **********/

