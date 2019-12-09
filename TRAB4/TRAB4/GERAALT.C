/***************************************************************************
*  $MCI M�dulo de implementa��o: ALT  Gerador de n�meros aleat�rios
*
*  Arquivo gerado:              GERAALT.c
*  Letras identificadoras:      ALT
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

#include <stdlib.h>
#include <time.h>

#define GERAALT_OWN
#include "GERAALT.H"
#undef GERAALT_OWN

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***************************************************************************
*
*  Fun��o: ALT  &Inicializar o gerador de n�meros aleat�rios
*  ****/

   void ALT_Inicializar( unsigned int Base )
   {

      if ( Base == 0  )
      {
         srand( 3141592653 ) ;
      } else if ( Base == 1  )
      {
         srand( 3141592653 | ( unsigned int ) time( NULL )) ;
      } else
      {
         srand( Base ) ;
      } /* if */

   } /* Fim fun��o: ALT  &Inicializar o gerador de n�meros aleat�rios */

/***************************************************************************
*
*  Fun��o: ALT  &Gerar inteiro em uniformemente distribu�do
*  ****/

   int ALT_GerarDistUniforme( int Inf , int Sup )
   {

      double x ;
      int i ;

      do
      {
         x  = rand( ) ;                 /* converte para flutuante */
         x /= RAND_MAX ;                /* 0.0000 <= x <= 1.0000     */
      } while ( x == 1.0 );             /* evitar valor == 1 */

      #pragma warning(disable:4244)
      i  = ( Sup - Inf + 1 ) * x ;      /* converte/trunca de volta para inteiro */
      #pragma warning(default:4244)
      return Inf + i ;

   } /* Fim fun��o: ALT  &Gerar inteiro em uniformemente distribu�do */

/***************************************************************************
*
*  Fun��o: ALT  &Gerar distribui��o dada por tabela de freq��ncias
*  ****/

   int ALT_GerarFrequencia( int numElem , int * vetElem , int Limite )
   {

      int i ,
          j ;

      i = ALT_GerarDistUniforme( 0 , Limite ) ;
      for( j = 0 ; j < numElem ; j++ )
      {
         if ( i <= vetElem[ j ] )
         {
            return j ;
         } /* if */
      } /* for */
      return numElem ;

   } /* Fim fun��o: ALT  &Gerar distribui��o dada por tabela de freq��ncias */

/***************************************************************************
*
*  Fun��o: ALT  &Gerar permuta��o rand�mica uniformemente distribu�da
*  ****/

   void ALT_GerarPermutacao( int   tamVetor ,
                             int * Vetor     )
   {

      int Temp ,
          inx  ,
          i     ;

      for( i = tamVetor - 1 ; i >= 1 ; i-- )
      {
         inx          = ALT_GerarDistUniforme( 0 , i ) ;
         Temp         = Vetor[ inx ] ;
         Vetor[ inx ] = Vetor[ i   ] ;
         Vetor[ i   ] = Temp ;
      } /* for */

   } /* Fim fun��o: ALT  &Gerar permuta��o rand�mica uniformemente distribu�da */

/********** Fim do m�dulo de implementa��o: ALT  Gerador de n�meros aleat�rios **********/

