#if ! defined( INTRPCED_ )
#define INTRPCED_
/***************************************************************************
*
*  $MCD M�dulo de defini��o: ICED Interpretador controle de acesso a espacos de dados din�micos
*
*  Arquivo gerado:              INTRPCED.H
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
*  $EIU Interface com o usu�rio pessoa
*     Comandos de controle de espa�os din�micos sempre dispon�veis
*
*     =exibirtodosespacos  <regra>
*        <regra> pode ser uma de
*           1 - exibe todos os espa�os
*           2 - exibe somente os marcados ativo
*           3 - exibe somente os marcados inativo
*
*     =verificartodosespacos  <num erros esperado>
*
*     =obternumeroespacosalocados <num espa�os esperado>
*
*     =obternumerototalespacos <num total de aloca��es esperado>
*
*     =obternumeroregra  <regra>  <num esperado>
*        <regra> pode ser uma de
*           1 - exibe todos os espa�os
*           2 - exibe somente os marcados ativo
*           3 - exibe somente os marcados inativo
*        <num esperado>  � o n�mero esperado de espa�os alocados e que
*                        satisfazem a regra
*
*     =obtertotalespacosalocado  <num total bytes alocados esperado>
*
*     =obternumeroNULLgerado  <num total de NULLs simulados esperado>
*
*     =exibirnumeroNULLgerado
*
*     =limitarmemoriadisponivel <limite do n�mero de bytes dispon�veis
*                                para a aloca��o din�mica>
*
*     =limitarnumeroespacos     <limite do n�mero de espa�os alocados>
*
*     =limitarfrequenciaNULL    <freq��ncia com que alloca��es t�m sucesso>
*           � um valor dado em mil�simos:  999 indica 99,9% das vezes
*           CED_Malloc alocar� mem�ria
*
*     =eliminarlimitesespaco
*
***************************************************************************/
 
#if defined( INTRPCED_OWN )
   #define INTRPCED_EXT
#else
   #define INTRPCED_EXT extern
#endif

/***** Declara��es exportadas pelo m�dulo *****/

#include    "TST_ESPC.H"


/***********************************************************************
*
*  $FC Fun��o: ICED &Interpretar comandos controle de espa�o
*
*  $ED Descri��o da fun��o
*     O m�dulo INTESPAC interpreta os comandos do arcabou�o que controlam
*     os acessos a espa�os de dados din�micos.
*     N�o existe contexto para este interpretador
*     S�o seguintes os comandos interpretados:
*
*     =inicializarespacos
*         inicializa o sistema de controle de espa�os.
*         Deve ser executado exatamente uma vez.
*
*     =exibirtodosespacos         <Regra>
*         exibe todos os espa�os segundo a <Regra> de sele��o dada,
*         que pode ser:
*             CED_ExibirTodos    = 1  Exibir todos os espa�os
*             CED_ExibirAtivos   = 2  Exibir todos os espa�os ativos
*             CED_ExibirInativos = 3  Exibir todos os espa�os inativos
*
*     =verificartodosespacos      <numFalhasEsperado>
*         examina a validade de todo o espa�o
*
*     =obternumeroespacosalocados <numAlocadosEsperado>
*         obt�m o n�mero de espa�os controlados alocados no momento
*
*     =obternumerototalespacos    <numTotalEsperado>
*         obt�m o n�mero total de vezes em que foram alocados espa�os controlados
*
*     =obternumeroregra           <regra>  <numEsperado>
*         obt�m o n�mero de espa�os segundo <regra> de sele��o dada (ver acima)
*
*     =obtertotalespacosalocado   <totalEsperado>
*         obt�m o n�mero de bytes �teis alocados no momento
*         se <totalEsperado> for < 0  exibe somente o n�mero de bytes, sem controlar
*
*     =limitarmemoriadisponivel   <novoLimite>
*         estabelece um limite artificial de mem�ria dispon�vel
*
*     =limitarnumeroespacos       <novoLimite>
*         estabelece um limite artificial do n�umero de espa�os que podem
*         estar alocados em determinado momento
*
*     =eliminarlimitesespaco
*         retira os limites de aloca��o estabelecidos
*
*     As demais fun�oes do m�dulo CESPDIN CED Controle de Espa�o Din�mico
*     est�o dispon�veis no arcabou�o e podem ser utilizados livremente
*     por m�dulos de teste (ou outros) desenvolvidos pelo usu�rio do
*     arcabou�o. Elas s�o testadas atrav�s do m�dulo de teste TESTCED
*     espec�fico para este m�dulo
*
*  $EP Par�metros
*     $P ComandoTeste - String contendo o comando
*
***********************************************************************/

   TST_tpCondRet ICED_InterpretarTesteEspaco( char * ComandoTeste ) ;

#undef INTRPCED_EXT

/********** Fim do m�dulo de defini��o: ICED Interpretador controle de acesso a espacos de dados din�micos **********/

#else
#endif
