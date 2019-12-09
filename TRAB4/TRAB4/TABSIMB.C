/***************************************************************************
*  $MCI M�dulo de implementa��o: TBS  Tabela de s�mbolos
*
*  Arquivo gerado:              TABSIMB.c
*  Letras identificadoras:      TBS
*
*  Nome da base de software:    Arcabou�o para a automa��o de testes de programas redigidos em C
*  Arquivo da base de software: D:\AUTOTEST\PROJETOS\ARCABOUC.BSW
*
*  Projeto: INF 1301 / 1628 Automatiza��o dos testes de m�dulos C
*  Gestor:  LES/DI/PUC-Rio
*  Autores: avs
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*     5       avs   18/mai/2008 corrigir e ampliar CESPDIN
*     4       avs   01/fev/2006 criar linguagem script simb�lica
*     3       avs   08/dez/2004 uniformiza��o dos exemplos
*     2       avs   07/jul/2003 unifica��o de todos os m�dulos em um s� projeto
*     1       avs   16/abr/2003 in�cio desenvolvimento
*
***************************************************************************/

#include   <stdio.h>
#include   <string.h>
#include   <memory.h>
#include   <malloc.h>

#define TABSIMB_OWN
#include "TABSIMB.H"
#undef TABSIMB_OWN

#ifdef _DEBUG
   #include   "CESPDIN.H"
#endif

/***********************************************************************
*
*  $TC Tipo de dados: TBS Elemento de lista de colis�o
*
*
***********************************************************************/

   typedef struct tagLista {

         void * pDado ;
               /* Ponteiro para o dado contido no elemento */

         struct tagLista * pAnt ;
               /* Ponteiro para elemento predecessor */

         struct tagLista * pProx ;
               /* Ponteiro para lemento sucessor */

   } tpLista ;

/***********************************************************************
*
*  $TC Tipo de dados: TBS Tipo cabe�a de tabela
*
*
***********************************************************************/

   typedef struct TBS_tagTabela {

         unsigned tamVtHash ;
               /* Tamanho do vetor de randomiza��o */

         tpLista ** pVtHash ;
               /* Ponteiro para o vetor de randomiza��o
               *
               *$ED Descri��o
               *   Este ponteiro aponta para um vetor alocado dinamicamente.
               *   Vetores s�o tratados como ponteiros, donde surge o ponteiro para
               *   ponteiro ( "**" ) na declara��o. */

         char * ( * ObterSimbolo) ( void * pDado ) ;
               /* Ponteiro para fun��o obter s�mbolo */

         void ( * LiberarDado ) ( void * pDado ) ;
               /* Ponteiro para a fun��o de destrui��o da estrutura de dados que cont�m o s�mbolo
               *
               *$ED Descri��o
               *   Caso elemento da tabela de s�mbolos contenha um ou mais ponteiros
               *   para espa�os que devem ser desalocados quando for desalocado o
               *   elemento, este atributo deve conter o ponteiro para a fun��o que
               *   realiza o free destes espa�os. */

   } TBS_tpTabela ;

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

   static void LiberarElemento( TBS_tpTabela * pTabela ,
                                tpLista      * pElem    ) ;

   static tpLista * ProcurarSimbolo( TBS_tpTabela * pTabela  ,
                                     char         * pSimbolo ,
                                     unsigned     * inxHash   ) ;

   static unsigned Hash( char * pSimbolo , unsigned tamVtHash ) ;

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***************************************************************************
*
*  Fun��o: TBS  &Criar tabela de s�mbolos
*  ****/

   TBS_tppTabela TBS_CriarTabela(
             int tamVetor                               ,
             char * ( * ObterSimbolo ) ( void * pDado ) ,
             void   ( * LiberarDado  ) ( void * pDado )  )
   {

      TBS_tpTabela * pTabela = NULL ;
      int i ;

      #ifdef _DEBUG
         TST_ASSERT( tamVetor      > 1    ) ;
         TST_ASSERT( ObterSimbolo != NULL ) ;
      #endif

      pTabela = ( TBS_tpTabela * ) malloc( sizeof( TBS_tpTabela )) ;
      if ( pTabela == NULL )
      {
         return NULL ;
      } /* if */

      pTabela->pVtHash = ( tpLista ** ) malloc( tamVetor * sizeof( tpLista * )) ;
      if ( pTabela->pVtHash == NULL )
      {
         free( pTabela ) ;
         return NULL ;
      } /* if */

      for( i = 0 ; i < tamVetor ; i++ )
      {
         pTabela->pVtHash[ i ] = NULL ;
      } /* for */

      pTabela->tamVtHash    = tamVetor ;
      pTabela->ObterSimbolo = ObterSimbolo ;
      pTabela->LiberarDado  = LiberarDado ;

      return pTabela ;

   } /* Fim fun��o: TBS  &Criar tabela de s�mbolos */

/***************************************************************************
*
*  Fun��o: TBS  &Destruir tabela de s�mbolos
*  ****/

   void TBS_DestruirTabela( TBS_tppTabela pTabela )
   {

      unsigned inxElem ;

      tpLista * pElem ;
      tpLista * pProx ;

      #ifdef _DEBUG
         TST_ASSERT( pTabela != NULL ) ;
      #endif

      for ( inxElem = 0 ; inxElem < pTabela->tamVtHash ; inxElem++ ) {

      /* Destruir todos elementos de lista de colis�o */

         pElem = pTabela->pVtHash[ inxElem ] ;
         while ( pElem != NULL )
         {
            pProx = pElem->pProx ;
            LiberarElemento( pTabela , pElem ) ;
            pElem = pProx ;
         } /* while */

      } /* fim repete: Raiz de TBS  &Destruir tabela de s�mbolos */

      free( pTabela->pVtHash ) ;
      free( pTabela ) ;

   } /* Fim fun��o: TBS  &Destruir tabela de s�mbolos */

#ifdef _DEBUG

/***************************************************************************
*
*  Fun��o: TBS  &Validar tabela de s�mbolos
*  ****/

   TST_tpCondRet TBS_ValidarTabela( TBS_tppTabela pTabela )
   {

      unsigned inxHash ;

      tpLista * pElem ;

      #ifdef _DEBUG
         TST_ASSERT( pTabela != NULL ) ;
      #endif

      /* Validar exist�ncia de dados da cabe�a */

         if ( pTabela->tamVtHash <= 1 )
         {
            return TST_NotificarFalha( "Tamanho incorreto do vetor de randomiza��o." ) ;
         } /* if */

         if ( pTabela->pVtHash == NULL )
         {
            return TST_NotificarFalha( "Falta vetor de randomiza��o." ) ;
         } /* if */

         if ( pTabela->ObterSimbolo == NULL )
         {
            return TST_NotificarFalha( "Falta fun��o obter simbolo." ) ;
         } /* if */

      /* Validar listas de colis�o */

         for ( inxHash = 0 ; inxHash < pTabela->tamVtHash ; inxHash ++ ) {

         /* Validar toda a lista de colis�o */

            pElem = pTabela->pVtHash[ inxHash ] ;

            while ( pElem != NULL ) {

            /* Validar elemento da lista de colis�o */

               if ( pElem->pDado == NULL )
               {
                  return TST_NotificarFalha( "Faltou dado em elemento de lista." ) ;
               } /* if */

               if ( Hash( pTabela->ObterSimbolo( pElem->pDado ) ,
                              pTabela->tamVtHash ) != inxHash )
               {
                  return TST_NotificarFalha( "�ndice has de elemento est� incorreto." ) ;
               } /* if */

               if ( pElem->pAnt != NULL )
               {
                  if ( pElem->pAnt->pProx != pElem )
                  {
                     return TST_NotificarFalha( "Erro de encadeamento � esquerda em elemento de lista." ) ;
                  } /* if */
               } else
               {
                  if ( pElem != pTabela->pVtHash[ inxHash ] )
                  {
                     return TST_NotificarFalha( "Erro de encadeamento origem em elemento de lista." ) ;
                  } /* if */
               } /* if */

               if ( pElem->pProx != NULL )
               {
                  if ( pElem->pProx->pAnt != pElem )
                  {
                     return TST_NotificarFalha( "Erro de encadeamento � direita em elemento de lista." ) ;
                  } /* if */
               } /* if */

               pElem = pElem->pProx ;

            } /* fim repete: Validar toda a lista de colis�o */

         } /* fim repete: Validar listas de colis�o */

      return TST_CondRetOK ;

   } /* Fim fun��o: TBS  &Validar tabela de s�mbolos */

#endif

/***************************************************************************
*
*  Fun��o: TBS  &Inserir s�mbolo
*  ****/

   TBS_tpCondRet TBS_InserirSimbolo( TBS_tppTabela pTabela ,
                                     void * pDado           )
   {

      unsigned inxHash ;

      tpLista * pElem ;

      #ifdef _DEBUG
         TST_ASSERT( pTabela != NULL ) ;
         TST_ASSERT( pDado   != NULL ) ;
      #endif

      /* Procurar s�mbolo */

         if ( ProcurarSimbolo( pTabela ,
                               pTabela->ObterSimbolo( pDado ) ,
                               &inxHash ) != NULL )
         {
            return TBS_CondRetSimboloExiste ;
         } /* if */

      /* Inserir o s�mbolo */

         pElem = ( tpLista * ) malloc( sizeof( tpLista )) ;
         pElem->pDado = pDado ;
         pElem->pAnt  = NULL ;
         pElem->pProx = pTabela->pVtHash[ inxHash ] ;
         if ( pElem->pProx != NULL )
         {
            pElem->pProx->pAnt = pElem ;
         } /* if */

         pTabela->pVtHash[ inxHash ] = pElem ;

         return TBS_CondRetOK ;

   } /* Fim fun��o: TBS  &Inserir s�mbolo */

/***************************************************************************
*
*  Fun��o: TBS  &Procurar s�mbolo
*  ****/

   void * TBS_ProcurarSimbolo( TBS_tppTabela pTabela ,
                               char * pSimbolo         )
   {

      tpLista * pElem ;
      unsigned inxHash ;

      #ifdef _DEBUG
         TST_ASSERT( pTabela  != NULL ) ;
         TST_ASSERT( pSimbolo != NULL ) ;
      #endif

      pElem = ProcurarSimbolo( pTabela , pSimbolo , &inxHash ) ;
      if ( pElem == NULL )
      {
         return NULL ;
      } /* if */

      return pElem->pDado ;

   } /* Fim fun��o: TBS  &Procurar s�mbolo */

/***************************************************************************
*
*  Fun��o: TBS  &Excluir s�mbolo
*  ****/

   TBS_tpCondRet TBS_ExcluirSimbolo( TBS_tppTabela pTabela ,
                                     char * pSimbolo        )
   {

      unsigned inxHash ;
      tpLista * pElem ;

      #ifdef _DEBUG
         TST_ASSERT( pTabela  != NULL ) ;
         TST_ASSERT( pSimbolo != NULL ) ;
      #endif

      pElem = ProcurarSimbolo( pTabela , pSimbolo , &inxHash ) ;
      if ( pElem == NULL )
      {
         return TBS_CondRetSimboloNaoExiste ;
      } /* if */

      if ( pElem->pProx != NULL )
      {
         pElem->pProx->pAnt = pElem->pAnt ;
      } /* if */

      if ( pElem->pAnt != NULL )
      {
         pElem->pAnt->pProx = pElem->pProx ;
      } else {
         pTabela->pVtHash[ inxHash ] = pElem->pProx ;
      } /* if */

      LiberarElemento( pTabela , pElem ) ;

      return TBS_CondRetOK ;

   } /* Fim fun��o: TBS  &Excluir s�mbolo */


/*****  C�digo das fun��es encapsuladas no m�dulo  *****/


/***********************************************************************
*
*  $FC Fun��o: TBS  -Liberar elemento da tabela
*
*  $ED Descri��o da fun��o
*     Elimina os espa�os apontados pelo valor do elemento, o valor e o
*     pr�prio elemento.
*
***********************************************************************/

   void LiberarElemento( TBS_tpTabela * pTabela ,
                         tpLista      * pElem    )
   {

      if ( pTabela->LiberarDado != NULL )
      {
         pTabela->LiberarDado( pElem->pDado ) ;
      } /* if */
      free( pElem->pDado ) ;
      free( pElem ) ;

   } /* Fim fun��o: TBS  -Liberar elemento da tabela */


/***********************************************************************
*
*  $FC Fun��o: TBS  -Procurar o s�mbolo
*
***********************************************************************/

   tpLista * ProcurarSimbolo( TBS_tpTabela * pTabela  ,
                              char         * pSimbolo ,
                              unsigned     * inxHash   )
   {

      tpLista * pElem ;

      *inxHash = Hash( pSimbolo , pTabela->tamVtHash ) ;
      pElem    = pTabela->pVtHash[ *inxHash ] ;
      while ( pElem != NULL )
      {
         if ( strcmp( pTabela->ObterSimbolo( pElem->pDado ) ,
                      pSimbolo ) == 0 )
         {
            return pElem ;
         } /* if */
         pElem = pElem->pProx ;
      } /* while */

      return NULL ;

   } /* Fim fun��o: TBS  -Procurar o s�mbolo */


/***********************************************************************
*
*  $FC Fun��o: TBS  -Computar hash
*
***********************************************************************/

   unsigned Hash( char * pSimbolo , unsigned tamVtHash )
   {

      unsigned inxHash = 0 ;
      unsigned i = 0 ;

      #ifdef _DEBUG
         TST_ASSERT( pSimbolo != NULL ) ;
      #endif

      for( i = 0 ; i < strlen( pSimbolo ) ; i ++ )
      {
         inxHash = ( inxHash << 2 ) + pSimbolo[ i ] ;
      } /* for */

      return inxHash % tamVtHash ;

   } /* Fim fun��o: TBS  -Computar hash */

/********** Fim do m�dulo de implementa��o: TBS  Tabela de s�mbolos **********/

