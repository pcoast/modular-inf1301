/***************************************************************************
*  $MCI M�dulo de implementa��o: CED  Controlador de espa�os de dados alocados dinamicamente
*
*  Arquivo gerado:              CESPDIN.c
*  Letras identificadoras:      CED
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

#include    <malloc.h>
#include    <stdio.h>
#include    <memory.h>
#include    <string.h>

#define CESPDIN_OWN
#include "CESPDIN.H"
#undef CESPDIN_OWN

#include    "GENERICO.H"
#include    "GERAALT.H"

#include    "Tabelas/IdTiposEspaco.def"

    /* O pragma � necess�rio para assegurar alinhamento a byte,
       uma vez que toda a aritm�tica com ponteiros assume isso */

#pragma pack (1)

#define     DIM_NOME_ARQ_FONTE  32
#define     DIM_PONTEIRO         4

#define     TIPO_MSG_INFO     "!!!"
#define     TIPO_MSG_ERRO     "ed>"
#define     TIPO_MSG_VAZIA    "   "

#define     CHAR_ALOC      '|'
#define     CHAR_DESALOC   '?'

#define     LIMITE_FREQUENCIA      1000
#define     LIMITE_INF_FREQUENCIA    10
#define     LIMITE_SUP_FREQUENCIA  1000

/* Controle do conte�do do espa�o alocado
*
* Ao serem alocados, a parte Valor dos espa�os � inicializados para
* CHAR_ALOC ("|"). Ao serem desalocados, os espa�o todo � preenchido
* com CHAR_DESALOC ("?"). Isto permite verificar o estado do espa�o
* ao acess�-lo. O objetivo � provocar erros sistem�ticos caso um
* programa tente utilizar espa�os desalocados, ou se esque�a de
* inicializar campos que contenha ponteiros.
*/

#define     DIM_VALOR      2
#define     DIM_CONTROLE   2
#define     CONTROLE       "\x96\xC3"
#define     CONTROLE_ERRO  "\x02\0"

/* Todos os espa�os s�o antecedidos e sucedidos por strings constantes
* de controle (CONTROLE). Isto permite verificar se houve extravas�o
* ao transferir dados para o espa�o.
* O CONTROLE_ERRO � inserido ap�s quando o espa�o for liberado.
* Procura evitar que strings possam extravasar o espa�o dispon�vel.
*/

#define     ESPACO_NULL        0x98FB5D32L

/* Ao dar free o primeiro long � tornado igual a zero
*  Este valor indica que ainda n�o ocorreu um free no espa�o
*/

#define     IMPRESSAO_DIGITAL  0xC07146AEL

/* Todos os espacos de dados teem esta marca digital.
* Isto permite assegurar que se trata de um espa�o controlado.
* O que, por sua vez permite misturar m�dulos instrumentados
* com m�dulos n�o instrumentados sem que ocorram erros
* de processamento que acabam levando ao cancelamento do programa
*/

/***********************************************************************
*
*  $TC Tipo de dados: CED Elemento da lista de controle de espa�os de dados
*
*
*  $ED Descri��o do tipo
*     Esta estrutura define a organiza��o de um espa�o de dados
*     alocado pelo controlador de vazamento.
*     O elemento prev� campos para a identifica��o do tipo do espa�o de
*     dados e para o controle de extravas�o dos limites estabelecidos
*     para o espa�o de dados �til.
*
***********************************************************************/

   typedef struct tgElemListaControle {

         unsigned long EspacoNULL ;
               /* Espa�o para o NULL */

         unsigned long ImpressaoDigital ;
               /* Marcador de espa�o controlado
               *
               *$ED Descri��o
               *   O valor contido neste marcador � sempre o mesmo.
               *   Tem por objetivo servir de um identificador de que o espa�o de dados
               *   corresponde a um espa�o controlado.
               *   Caso n�o exista as opera��es de manipula��o de espa�o controlado s�o
               *   inibidas. Isto � particularmente importante para o free,
               *   uma vez que o espa�o a ser liberado poderia ter sido criado
               *   sem em um m�dulo que n�o aloca de forma controlada. */

         int tamTotal ;
               /* Tamanho total do espa�o
               *
               *$ED Descri��o
               *   Tem por objetivo estabelecer uma redund�ncia com tamValor. */

         struct tgElemListaControle * pAnt ;
               /* Ponteiro para o elemento anterior na lista de espa�os alocados */

         struct tgElemListaControle * pProx ;
               /* Ponteiro para o elemento a seguir na lista de espa�os alocados */

         int idEspaco ;
               /* Identificador do espa�o
               *
               *$ED Descri��o
               *   Identificador �nico do espa�o alocado */

         int ehAtivo ;
               /* Marca de espa�o ativo
               *
               *$ED Descri��o
               *   Indica se o espa�o � ou n�o ativo. A marca � estabelecida pelo usu�rio. */

         int idTipoValor ;
               /* Identificador do tipo do valor contido no espa�o alocado
               *
               *$ED Descri��o
               *   O tipo espec�fico do valor contido neste elemento pode ser identificado
               *   por este atributo. Ao instrumentar um ou mais m�dulos, o programador
               *   cliente deve definir um inteiro que esteja em correspond�ncia
               *   um para um com os diversos tipos de valores que ser�o alocados
               *   dinamicamente. Ap�s criar o elemento deve ser atribu�do o
               *   identificador do tipo do valor que ser� inserido no elemento. */

         int tamValor ;
               /* Dimens�o, em bytes, do valor contido no espa�o alocado */

         int numLinhaFonte ;
               /* N�mero da linha do c�digo fonte */

         char NomeArquivoFonte[ DIM_NOME_ARQ_FONTE ] ;
               /* Nome arquivo fonte
               *
               *$ED Descri��o
               *   Os atributos NumLinhaFonte e NomeArquivoFonte identificam o local
               *   no c�digo fonte na qual a fun��o CED_malloc foi chamada.
               *   Isto ajuda a identificar a causa dos poss�veis problemas de manipula��o
               *   de mem�ria din�mica encontrados ao executar o m�dulo controlado. */

         char ControleAntes[ DIM_CONTROLE ] ;
               /* Marcador de controle de extravas�o antes do espa�o */

         char Valor[ DIM_VALOR ] ;
               /* Dummy a ser substitu�do pelo espa�o a ser ocupado pelo valor */

         char ControleApos[  DIM_CONTROLE ] ;
               /* Marcador de controle de extravas�o ap�s
               *
               *$ED Descri��o
               *   A origem deste atributo varia em fun��o da dimens�o do valor
               *   contido no elemento. Ele � declarado somente para que se
               *   possa determinar o sizeof do elemento sem contar com o
               *   espa�o requerido pelo valor. */

   } tpElemListaControle ;

/*****  Dados encapsulados no m�dulo  *****/

      #define  TRUE     1
      #define  FALSE    0
            /* Constantes l�gicas */

      static int estaInicializado = FALSE ;
            /* Controle de inicializa��o */

      static tpElemListaControle * pOrgLista = NULL;
            /* Origem da lista de espa�os alocados */

      static tpElemListaControle * pEspacoCorr = NULL;
            /* Iterador: espa�o corrente
               *
               *$ED Descri��o
               *   Aponta para o espa�o corrente do iterador.
               *   Se NULL o iterador n�o est� ativo. */

      static long numEspacosAlocados = 0 ;
            /* Contador de espa�os alocados */

      static long TotalAlocacoes = 0 ;
            /* Contador do total de chamadas de aloca��o */

      static long EspacoTotalAlocado = 0 ;
            /* Espa�o total alocado */

      static long LimiteMemoria = 0 ;
            /* Limite de mem�ria dispon�vel
               *
               *$ED Descri��o
               *   Caso este valor seja maior do que zero, este valor restringe
               *   o total de mem�ria que pode estar alocado a cada momento. */

      static long LimiteNumEspacos = 0 ;
            /* Limite de n�mero de espa�os alocados */

      static int FrequenciaMalloc ;
            /* Limite de freq��ncia estabelecido
               *
               *$ED Descri��o
               *   Determina a freq��ncia de alloca��es que ser�o realizadas.
               *   Deixar�o de ser realizadas aloca��es numa freq��ncia de
               *   1 - ( FrequenciaMalloc / LIMITE_FREQUENCIA ) */

      static long ContaNULL ;
            /* Contagem de NULLs gerados
               *
               *$ED Descri��o
               *   Conta o n�mero de vezes que o malloc n�o foi gerado devido a algum
               *   limite de aloca��o simulado. */

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

   static void DesalocarEspaco( tpElemListaControle * pEspaco ) ;

   static tpElemListaControle * ObterOrigemElemento( void * Ponteiro ) ;

   static void ExibirCabecalho( char * TipoMsg , tpElemListaControle * pEspaco ) ;

   static void ExibirMensagem( char * Msg ) ;

   static int VerificarEspaco( tpElemListaControle * pEspaco ) ;

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***************************************************************************
*
*  Fun��o: CED  &Inicializar o controlador de aloca��o de espa�os din�micos
*  ****/

   void CED_InicializarControlador( void )
   {

      TST_ASSERT( !estaInicializado ) ;

      pOrgLista            = NULL ;
      pEspacoCorr          = NULL ;
      numEspacosAlocados   = 0 ;
      TotalAlocacoes       = 0 ;
      EspacoTotalAlocado   = 0 ;
      LimiteMemoria        = 0 ;
      LimiteNumEspacos     = 0 ;
      FrequenciaMalloc     = 0 ;
      ContaNULL            = 0 ;

      ALT_Inicializar( 0 ) ;

      estaInicializado     = TRUE ;

   } /* Fim fun��o: CED  &Inicializar o controlador de aloca��o de espa�os din�micos */

/***************************************************************************
*
*  Fun��o: CED  &Alocar espa�o malloc
*  ****/

   void * CED_Malloc( size_t Tamanho ,
                      int    numLinhaFonte ,
                      char * NomeArquivoFonte )
   {

      tpElemListaControle * pEspaco ;

      int tamNomeArq ;

      TST_ASSERT( Tamanho > 0 ) ;
      TST_ASSERT( numLinhaFonte > 0 ) ;
      TST_ASSERT( NomeArquivoFonte != NULL ) ;

      /* Alocar sem controle quando n�o inicializado */

         if ( ! estaInicializado )
         {
            return malloc( Tamanho ) ;
         } /* if */

      /* Controlar limites de mem�ria */

         if ( LimiteMemoria > 0 )
         {
            if ( LimiteMemoria < EspacoTotalAlocado + ( long ) Tamanho )
            {
               TST_ExibirPrefixo( TIPO_MSG_INFO , "Simula falta de memoria." ) ;
               ContaNULL++ ;
               return NULL ;
            } /* if */
         } /* if */

         if ( LimiteNumEspacos > 0 )
         {
            if ( numEspacosAlocados >= LimiteNumEspacos )
            {
               TST_ExibirPrefixo( TIPO_MSG_INFO , "Limita numero de espacos alocados." ) ;
               ContaNULL++ ;
               return NULL ;
            } /* if */
         } /* if */

         if ( FrequenciaMalloc > 0 )
         {
            if ( ALT_GerarFrequencia( 1 , &FrequenciaMalloc , LIMITE_FREQUENCIA ) > 0)
            {
               TST_ExibirPrefixo( TIPO_MSG_INFO , "Simulou NULL por frequencia." ) ;
               ContaNULL++ ;
               return NULL ;
            } /* if */
         } /* if */

      /* Alocar o espa�o solicitado */

         pEspaco = ( tpElemListaControle * )
                     malloc( sizeof( tpElemListaControle ) + Tamanho -
                             DIM_VALOR ) ;

         if ( pEspaco == NULL )
         {
            TST_ExibirPrefixo( TIPO_MSG_INFO , "Memoria real insuficiente." ) ;
            return NULL ;
         } /* if */


         /* Alocar o espa�o solicitado */

         pEspaco = ( tpElemListaControle * )
                     malloc( sizeof( tpElemListaControle ) + Tamanho -
                             DIM_VALOR ) ;

         if ( pEspaco == NULL )
         {
            return NULL ;
         }

         EspacoTotalAlocado += Tamanho ;

      /* Inicializar os valores de controle do espa�o */

         pEspaco->EspacoNULL       = ESPACO_NULL ;
         pEspaco->ImpressaoDigital = IMPRESSAO_DIGITAL ;
         pEspaco->tamTotal         = Tamanho + sizeof( tpElemListaControle ) ;
         pEspaco->idEspaco         = TotalAlocacoes ;
         pEspaco->idTipoValor      = CED_ID_TIPO_VALOR_NULO ;
         pEspaco->ehAtivo          = TRUE ;
         pEspaco->tamValor         = Tamanho ;
         memcpy( pEspaco->ControleAntes ,   CONTROLE  , DIM_CONTROLE ) ;
         memset( pEspaco->Valor ,           CHAR_ALOC , Tamanho ) ;
         memcpy( pEspaco->Valor + Tamanho , CONTROLE  , DIM_CONTROLE ) ;

      /* Inicializar a identifica��o do c�digo fonte */

         pEspaco->numLinhaFonte = numLinhaFonte ;

         tamNomeArq = strlen( NomeArquivoFonte ) ;
         if ( tamNomeArq >= DIM_NOME_ARQ_FONTE )
         {
            NomeArquivoFonte += ( tamNomeArq - DIM_NOME_ARQ_FONTE + 1 ) ;
         } /* if */
         strcpy( pEspaco->NomeArquivoFonte , NomeArquivoFonte ) ;

      /* Encadear na origem da lista de espa�os alocados */

         pEspaco->pAnt      = NULL ;
         pEspaco->pProx     = pOrgLista ;
         if ( pOrgLista != NULL )
         {
            pOrgLista->pAnt = pEspaco ;
         } /* if */
         pOrgLista          = pEspaco ;

         numEspacosAlocados ++ ;
         TotalAlocacoes ++ ;

      return ( void * ) &( pEspaco->Valor ) ;

   } /* Fim fun��o: CED  &Alocar espa�o malloc */

/***************************************************************************
*
*  Fun��o: CED  &Alocar espa�o todo zerado calloc
*  ****/

   void * CED_Calloc( size_t Tamanho ,
                      int    numLinhaFonte ,
                      char * NomeArquivoFonte )
   {

      char * pAlloc ;

      pAlloc = ( char * ) CED_Malloc( Tamanho , numLinhaFonte ,
                                      NomeArquivoFonte ) ;
      if ( pAlloc != NULL )
      {
         memset( pAlloc , '\0' , Tamanho ) ;
      } /* if */

      return pAlloc ;

   } /* Fim fun��o: CED  &Alocar espa�o todo zerado calloc */

/***************************************************************************
*
*  Fun��o: CED  &Realocar espa�o alocado realloc
*  ****/

   void * CED_Realloc( size_t Tamanho ,
                       int    numLinhaFonte ,
                       char * NomeArquivoFonte )
   {

      char Msg[ 200 ] ;

      sprintf( Msg ,
               "realloc( %d ) nao esta disponivel em CESPDIN. "
               "Linha: %d  Modulo: %s" ,
               Tamanho , numLinhaFonte , NomeArquivoFonte ) ;

      ExibirMensagem( Msg ) ;
      return NULL ;

   } /* Fim fun��o: CED  &Realocar espa�o alocado realloc */

/***************************************************************************
*
*  Fun��o: CED  &Desalocar espa�o
*  ****/

   void CED_Free( void * Ponteiro )
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( estaInicializado ) ;

      if ( Ponteiro == NULL )
      {
         TST_ExibirPrefixo( TIPO_MSG_INFO , "Desalocar espaco recebeu ponteiro nulo" ) ;
         return ;
      } /* if */

      pEspaco = ObterOrigemElemento( Ponteiro ) ;
      if ( pEspaco == NULL )
      {
         free( Ponteiro ) ;
         return ;
      } /* if */

      DesalocarEspaco( pEspaco ) ;

   } /* Fim fun��o: CED  &Desalocar espa�o */

/***************************************************************************
*
*  Fun��o: CED  &Exibir conte�do do espa�o
*  ****/

   void CED_ExibirEspaco( void * Ponteiro )
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( estaInicializado ) ;

      if ( Ponteiro == NULL )
      {
         TST_ExibirPrefixo( TIPO_MSG_INFO , "Exibir espaco recebeu ponteiro nulo." ) ;
         return ;
      } /* if */

      pEspaco = ObterOrigemElemento( Ponteiro ) ;
      if ( pEspaco == NULL )
      {
         return ;
      } /* if */

      ExibirCabecalho( TIPO_MSG_INFO , pEspaco ) ;

      TST_ExibirEspacoHexa( pEspaco->tamValor , pEspaco->Valor ) ;

   } /* Fim fun��o: CED  &Exibir conte�do do espa�o */

/***************************************************************************
*
*  Fun��o: CED  &Exibir todos os espa�os
*  ****/

   void CED_ExibirTodosEspacos( CED_tpModoExibir Regra )
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( estaInicializado ) ;

      pEspaco = pOrgLista ;

      while ( pEspaco != NULL ) {

      /* Tratar modo de exibir */

         switch ( Regra ) {

         /* Exibir todos os espa�os */

            case CED_ExibirTodos :
            {

               CED_ExibirEspaco( &( pEspaco->Valor )) ;

               break ;

            } /* fim ativa: Exibir todos os espa�os */

         /* Exibir todos os espa�os alocados ativos */

            case CED_ExibirAtivos :
            {

               if ( pEspaco->ehAtivo )
               {
                  CED_ExibirEspaco( &( pEspaco->Valor )) ;
               } /* if */

               break ;

            } /* fim ativa: Exibir todos os espa�os alocados ativos */

         /* Exibir todos os espa�os alocados inativos */

            case CED_ExibirInativos :
            {

               if ( !( pEspaco->ehAtivo ))
               {
                  CED_ExibirEspaco( &( pEspaco->Valor )) ;
               } /* if */

               break ;

            } /* fim ativa: Exibir todos os espa�os alocados inativos */

         /* Exibir nada */

            default :
            {

               break ;

            } /* fim ativa: Exibir nada */

         } /* fim seleciona: Tratar modo de exibir */

      /* Avan�ar para o pr�ximo espa�o a exibir */

         pEspaco = pEspaco->pProx ;

      } /* fim repete: Raiz de CED  &Exibir todos os espa�os */

   } /* Fim fun��o: CED  &Exibir todos os espa�os */

/***************************************************************************
*
*  Fun��o: CED  &Definir tipo do espa�o
*  ****/

   int CED_DefinirTipoEspaco( void * Ponteiro , int idTipo )
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( estaInicializado ) ;

      if ( Ponteiro == NULL )
      {
         return FALSE ;
      } /* if */

      pEspaco = ObterOrigemElemento( Ponteiro ) ;

      if ( pEspaco == NULL )
      {
         return FALSE ;
      } /* if */

      if ( ( idTipo == CED_ID_TIPO_VALOR_NULO )
        || ( pEspaco->idTipoValor == CED_ID_TIPO_VALOR_NULO ))
      {
         pEspaco->idTipoValor = idTipo ;
         return TRUE ;
      } /* if */

      return FALSE ;

   } /* Fim fun��o: CED  &Definir tipo do espa�o */

/***************************************************************************
*
*  Fun��o: CED  &Marcar ativo o espa�o
*  ****/

   void CED_MarcarEspacoAtivo( void * Ponteiro )
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( estaInicializado ) ;

      if ( Ponteiro == NULL )
      {
         return ;
      } /* if */

      pEspaco = ObterOrigemElemento( Ponteiro ) ;
      if ( pEspaco == NULL )
      {
         return ;
      } /* if */

      pEspaco->ehAtivo = TRUE ;

   } /* Fim fun��o: CED  &Marcar ativo o espa�o */

/***************************************************************************
*
*  Fun��o: CED  &Marcar n�o ativo o espa�o
*  ****/

   void CED_MarcarEspacoNaoAtivo( void * Ponteiro )
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( estaInicializado ) ;
         
      if ( Ponteiro == NULL )
      {
         return ;
      } /* if */
         
      pEspaco = ObterOrigemElemento( Ponteiro ) ;
      if ( pEspaco == NULL )
      {
         return ;
      } /* if */
         
      pEspaco->ehAtivo = FALSE ;

   } /* Fim fun��o: CED  &Marcar n�o ativo o espa�o */

/***************************************************************************
*
*  Fun��o: CED  &Marcar n�o ativos todos os espa�os
*  ****/

   void CED_MarcarTodosEspacosInativos( )
   {

      tpElemListaControle * pEspaco ;
         

      TST_ASSERT( estaInicializado ) ;
         
      pEspaco = pOrgLista ;
      while ( pEspaco != NULL )
      {
         pEspaco->ehAtivo = FALSE ;
         pEspaco = pEspaco->pProx ;
      } /* while */

   } /* Fim fun��o: CED  &Marcar n�o ativos todos os espa�os */

/***************************************************************************
*
*  Fun��o: CED  &Limitar mem�ria dispon�vel
*  ****/

   void CED_LimitarMemoriaDisponivel( long NovoLimiteMemoria )
   {

      TST_ASSERT( estaInicializado ) ;
      TST_ASSERT( NovoLimiteMemoria >= 0 ) ;

      LimiteMemoria = NovoLimiteMemoria ;

   } /* Fim fun��o: CED  &Limitar mem�ria dispon�vel */

/***************************************************************************
*
*  Fun��o: CED  &Limitar n�mero de espa�os alocados
*  ****/

   void CED_LimitarNumeroTotalEspacos( long numTotalEspacos )
   {

      TST_ASSERT( estaInicializado ) ;
      TST_ASSERT( numTotalEspacos >= 0 ) ;

      LimiteNumEspacos = numTotalEspacos ;

   } /* Fim fun��o: CED  &Limitar n�mero de espa�os alocados */

/***************************************************************************
*
*  Fun��o: CED  &Limitar com freq��ncia de gera��o de NULL
*  ****/

   int CED_LimitarFrequencia( int Frequencia )
   {

      TST_ASSERT( estaInicializado ) ;

      if ( ( Frequencia <  LIMITE_INF_FREQUENCIA )
        || ( Frequencia >= LIMITE_SUP_FREQUENCIA ))
      {
         return FALSE ;
      } /* if */

      FrequenciaMalloc = Frequencia ;

      return TRUE ;

   } /* Fim fun��o: CED  &Limitar com freq��ncia de gera��o de NULL */

/***************************************************************************
*
*  Fun��o: CED  &Limitar, eliminar limites
*  ****/

   void CED_EliminarLimites( )
   {

      TST_ASSERT( estaInicializado ) ;

      LimiteMemoria        = 0 ;
      LimiteNumEspacos     = 0 ;
      FrequenciaMalloc     = 0 ;

   } /* Fim fun��o: CED  &Limitar, eliminar limites */

/***************************************************************************
*
*  Fun��o: CED  &Obter o tipo do espa�o de dados
*  ****/

   int CED_ObterTipoEspaco( void * Ponteiro )
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( estaInicializado ) ;

      if ( Ponteiro == NULL )
      {
         return CED_ID_TIPO_NULL ;
      } /* if */

      pEspaco = ObterOrigemElemento( Ponteiro ) ;
      if ( pEspaco == NULL )
      {
         return CED_ID_TIPO_ILEGAL ;
      } /* if */

      return pEspaco->idTipoValor ;

   } /* Fim fun��o: CED  &Obter o tipo do espa�o de dados */

/***************************************************************************
*
*  Fun��o: CED  &Obter tamanho do valor contido no espa�o
*  ****/

   int CED_ObterTamanhoValor( void * Ponteiro )
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( estaInicializado ) ;
         
      if ( Ponteiro == NULL )
      {
         return -1 ;
      } /* if */
         
      pEspaco = ObterOrigemElemento( Ponteiro ) ;
      if ( pEspaco == NULL )
      {
         return -1 ;
      } /* if */

      return pEspaco->tamValor ;

   } /* Fim fun��o: CED  &Obter tamanho do valor contido no espa�o */

/***************************************************************************
*
*  Fun��o: CED  &Obter tamanho do espa�o alocado _msize
*  ****/

   size_t CED_Msize( void * Ponteiro )
   {

      return ((size_t) CED_ObterTamanhoValor( Ponteiro )) ;

   } /* Fim fun��o: CED  &Obter tamanho do espa�o alocado _msize */

/***************************************************************************
*
*  Fun��o: CED  &Obter n�mero de espa�os alocados
*  ****/

   int CED_ObterNumeroEspacosAlocados( )
   {

      TST_ASSERT( estaInicializado ) ;

      return numEspacosAlocados ;

   } /* Fim fun��o: CED  &Obter n�mero de espa�os alocados */

/***************************************************************************
*
*  Fun��o: CED  &Obter total de espa�os alocados
*  ****/

   int CED_ObterTotalAlocacoes( )
   {

      TST_ASSERT( estaInicializado ) ;

      return TotalAlocacoes ;

   } /* Fim fun��o: CED  &Obter total de espa�os alocados */

/***************************************************************************
*
*  Fun��o: CED  &Obter n�mero total de NULLs gerados
*  ****/

   long CED_ObterNumNULL( void )
   {

      TST_ASSERT( estaInicializado ) ;

      return ContaNULL ;

   } /* Fim fun��o: CED  &Obter n�mero total de NULLs gerados */

/***************************************************************************
*
*  Fun��o: CED  &Obter n�mero de espa�os segundo regra
*  ****/

   int CED_ObterNumeroEspacos( CED_tpModoExibir Regra )
   {

      tpElemListaControle * pEspaco ;
      int Conta = 0 ;

      TST_ASSERT( estaInicializado ) ;
               
      pEspaco = pOrgLista ;

      while ( pEspaco != NULL )
      {
         switch ( Regra )
         {
            case CED_ExibirTodos :
               Conta ++ ;
               break ;

            case CED_ExibirAtivos :
               if ( pEspaco->ehAtivo )
               {
                  Conta ++ ;
               } /* if */
               break ;

            case CED_ExibirInativos :
               if ( !( pEspaco->ehAtivo ))
               {
                  Conta ++ ;
               } /* if */
               break ;

            default:
               break ;

         } /* fim switch */

         pEspaco = pEspaco->pProx ;
      } /* while */

      return Conta ;

   } /* Fim fun��o: CED  &Obter n�mero de espa�os segundo regra */

/***************************************************************************
*
*  Fun��o: CED  &Obter espa�o total alocado
*  ****/

   long CDE_ObterEspacoTotalAlocado( )
   {

      TST_ASSERT( estaInicializado ) ;

      return EspacoTotalAlocado ;

   } /* Fim fun��o: CED  &Obter espa�o total alocado */

/***************************************************************************
*
*  Fun��o: CED  &Iterador: iniciar iterador de espa�os
*  ****/

   void CED_InicializarIteradorEspacos( )
   {

      TST_ASSERT( estaInicializado ) ;

      pEspacoCorr = pOrgLista ;

   } /* Fim fun��o: CED  &Iterador: iniciar iterador de espa�os */

/***************************************************************************
*
*  Fun��o: CED  &Iterador: avan�ar para o pr�ximo espa�o
*  ****/

   int CED_AvancarProximoEspaco( )
   {

      TST_ASSERT( estaInicializado ) ;

      if ( pEspacoCorr == NULL )
      {
         return FALSE ;
      } /* if */

      pEspacoCorr = pEspacoCorr->pProx ;

      if ( pEspacoCorr != NULL )
      {
         return TRUE ;
      } else {
         return FALSE ;
      } /* if */

   } /* Fim fun��o: CED  &Iterador: avan�ar para o pr�ximo espa�o */

/***************************************************************************
*
*  Fun��o: CED  &Iterador: obter refer�ncia para o espa�o corrente
*  ****/

   void * CED_ObterPonteiroEspacoCorrente( )
   {

      TST_ASSERT( estaInicializado ) ;

      if ( pEspacoCorr == NULL )
      {
         return NULL ;
      } /* if */

      return ( void * ) &( pEspacoCorr->Valor ) ;

   } /* Fim fun��o: CED  &Iterador: obter refer�ncia para o espa�o corrente */

/***************************************************************************
*
*  Fun��o: CED  &Iterador: existe espa�o corrente
*  ****/

   int CED_ExisteEspacoCorrente( )
   {

      TST_ASSERT( estaInicializado ) ;

      if ( pEspacoCorr == NULL )
      {
         return FALSE ;
      } /* if */

      return TRUE ;

   } /* Fim fun��o: CED  &Iterador: existe espa�o corrente */

/***************************************************************************
*
*  Fun��o: CED  &Iterador: terminar iterador
*  ****/

   void CED_TerminarIteradorEspacos( )
   {

      pEspacoCorr = NULL ;

   } /* Fim fun��o: CED  &Iterador: terminar iterador */

/***************************************************************************
*
*  Fun��o: CED  &Iterador: excluir espa�o corrente
*  ****/

   void CED_ExcluirEspacoCorrente( )
   {

      tpElemListaControle * pDesaloca ;

      TST_ASSERT( estaInicializado ) ;

      if ( pEspacoCorr != NULL )
      {
         pDesaloca   = pEspacoCorr ;
         pEspacoCorr = pEspacoCorr->pProx ;
         DesalocarEspaco( pDesaloca ) ;
      } /* if */

   } /* Fim fun��o: CED  &Iterador: excluir espa�o corrente */

/***************************************************************************
*
*  Fun��o: CED  &Verificar se espa�o � ativo
*  ****/

   int CED_EhEspacoAtivo( void * Ponteiro )
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( estaInicializado ) ;

      if ( Ponteiro == NULL )
      {
         return FALSE ;
      } /* if */

      pEspaco = ObterOrigemElemento( Ponteiro ) ;
      if ( pEspaco == NULL )
      {
         return FALSE ;
      } /* if */

      return pEspaco->ehAtivo ;

   } /* Fim fun��o: CED  &Verificar se espa�o � ativo */

/***************************************************************************
*
*  Fun��o: CED  &Verificar a integridade de um espa�o de dados
*  ****/

   int CED_VerificarEspaco( void * Ponteiro ,
                            int ( * pVerificarValor )( void * pValor ))
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( estaInicializado ) ;

      /* Verificar as refer�ncias para o espa�o */

         if ( Ponteiro == NULL )
         {
            return TRUE ;
         } /* if */

         pEspaco = ObterOrigemElemento( Ponteiro ) ;

         if ( pEspaco == NULL )
         {
            return FALSE ;
         } /* if */

      /* Verificar o conte�do do espa�o */

         if ( pVerificarValor != NULL )
         {
            if ( pVerificarValor( pEspaco->Valor ) == 0 )
            {
               ExibirMensagem( "Valor contido no espaco esta incorreto." ) ;
               ExibirCabecalho( TIPO_MSG_VAZIA , pEspaco ) ;
               fprintf( TST_ObterArqLog( ) , "\n" ) ;
               return FALSE ;
            } /* if */
         } /* if */

      return TRUE ;

   } /* Fim fun��o: CED  &Verificar a integridade de um espa�o de dados */

/***************************************************************************
*
*  Fun��o: CED  &Verificar todos os espa�os alocados
*  ****/

   int CED_VerificarTudo( int ( * pVerificarValor )( void * pValor ))
   {

      tpElemListaControle * pEspaco ;

      int ContaErro   = 0 ,
          ContaEspaco = 0 ;

      TST_ASSERT( estaInicializado ) ;

      /* Verificar toda a lista de espa�os */

         pEspaco = pOrgLista ;
         while ( pEspaco != NULL )
         {
            ContaEspaco ++ ;
            if ( ! CED_VerificarEspaco( pEspaco->Valor , pVerificarValor ))
            {
               ContaErro ++ ;
            } /* if */
            pEspaco = pEspaco->pProx ;
         } /* while */

      /* Verificar n�mero de espa�os alocados */

         if ( ContaEspaco != numEspacosAlocados )
         {
            ExibirMensagem( "N�mero de espa�os alocados errado." ) ;
            ContaErro ++ ;
         } /* if */

         if ( numEspacosAlocados > TotalAlocacoes )
         {
            ExibirMensagem( "N�mero de espa�os alocados maior do que total alocado." ) ;
            ContaErro ++ ;
         } /* if */

      return ContaErro == 0 ;

   } /* Fim fun��o: CED  &Verificar todos os espa�os alocados */


/*****  C�digo das fun��es encapsuladas no m�dulo  *****/


/***********************************************************************
*
*  $FC Fun��o: CED  -Desalocar espa�o dado
*
***********************************************************************/

   void DesalocarEspaco( tpElemListaControle * pEspaco )
   {

      /* Desalocar o espa�o apontado */

         if ( pEspaco->pProx != NULL )
         {
            pEspaco->pProx->pAnt  = pEspaco->pAnt ;
         } /* if */

         if ( pEspaco->pAnt != NULL )
         {
            pEspaco->pAnt->pProx = pEspaco->pProx ;
         } else {
            pOrgLista = pEspaco->pProx ;
         } /* if */

         numEspacosAlocados -- ;
         EspacoTotalAlocado -= pEspaco->tamValor ;

      /* Limpar o espa�o a liberar */

         memset( pEspaco->ControleAntes , CHAR_DESALOC ,
                 pEspaco->tamValor + DIM_CONTROLE ) ;
         * (( unsigned long * ) ( pEspaco->Valor )) = 0 ;
         memcpy( pEspaco->ControleAntes + pEspaco->tamValor + DIM_CONTROLE ,
                 CONTROLE_ERRO , DIM_CONTROLE ) ;

         pEspaco->idTipoValor = CED_ID_TIPO_ILEGAL ;
         free( pEspaco ) ;

   } /* Fim fun��o: CED  -Desalocar espa�o dado */


/***********************************************************************
*
*  $FC Fun��o: CED  -Obter ponteiro para o elemento da lista de espa�os ativos
*
*  $ED Descri��o da fun��o
*     Retorna o ponteiro para o in�cio do elemento da lista de espa�os
*     alocados, no entanto sem controlar a integridade do esp�co.
*
*  $EP Par�metros
*     $P Ponteiro - aponta para o campo Valor do espa�o
*
*  $FV Valor retornado
*     Se o espa�o estiver v�lido, retorna o ponteiro para ele,
*     caso contr�rio retorna NULL.
*
***********************************************************************/

   tpElemListaControle * ObterOrigemElemento( void * Ponteiro )
   {

      tpElemListaControle * pEspaco ;

      TST_ASSERT( Ponteiro != NULL ) ;

      pEspaco = ( tpElemListaControle * )
                  ((( char * ) Ponteiro ) - ( sizeof( tpElemListaControle ) -
                     DIM_VALOR - DIM_CONTROLE )) ;

      if ( pEspaco->ImpressaoDigital != IMPRESSAO_DIGITAL )
      {
         return NULL ;
      } /* if */

      if ( VerificarEspaco( pEspaco ))
      {
         ExibirMensagem(  "Espaco de dados esta corrompido." ) ;

         fprintf( TST_ObterArqLog( ) , " Id: %d  pValor: %p" ,
                  pEspaco->idEspaco , pEspaco->Valor ) ;

         ExibirCabecalho( TIPO_MSG_VAZIA , pEspaco ) ;

         fprintf( TST_ObterArqLog( ) , "\n" ) ;

         return NULL ;
      } /* if */

      return pEspaco ;

   } /* Fim fun��o: CED  -Obter ponteiro para o elemento da lista de espa�os ativos */


/***********************************************************************
*
*  $FC Fun��o: CED  -Exibir cabecalho do espaco
*
*  $ED Descri��o da fun��o
*     Exibe todos os atributos, exceto o valor, contido no espa�o.
*
*  $EP Par�metros
*     $P TipoMsg - string (3 caracteres) indicando o significado do texto
*                  exibido na sa�da
*     $P pEspaco - ponteiro para a origem do espaco (n�o para o valor)
*
***********************************************************************/

   void ExibirCabecalho( char * TipoMsg , tpElemListaControle * pEspaco )
   {

      FILE * pArqLog ;

      int i ;

      char * pVal  ;
      char * pChar ;

      pArqLog = TST_ObterArqLog( ) ;

      fprintf( pArqLog , "\n\n%s  Espaco de dados, id: %5d" ,
                  TipoMsg , pEspaco->idEspaco ) ;

      /* Exibir ponteiro do espa�o */

         pVal  = pEspaco->Valor ;

         fprintf( pArqLog , "  pValor: %p  hexa:" , pVal ) ;

         pChar = ( char * ) &pVal ;
         for( i = 0 ; i < DIM_PONTEIRO ; i++ )
         {
            fprintf( pArqLog , " %02X" , pChar[ i ] ) ;
         } /* for */

      /* Exibir identifica��o do espa�o antecessor */

         if ( pEspaco->pAnt != NULL  )
         {
            fprintf( pArqLog , "  Id ant: %5d" , pEspaco->pAnt->idEspaco ) ;
         } else {
            fprintf( pArqLog , "  E' origem lista" ) ;
         } /* if */

      /* Exibir identifica��o do espa�o sucessor */

         if ( pEspaco->pProx != NULL )
         {
            fprintf( pArqLog , "  Id suc: %5d" , pEspaco->pProx->idEspaco ) ;
         } else {
            fprintf( pArqLog , "  E' final lista" ) ;
         } /* if */

      /* Exibir o identificador do tipo do valor */

         if ( pEspaco->idTipoValor == CED_ID_TIPO_VALOR_NULO )
         {
            fprintf( pArqLog , "\n     Tipo indefinido" ) ;
         } else if ( pEspaco->idTipoValor == CED_ID_TIPO_ILEGAL )
         {
            fprintf( pArqLog , "\n     Tipo ilegal" ) ;
         } else
         {
            fprintf( pArqLog , "\n     Id tipo valor: %d" , pEspaco->idTipoValor ) ;
         } /* if */

      /* Tamanho �til do valor alocado */

         fprintf( pArqLog , "  Tamanho: %d" , pEspaco->tamValor ) ;

      /* Exibir flag ativo */

         if ( pEspaco->ehAtivo )
         {
            fprintf( pArqLog , "  Ativo" ) ;
         } else {
            fprintf( pArqLog , "  Nao ativo" ) ;
         } /* if */

      /* Exibir n�mero da linha de c�digo fonte onde foi alocado */

         fprintf( pArqLog , "  Linha onde alocou: %d" , pEspaco->numLinhaFonte ) ;

      /* Exibir nome do arquivo onde alocou */

         fprintf( pArqLog , "  Arquivo: %s" , pEspaco->NomeArquivoFonte ) ;

      /* Exibir controle antes */

         if ( memcmp( pEspaco->ControleAntes , CONTROLE , DIM_CONTROLE ) == 0 )
         {
            fprintf( pArqLog , "\n     Controle antes OK" ) ;
         } else {
            fprintf( pArqLog , "\n     Controle antes errado:" ) ;
            for( i = 0 ; i < DIM_CONTROLE ; i++ )
            {
               fprintf( pArqLog , " %02X" , pEspaco->ControleAntes[ i ] ) ;
            } /* for */
         } /* if */

      /* Exibir controle ap�s */

         pChar = pEspaco->ControleApos - DIM_VALOR + pEspaco->tamValor ;
         if ( memcmp( pChar , CONTROLE , DIM_CONTROLE ) == 0 )
         {
            fprintf( pArqLog , "  Controle apos OK" ) ;
         } else {
            fprintf( pArqLog , "  Controle apos errado:" ) ;
            for( i = 0 ; i < DIM_CONTROLE ; i++ )
            {
               fprintf( pArqLog , " %02X" , pChar[ i ] ) ;
            } /* for */
         } /* if */

   } /* Fim fun��o: CED  -Exibir cabecalho do espaco */


/***********************************************************************
*
*  $FC Fun��o: CED  -Exibir mensagem de erro
*
*  $ED Descri��o da fun��o
*     Exibe uma mensagem de erro ou advert�ncia no formato padr�o
*
*  $EP Par�metros
*     $P Msg  - mensagem a exibir. O string n�o deve conter controles de
*               avan�o de linha
*
***********************************************************************/

   void ExibirMensagem( char * Msg )
   {

      TST_ContarFalhas( ) ;
      TST_ExibirPrefixo( TIPO_MSG_ERRO , Msg ) ;

   } /* Fim fun��o: CED  -Exibir mensagem de erro */


/***********************************************************************
*
*  $FC Fun��o: CED  -Verificar integridade de determinado espa�o
*
*  $ED Descri��o da fun��o
*     Controla a integridade ap�s conhecer o ponteiro para a origem
*     do espa�o. N�o controla o campo valor.
*
*  $EP Par�metros
*     $P pEspaco - aponta a origem do espa�o alocado
*
***********************************************************************/

   int VerificarEspaco( tpElemListaControle * pEspaco )
   {

      int ContaErro = 0 ;

      /* Verificar o marcador de espaco ainda n�o desalocado */

         if ( pEspaco->EspacoNULL != ESPACO_NULL )
         {
            ContaErro ++ ;
            if ( pEspaco->EspacoNULL == 0 )
            {
               ExibirMensagem( "Espaco ja\' foi desalocado: " ) ;
            } else
            {
               ExibirMensagem( "Controle inicial corrompido: " ) ;
            } /* if */
            return ContaErro ;
         } /* if */

      /* Verificar id espa�o */

         if ( ( pEspaco->idEspaco   < 0 )
           || ( pEspaco->idEspaco   > TotalAlocacoes )
           || ( numEspacosAlocados  > TotalAlocacoes ))
         {
            ContaErro ++ ;
            ExibirMensagem( "Identifica��o do espa�o est� errada." ) ;
         } /* if */

      /* Verificar encadeamento origem da lista */

         if ( pOrgLista != NULL )
         {
            if ( pOrgLista->pAnt != NULL )
            {
               ContaErro ++ ;
               ExibirMensagem( "Origem da lista de espa�os est� errada." ) ;
            } /* if */
         } else {
            ContaErro ++ ;
            ExibirMensagem( "Origem da lista n�o vazia � NULL." ) ;
            pOrgLista = pEspaco ;
         } /* if */

      /* Verificar encadeamento antes */

         if ( pEspaco != pOrgLista )
         {
            if ( pEspaco->pAnt != NULL )
            {
               if ( pEspaco->pAnt->pProx != pEspaco )
               {
                  ContaErro ++ ;
                  ExibirMensagem( "Encadeamento antes est� errado." ) ;
                  pEspaco->pAnt = NULL ;
               } /* if */
            } else {
               ContaErro ++ ;
               ExibirMensagem( "Esp�co anterior == NULL, mas n�o � a origem da lista de espa�os." ) ;
            } /* if */
         } else
         {
            if ( pEspaco->pAnt != NULL )
            {
               ContaErro ++ ;
               ExibirMensagem( "Espa�o origem da lista est� errada." ) ;
               pEspaco->pAnt = NULL ;
            } /* if */
         } /* if */

      /* Verificar encadeamento ap�s */

         if ( pEspaco->pProx != NULL )
         {
            if ( pEspaco->pProx->pAnt != pEspaco )
            {
               ContaErro ++ ;
               ExibirMensagem( "Encadeamento ap�s est� errado." ) ;
               pEspaco->pProx = NULL ;
            } /* if */
         } /* if */

      /* Verificar tamanho */

         if ( ( pEspaco->tamTotal != pEspaco->tamValor +
                          ( int ) sizeof( tpElemListaControle ))
           || ( pEspaco->tamValor <= 0 ))
         {
            ContaErro ++ ;
            ExibirMensagem( "Tamanho do espa�o est� errado." ) ;
         } /* if */

      /* Verificar controle antes */

         if ( memcmp( pEspaco->ControleAntes , CONTROLE , DIM_CONTROLE ) != 0 )
         {
            ContaErro ++ ;
            ExibirMensagem( "Controle de extravas�o antes est� errado." ) ;
         } /* if */

      /* Verificar controle ap�s */

         if ( memcmp( pEspaco->ControleApos - DIM_VALOR + pEspaco->tamValor ,
                      CONTROLE , DIM_CONTROLE ) != 0 )
         {
            ContaErro ++ ;
            ExibirMensagem( "Controle de extravas�o ap�s est� errado." ) ;
         } /* if */

      return ContaErro ;

   } /* Fim fun��o: CED  -Verificar integridade de determinado espa�o */

/********** Fim do m�dulo de implementa��o: CED  Controlador de espa�os de dados alocados dinamicamente **********/

