#if ! defined( TABSIMB_ )
#define TABSIMB_
/***************************************************************************
*
*  $MCD M�dulo de defini��o: TBS  Tabela de s�mbolos
*
*  Arquivo gerado:              TABSIMB.h
*  Letras identificadoras:      TBS
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
*     Implementa tabelas de s�mbolos gen�ricas, i.e. que cont�m dados
*        de qualquer tipo.
*     Podem existir n tabelas simultaneamente.
*     Cada tabela � homog�nea quanto ao tipo dos dados que armazena.
*     Cada dado cont�m o s�mbolo e o correspondente valor.
*     Para a tabela de s�mbolos � irrelevante a estrutura dos dados
*        armazenados, ou seja n�o � necess�rio conhecer o tipo espec�fico.
*     A tabela n�o se preocupa com poss�vel ordena��o dos s�mbolos
*        contidos nos dados.
*     Os ponteiros para os dados s�o copiados para dentro da tabela.
*        N�o � copiado o valor apontado por estes ponteiros.
*     Os dados passam a pertencer � tabela, ou seja, caso um s�mbolo seja
*        exclu�do, ser� destru�do o correspondente dado.
*     Cada tabela referencia uma fun��o que determina como devem
*        ser acessados os s�mbolos contidos nos dados nela contidos.
*     A fun��o de obten��o do s�mbolo contido em um dado � dependente do tipo
*        deste dado. Conseq�entemente a tabela passa a ser implicitamente
*        tipada com o tipo correspondente aos dados que cont�m. Um uso
*        incorreto dos tipos pode provocar erro de execu��o (cancelamento
*        da execu��o por acesso a endere�os - ponteiros - ilegais)
*     Cada tabela referencia uma fun��o que determina como devem ser
*        desalocados os dados nela contidos.
*        Isto permite armazenar dados compostos que contenham ponteiros
*        para espa�os que devam ser desalocados. Evita-se, assim, o
*        vazamento de mem�ria.
*     A fun��o de libera��o de espa�os de elementos deve assegurar a
*        libera��o de todos os espa�os de um elemento. Esta fun��o
*        � chamada antes de se desalocar o espa�o. Caso n�o seja
*        necess�rio desalocar espa�os referenciados pelo dado, o ponteiro
*        para esta fun��o de libera��o poder� ser NULL .
*
***************************************************************************/
 
#if defined( TABSIMB_OWN )
   #define TABSIMB_EXT
#else
   #define TABSIMB_EXT extern
#endif

/***** Declara��es exportadas pelo m�dulo *****/

#ifdef _DEBUG
   #include    "GENERICO.H"
#endif

typedef struct TBS_tagTabela * TBS_tppTabela ;


/***********************************************************************
*
*  $TC Tipo de dados: TBS Condi��es de retorno
*
*
*  $ED Descri��o do tipo
*     Condi��es de retorno das fun��es da tabela de s�mbolos
*
***********************************************************************/

   typedef enum {

         TBS_CondRetOK ,
               /* Concluiu corretamente */

         TBS_CondRetSimboloExiste ,
               /* S�mbolo j� existe */

         TBS_CondRetSimboloNaoExiste
               /* S�mbolo n�o existe */

   } TBS_tpCondRet ;


/***********************************************************************
*
*  $FC Fun��o: TBS  &Criar tabela de s�mbolos
*
*  $ED Descri��o da fun��o
*     Cria uma tabela de s�mbolos que armazenar� s�mbolos de determinado
*     tipo. Os poss�veis tipos s�o desconhecidos a priori. A tipagem
*     � implicita. N�o existe identificador de tipo associado � tabela.
*
*  $EP Par�metros
*     tamVetor  - tamanho do vetor de randomiza��o. Procure sempre
*                 escolher um n�mero primo. Isto tende a otimizar a
*                 randomiza��o.
*
*     ObterSimbolo - ponteiro para fun��o que retorna um ponteiro
*                    para o string do s�mbolo contido na estrutura de dados
*                    fornecida por par�metro
*                 Prot�tipo da fun��o :
*
*                       char * ObterSimbolo( void * pDado )
*
*                 A fun��o dever retornar:
*                    ponteiro para o string contido em dado
*
*                 A fun��o n�o realiza controle de exist�ncia do string.
*                 Como o tipo dos dados � indefinido (void), este poder� ser
*                 qualquer estrutura t�o complexa quanto se queira.
*                 No entanto, qualquer s�mbolo ser� sempre um string
*                 terminado em zero.
*
*     LiberarDado  - ponteiro para fun��o que libera os espa�os apontados
*                    pelo dado referenciado por um elemento da tabela de
*                    s�mbolos. Caso n�o existam tais ponteirospode-se
*                    passar o argumento NULL.
*
*  $FV Valor retornado
*     Se executou corretamente retorna o ponteiro para a tabela de
*     s�mbolos criada.
*     Este ponteiro deve ser utilizado pelas fun��es de manipula��o de
*     s�mbolos e da pr�pria tabela.
*
*     Se ocorreu algum erro, por exemplo falta de mem�ria ou dados errados,
*     a fun��o retornar� NULL. N�o ser� feita qualquer discrimina��o quanto
*     ao tipo de problema ocorrido.
*
***********************************************************************/

   TBS_tppTabela TBS_CriarTabela(
             int tamVetor                               ,
             char * ( * ObterSimbolo ) ( void * pDado ) ,
             void   ( * LiberarDado  ) ( void * pDado )  ) ;


/***********************************************************************
*
*  $FC Fun��o: TBS  &Destruir tabela de s�mbolos
*
*  $ED Descri��o da fun��o
*     Destr�i a tabela fornecida. O ponteiro para a tabela n�o � modificado.
*     Se ocorrer algum erro durante a destrui��o, a tabela resultar�
*     estruturalmente incorreta. OBS. n�o existe previs�o para poss�veis
*     falhas de execu��o.
*
*  $FV Valor retornado
*     TBS_CondRetOK    - destruiu sem problemas
*
***********************************************************************/

   void TBS_DestruirTabela( TBS_tppTabela pTabela ) ;


/***********************************************************************
*
*  $FC Fun��o: TBS  &Validar tabela de s�mbolos
*
*  $ED Descri��o da fun��o
*     Valida a estrutura da tabela. A fun��o verifica todas as
*     assertivas estruturais da tabela de s�mbolos
*
*     Esta fun��o depende da exist�ncia do arcabou�o de teste.
*     Ser� compilada se e somente se o nome _DEBUG estiver definido.
*
*  $EP Par�metros
*     pTabela - ponteiro para a tabela a verificar
*
*  $FV Valor retornado
*     TST_CondRetOK   - se a estrutura est� correta
*     TST_CondRetErro - se estiver incorreta
*
***********************************************************************/

#ifdef _DEBUG

   TST_tpCondRet TBS_ValidarTabela( TBS_tppTabela pTabela ) ;

#endif


/***********************************************************************
*
*  $FC Fun��o: TBS  &Inserir s�mbolo
*
*  $ED Descri��o da fun��o
*     Insere um dado contendo um s�mbolo na tabela dada.
*     � inserido o ponteiro, n�o a c�pia do dado. Caso o elemento da
*     tabela seja destr�do, o correspondente dado tamb�m o ser�.
*     No entanto, caso o dado contenha refer�ncias, estas n�o ser�o
*     processadas ao destruir.
*
*     Limita��es:
*     - s�mbolos devem sempre ser strings terminados em zero.
*     - as fun��es internas s�o sens�veis � caixa e � existe�ncia
*       de caracteres diacr�ticos.
*     - o dado contendo o s�mbolo passa a ser controlado pela tabela.
*       N�o � feita c�pia dele ao inserir.
*     - dados n�o podem conter ponteiros para outros espa�os, exceto no
*       caso de nem a tabela nem elementos individuais jamais serem
*       destru�dos durante a execu��o do programa.
*     - caso a fun��o n�o consiga inserir o dado (o s�mbolo contido
*       no dado j� existe na tabela), cabe � fun��o cliente destruir
*       a estrutura de dados enviada. Caso isto n�o seja feito, poder�
*       ocorrer vazamento de mem�ria.
*
*  $EP Par�metros
*     pTabela - ponteiro para a tabela onde deve ser inserido o s�mbolo
*     pDado   - ponteiro para o dado que cont�m o s�mbolo. O tipo do
*               dado deve ser consistente com o tipo registrado na tabela
*
*  $FV Valor retornado
*     TBS_CondRetOK            - inseriu um s�mbolo novo
*     TBS_CondRetS�mboloExiste - se n�o inseriu por j� existir
*
***********************************************************************/

   TBS_tpCondRet TBS_InserirSimbolo( TBS_tppTabela pTabela ,
                                     void * pDado           ) ;


/***********************************************************************
*
*  $FC Fun��o: TBS  &Procurar s�mbolo
*
*  $ED Descri��o da fun��o
*     Procura o s�mbolo dado
*
*  $EP Par�metros
*     pTabela  - ponteiro para a tabela onde procura
*     pSimbolo - ponteiro para um string (s�mbolo) a procurar
*
*  $FV Valor retornado
*     Se encontrou: ponteiro para o dado correspondente ao s�mbolo
*                   o ponteiro ser� consistente com o tipo registrado
*                   na tabela. Exceto o atributo s�mbolo, qualquer outro
*                   atributo desta estrutura poder� ser alterado
*                   externamente � tabela de s�mbolos
*     Se n�o encontrou: NULL
*
***********************************************************************/

   void * TBS_ProcurarSimbolo( TBS_tppTabela pTabela ,
                               char * pSimbolo         ) ;


/***********************************************************************
*
*  $FC Fun��o: TBS  &Excluir s�mbolo
*
*  $ED Descri��o da fun��o
*     Exclui o s�mbolo dado da tabela dada.
*
*  $EP Par�metros
*     pTabela    - ponteiro para a tabela onde deve excluir.
*     pSimbolo   - ponteiro para o simbolo correspondente ao dado
*                  a ser eliminado da tabela
*
*  $FV Valor retornado
*     TBS_CondRetOK               - se foi exclu�do
*     TBS_CondRetSimboloNaoExiste - se o s�mbolo j� n�o existia na tabela
*
*  $DE Observa��es
*     Note que o dado contido na tabela ser� destru�do e que n�o �
*     feita c�pia ao inseri-lo na tabela.
*
***********************************************************************/

   TBS_tpCondRet TBS_ExcluirSimbolo( TBS_tppTabela pTabela ,
                                     char * pSimbolo        ) ;

#undef TABSIMB_EXT

/********** Fim do m�dulo de defini��o: TBS  Tabela de s�mbolos **********/

#else
#endif
