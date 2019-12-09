#if ! defined( INTRPCNT_ )
#define INTRPCNT_
/***************************************************************************
*
*  $MCD M�dulo de defini��o: ICNT Interpretador de contadores
*
*  Arquivo gerado:              INTRPCNT.H
*  Letras identificadoras:      ICNT
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
*     Cont�m o interpretador de comandos de contagem.
*     Este m�dulo deve ser referenciado no m�dulo de teste gen�rico
*     e a fun��o de interpreta��o deve ser incorporada no
*     "daisy chain" que chama interpretadores sucessivos at� que
*     um deles interprete o comando.
*
*  $EIU Interface com o usu�rio pessoa
*     Comandos de contagem sempre dispon�veis
*
*     =inicializarcontadores   <nome arquivo contadores acumulado>
*          o nome do arquivo pode ser nulo.
*          Se o nome do arquivo for ".", ser� tamb�m tratado como nulo.
*
*     =terminarcontadores    <n�mero contadores iguais a zero esperado>
*          Grava o arquivo de contadores acumulados, caso esteja
*          definido (nome n�o vazio).
*
*     =registraracumulador   <nome arquivo contadores acumulado>
*          substitui o nome registrado.
*          o nome do arquivo pode ser nulo.
*          Se o nome do arquivo for ".", ser� tamb�m tratado como nulo.
*
*     =lercontadores         <nome arquivo contadores>
*          le� o arquivo e atribui os valores iniciais somente se
*          o contador tiver sido lido de um arquivo acumulado.
*
*     =gravarcontadores      <nome arquivo contadores acumulado>
*          grava o arquivo de contadores acumulado registrado.
*          A grava��o ocorre tamb�m ao terminar contadores
*
*     =zerartodoscontadores
*
*     =zerarcontador   <string nome do contador>
*
*     =iniciarcontagem
*          Ativa a contagem. Este comando precisa ser dado.
*          =inicializarcontadores n�o ativa a contagem, somente ativa
*          o m�dulo
*
*     =pararcontagem
*          Desativa a contagem
*
*     =contagemcontador   <n�mero esperado>
*
*     =exibircontagem
*
*     =numcontadores      <n�mero esperado>
*
*     =contagemtotal      <somat�rio das contagens esperado>
*
*     =verificarcontagens <n�mero contadores zero iguais a zero esperado>
*
***************************************************************************/
 
#if defined( INTRPCNT_OWN )
   #define INTRPCNT_EXT
#else
   #define INTRPCNT_EXT extern
#endif

/***** Declara��es exportadas pelo m�dulo *****/

#include    "TST_ESPC.H"


/***********************************************************************
*
*  $FC Fun��o: ICNT &Interpretar comandos de contagem
*
*  $ED Descri��o da fun��o
*     Esta fun��o interpreta todos os comandos espec�ficos para
*     a contagem da passagem do controle da execu��o por determinado
*     ponto do c�digo fonte.
*
*     Interpreta os comandos
*
*     =inicializarcontadores     <cond ret>
*     =terminarcontadores        <cond ret>
*     =registraracumulador       <nome arquivo acumulador>
*     =lercontadores             <nome arquivo contadores> <num erros esperados>
*     =zerartodoscontadores      <cond ret>
*     =zerarcontador             <nome contador> <cond ret>
*     =iniciarcontagem
*     =pararcontagem
*     =contagemcontador          <nome contador> <contagem esperada>
*     =numcontadores             <num esperado>
*     =contagemtotal             <Contagem esperada>
*     =verificarcontagens        <num contadores zero esperados>
*
***********************************************************************/

   TST_tpCondRet ICNT_EfetuarComadoContagem( char * ComandoTeste ) ;

#undef INTRPCNT_EXT

/********** Fim do m�dulo de defini��o: ICNT Interpretador de contadores **********/

#else
#endif
