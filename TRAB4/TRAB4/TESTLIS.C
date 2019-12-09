/***************************************************************************
*  $MCI M�dulo de implementa��o: TLIS Teste lista de s�mbolos
*
*  Arquivo gerado:              TestLIS.c
*  Letras identificadoras:      TLIS
*
*  Nome da base de software:    Arcabou�o para a automa��o de testes de programas redigidos em C
*  Arquivo da base de software: D:\AUTOTEST\PROJETOS\LISTA.BSW
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
#include    <stdlib.h>

#include    "TST_ESPC.H"

#include    "GENERICO.H"
#include    "LERPARM.H"

#include    "LISTA.H"


static const char RESET_LISTA_CMD         [ ] = "=resetteste"     ;
static const char CRIAR_LISTA_CMD         [ ] = "=criarlista"     ;
static const char DESTRUIR_LISTA_CMD      [ ] = "=destruirlista"  ;
static const char ESVAZIAR_LISTA_CMD      [ ] = "=esvaziarlista"  ;
static const char INS_ELEM_ANTES_CMD      [ ] = "=inselemantes"   ;
static const char INS_ELEM_APOS_CMD       [ ] = "=inselemapos"    ;
static const char OBTER_VALOR_CMD         [ ] = "=obtervalorelem" ;
static const char EXC_ELEM_CMD            [ ] = "=excluirelem"    ;
static const char IR_PROX				  [ ] = "=irproximo"	  ;
static const char IR_ANTERIOR			  [ ] = "=iranterior"     ;
static const char IR_PRIMEIRO			  [ ] = "=irprimeiro"     ;


#ifdef _DEBUG
static const char DETURPAR			  [ ] = "=deturpa"		  ;
static const char VERIFICAR			  [ ] = "=verifica"		  ;
#endif // _DEBUG



#define TRUE  1
#define FALSE 0

#define VAZIO     0
#define NAO_VAZIO 1

#define DIM_VT_LISTA   10
#define DIM_VALOR     100

LIS_tppCabecaLista vtListas[ DIM_VT_LISTA ] ;

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

   static void DestruirValor( void * pValor ) ;

   static int ValidarInxLista( int inxLista , int Modo ) ;

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/


/***********************************************************************
*
*  $FC Fun��o: TLIS &Testar lista
*
*  $ED Descri��o da fun��o
*     Podem ser criadas at� 10 listas, identificadas pelos �ndices 0 a 10
*
*     Comandos dispon�veis:
*
*     =resetteste
*           - anula o vetor de listas. Provoca vazamento de mem�ria
*     =criarlista                   inxLista
*     =destruirlista                inxLista
*     =esvaziarlista                inxLista
*     =inselemantes                 inxLista  string  CondRetEsp
*     =inselemapos                  inxLista  string  CondRetEsp
*     =obtervalorelem               inxLista  string  CondretPonteiro
*     =excluirelem                  inxLista  CondRetEsp
*     =irproximo						inxLista
*     =iranterior                   inxLista
*
*     =deturpa	                    inxLista  deturpacao
*	  =verifica						inxLista  numFalhas
*
***********************************************************************/

   TST_tpCondRet TST_EfetuarComando( char * ComandoTeste )
   {

      int inxLista  = -1 ,
          numLidos   = -1 ,
          CondRetEsp = -1  ;

      TST_tpCondRet CondRet ;

      char   StringDado[  DIM_VALOR ] ;
      char * pDado ;

      int ValEsp = -1 ;

      int i ;

      int numElem = -1 ;

      StringDado[ 0 ] = 0 ;

      /* Efetuar reset de teste de lista */

         if ( strcmp( ComandoTeste , RESET_LISTA_CMD ) == 0 )
         {

            for( i = 0 ; i < DIM_VT_LISTA ; i++ )
            {
               vtListas[ i ] = NULL ;
            } /* for */

            return TST_CondRetOK ;

         } /* fim ativa: Efetuar reset de teste de lista */

      /* Testar CriarLista */

         else if ( strcmp( ComandoTeste , CRIAR_LISTA_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                       &inxLista ) ;

            if ( ( numLidos != 1 )
              || ( ! ValidarInxLista( inxLista , VAZIO )))
            {
               return TST_CondRetParm ;
            } /* if */

			LIS_criarLista( DestruirValor, &vtListas[inxLista]);

            return TST_CompararPonteiroNulo( 1 , vtListas[ inxLista ] ,
               "Erro em ponteiro de nova lista."  ) ;

         } /* fim ativa: Testar CriarLista */

      /* Testar Destruir lista */

         else if ( strcmp( ComandoTeste , DESTRUIR_LISTA_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "i" ,
                               &inxLista ) ;

            if ( ( numLidos != 1 ) || ( ! ValidarInxLista( inxLista , NAO_VAZIO )))
            {
               return TST_CondRetParm ;
            } /* if */

            LIS_DestruirLista( vtListas[ inxLista ] ) ;
            vtListas[ inxLista ] = NULL ;

            return TST_CondRetOK ;

         } /* fim ativa: Testar Destruir lista */

      /* Testar inserir elemento antes */

         else if ( strcmp( ComandoTeste , INS_ELEM_ANTES_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "isi" , &inxLista , StringDado , &CondRetEsp ) ;

            if ( ( numLidos != 3 ) || ( ! ValidarInxLista( inxLista , NAO_VAZIO )) )
            {
               return TST_CondRetParm ;
            } /* if */

            pDado = ( char * ) malloc( strlen( StringDado ) + 1 ) ;
            if ( pDado == NULL )
            {
               return TST_CondRetMemoria ;
            } /* if */

            strcpy( pDado , StringDado ) ;


            CondRet = (TST_tpCondRet) LIS_InserirNoAntes( vtListas[ inxLista ] , pDado ) ;

            if ( CondRet != LIS_CondRetOK )
            {
               free( pDado ) ;
            } /* if */

            return TST_CompararInt(CondRetEsp , CondRet , "Condicao de retorno errada ao inserir antes.") ;

         } /* fim ativa: Testar inserir elemento antes */

      /* Testar inserir elemento apos */

         else if (strcmp(ComandoTeste , INS_ELEM_APOS_CMD) == 0 )
         {

            numLidos = LER_LerParametros( "isi" ,
                       &inxLista , StringDado , &CondRetEsp ) ;

            if ( ( numLidos != 3 )
              || ( ! ValidarInxLista( inxLista , NAO_VAZIO )) )
            {
               return TST_CondRetParm ;
            } /* if */

            pDado = ( char * ) malloc( strlen( StringDado ) + 1 ) ;
            if ( pDado == NULL )
            {
               return TST_CondRetMemoria ;
            } /* if */

            strcpy( pDado , StringDado ) ;


            CondRet = (TST_tpCondRet) LIS_InserirNoApos( vtListas[ inxLista ] , pDado ) ;

            if ( CondRet != LIS_CondRetOK )
            {
               free( pDado ) ;
            } /* if */

            return TST_CompararInt( CondRetEsp , CondRet ,
                     "Condicao de retorno errada ao inserir apos."                   ) ;

         } /* fim ativa: Testar inserir elemento apos */

      /* Testar excluir simbolo */

         else if ( strcmp( ComandoTeste , EXC_ELEM_CMD ) == 0 )
         {

            numLidos = LER_LerParametros( "ii" ,
                  &inxLista , &CondRetEsp ) ;

            if ( ( numLidos != 2 )
              || ( ! ValidarInxLista( inxLista , NAO_VAZIO )) )
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararInt( CondRetEsp ,
						LIS_ExcluirNo( vtListas[ inxLista ] ) ,
                     "Condição de retorno errada ao excluir."   ) ;

         } /* fim ativa: Testar excluir simbolo */

      /* Testar obter valor do elemento corrente */

         else if ( strcmp( ComandoTeste , OBTER_VALOR_CMD ) == 0 )
         {
            numLidos = LER_LerParametros( "isi" ,
                       &inxLista , StringDado , &CondRetEsp ) ;

            if ( ( numLidos != 3 )
              || ( ! ValidarInxLista( inxLista , NAO_VAZIO )) )
            {
               return TST_CondRetParm ;
            } /* if */

			TST_tpCondRet condRetObtida = (TST_tpCondRet) LIS_obterConteudo(vtListas[inxLista], (void**) &pDado);

			if (pDado != NULL) {
				CondRet = TST_CompararString(StringDado, pDado, "Valor obtido não bate com o esperado");

				if (CondRet != TST_CondRetOK)
					return CondRet;
			}

			return TST_CompararInt(CondRetEsp, condRetObtida, "Retorno errado ao obter conteúdo do nó.");

         } /* fim ativa: Testar obter valor do elemento corrente */

      /* LIS  &Avan�ar elemento */

         else if ( strcmp( ComandoTeste , IR_PROX ) == 0 )
         {

            numLidos = LER_LerParametros( "ii" , &inxLista  ,
                                &CondRetEsp ) ;

            if ( ( numLidos != 2 )
              || ( ! ValidarInxLista( inxLista , NAO_VAZIO )) )
            {
               return TST_CondRetParm ;
            } /* if */

            return TST_CompararInt( CondRetEsp ,
				LIS_vaiParaProximoNo( vtListas[ inxLista ] ) ,
                      "Condicao de retorno errada ao ir para o proximo nó" ) ;

         } /* fim ativa: LIS  &Avan�ar elemento */

		   /* LIS Voltar elemento */

		 else if (strcmp(ComandoTeste, IR_ANTERIOR) == 0)
		 {

			 numLidos = LER_LerParametros("ii", &inxLista,
				 &CondRetEsp);

			 if ((numLidos != 2)
				 || (!ValidarInxLista(inxLista, NAO_VAZIO)))
			 {
				 return TST_CondRetParm;
			 } /* if */

			 return TST_CompararInt(CondRetEsp,
				 LIS_vaiParaNoAnterior(vtListas[inxLista]),
				 "Condicao de retorno errada ao ir para o nó anterior");

		 } /* fim ativa: LIS  Voltar elemnento */

         /* LIS Ir para primeiro nó */

        else if (strcmp(ComandoTeste, IR_PRIMEIRO) == 0)
		 {

			 numLidos = LER_LerParametros("ii", &inxLista,
				 &CondRetEsp);

			 if ((numLidos != 2)
				 || (!ValidarInxLista(inxLista, NAO_VAZIO)))
			 {
				 return TST_CondRetParm;
			 } /* if */

			 return TST_CompararInt(CondRetEsp,
				 LIS_vaiParaPrimeiroNo(vtListas[inxLista]),
				 "Condicao de retorno errada ao ir para primeiro nó");

		 } /* fim ativa: LIS  Voltar elemnento */

#ifdef _DEBUG
		   /* LIS  Deturpa estrutura */

		 else if (strcmp(ComandoTeste, DETURPAR) == 0)
		 {

			 numLidos = LER_LerParametros("ii", &inxLista, &i);

			 if ((numLidos != 2)
				 || (!ValidarInxLista(inxLista, NAO_VAZIO)))
			 {
				 return TST_CondRetParm;
			 } /* if */
			 LIS_deturpador(vtListas[inxLista], i);

			 return TST_CondRetOK;
		 } /* fim ativa: LIS  Deturpa estrutura */

		   /* LIS Verifica estrutura */

		 else if (strcmp(ComandoTeste, VERIFICAR) == 0)
		 {

			 numLidos = LER_LerParametros("ii", &inxLista,
				 &ValEsp);

			 if ((numLidos != 2)
				 || (!ValidarInxLista(inxLista, NAO_VAZIO)))
			 {
				 return TST_CondRetParm;
			 } /* if */

			 return TST_CompararInt(ValEsp,
				 LIS_verificador(vtListas[inxLista]),
				 "Numero de falhas obtido diferente do esperado");

		 } /* fim ativa: LIS  Verifica estrutura */
#endif // _DEBUG


      return TST_CondRetNaoConhec ;

   } /* Fim fun��o: TLIS &Testar lista */


/*****  C�digo das fun��es encapsuladas no m�dulo  *****/


/***********************************************************************
*
*  $FC Fun��o: TLIS -Destruir valor
*
***********************************************************************/

   void DestruirValor( void * pValor )
   {

      free( pValor ) ;

   } /* Fim fun��o: TLIS -Destruir valor */


/***********************************************************************
*
*  $FC Fun��o: TLIS -Validar indice de lista
*
***********************************************************************/

   int ValidarInxLista( int inxLista , int Modo )
   {

      if ( ( inxLista <  0 )
        || ( inxLista >= DIM_VT_LISTA ))
      {
         return FALSE ;
      } /* if */
         
      if ( Modo == VAZIO )
      {
         if ( vtListas[ inxLista ] != 0 )
         {
            return FALSE ;
         } /* if */
      } else
      {
         if ( vtListas[ inxLista ] == 0 )
         {
            return FALSE ;
         } /* if */
      } /* if */
         
      return TRUE ;

   } /* Fim fun��o: TLIS -Validar indice de lista */

/********** Fim do m�dulo de implementa��o: TLIS Teste lista de s�mbolos **********/

