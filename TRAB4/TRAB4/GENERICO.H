#if ! defined( GENERICO_ )
#define GENERICO_
/***************************************************************************
*
*  $MCD M�dulo de defini��o: TSTG Controlador de teste gen�rico
*
*  Arquivo gerado:              GENERICO.h
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
*  $ED Descri��o do m�dulo
*     O m�dulo de teste gen�rico cont�m um conjunto de fun��es de
*     coordena��o, apoio e controle da execu��o de testes de m�dulos
*     quaisquer.
*
*     Embora na pr�tica a fun��o de teste espec�fica seja uma extens�o da
*     fun��o de teste gen�rica, a fun��o de teste espec�fica encontra-se em
*     outro m�dulo. Desta forma as altera��es necess�rias para cada
*     m�dulo a testar concentram-se exclusivamente naquele m�dulo.
*
*  $EIU Interface com o usu�rio pessoa
*     Os seguintes comandos gen�ricos est�o definidos:
*
*     linha em branco - faz nada
*
*     //      - coment�rio, � impresso no LOG
*
*     ==      - t�tulo de caso de teste, o t�tulo � impresso no LOG
*
*     =recuperar
*             - Caso o comando imediatamente anterior ao comando
*               =recuperar retorne TST_CondRetOK, obviemanete o comando
*               executou corretamente, portanto o comando =recuperar
*               reportar� uma falha no script de teste, uma vez que n�o
*               foi encontrada a falha esperada ao executar o comando.
*             - Caso o comando antecessor retorne TST_CondRetErro,
*               ou TST_CondRetParm (erro de par�metros do comando)
*               a falha reportada ser� desconsiderada, uma vez que
*               era esperada. O comando =recuperar destina-se a
*               apoiar o teste da corretude dos validadores de
*               par�metros dos comandos e das fun��es por eles
*               ativadas.
*             - caso o comando antecessor retorne outras condi��es, o
*               comando =recuperar ser� tratado como falha.
*
*     =bkpt   - permite interagir com o depurador utilizado para
*               desenvolver os m�dulos sob teste. O comando =bkpt
*               executa um c�digo "dummy" no qual pode ser inserido
*               um "breakpoint" do depurador. Desta forma o teste pode
*               prosseguir sem interrup��o at� o comando =bkpt.
*               Para isto
*               - apague ou desative todos os breakpoints do depurador
*               - insira o comando =bkpt no script pouco antes do ponto
*                 de interrup��o desejado
*               - usando o depurador, estabele�a um breakpoint na linha de
*                 c�digo: "Dummy = 0 ;" contida no m�dulo de teste
*                 gen�rico. Esta linha pode ser encontrada
*                 procurando por "BKPT_CMD"
*               - d� partida no programa e espere at� chegar ao breakpoint
*                 associado ao comando "dummy". Agora reative ou instale
*                 os breakpoints requeridos pela a sess�o de depura��o.
*                 e continue depurando
*
*     =cancelar
*             - termina a interpreta��o dos comandos de teste.
*               Sempre sinaliza falha.
*
*     =AceitaFalhasAcumuladas  numFalhas
*             - caso o contador de falhas registre numFalhas, ele ser�
*               tornado zero. Este comando � similar a =recuperar.
*               Difere deste por ser executado desde que o comando
*               anterior n�o tenha reportado erro, tal como acontece
*               com todos os comandos, exceto =recuperar.
*               O comando permite recuperar falhas sinalizadas por
*               m�dulos de apoio, tais como, leitor de par�metros,
*               contador de passagens e controle de espa�o din�mico.
*               Caso o contador de n�mero de falhas contenha um valor
*               diferente de numFalhas, ser� reportado um erro e
*               o contador � incrementado de 1.
*
*     Os demais comandos s�o passados para as fun��es de teste espec�fico.
*     Primeiro para as do arcabou�o, depois para a espec�fica do m�dulo
*     a testar. A �ltima fun��o de teste espec�fico deve retornar a condi��o
*     TST_CondRetNaoConhec caso n�o consiga interpretar a diretiva.
*
*  $EIM Interface com outros m�dulos
*     A primeira fun��o do arcabou�o ("framework") a ser chamada deve a
*     sempre ser TST_InicializarTeste. Caso n�o o seja, ocorrer�o erros
*     de execu��o (assert).
*
*     Ap�s abra o arquivo de script executando a fun��o
*     TST_AbrirArquivoScript. Se for desejado gerar um log abra
*     o arquivo corresponente com a fun��o TST_AbrirArquivoLog.
*
*     Depois de abrir os arquivos, execute os testes chamando a fun��o
*     TST_RealizarTeste. A fun��o de teste gen�rico ler� todo o arquivo
*     de script, retornando ap�s concluir o teste.
*
*     Ap�s, termine o teste atrav�s da fun��o TST_TerminarTeste.
*     Esta fun��o exibe as estat�sticas do teste e, caso solicitado,
*     acumula essas estat�sticas em um arquivo acumulador (ver
*     par�metro /a do programa principal.
*
*     As fun��es "TST_ObterXXX" podem ser utilizadas antes de fechar o
*     m�dulo de teste para exibir o resumo do teste no m�dulo cliente do
*     m�dulo de controle gen�rico.
*
*     O programa cliente do m�dulo controle de teste gen�rico pode
*     efetuar as opera��es acima repetidas vezes, permitindo, assim o
*     teste envolvendo diversos arquivos scripts de teste. A sa�da
*     de cada teste deve ser dirigida para um log espec�fico, ou para
*     stdout, uma vez que, ao abrir um arquivo log, o seu conte�do ser�
*     sempre destru�do.
*
*     O m�dulo de teste gen�rico tamb�m implementa diversas fun��es
*     de compara��o "TST_CompararXXX" use-as no m�dulo de controle de
*     teste espec�fico. Desta forma ser� assegurado o tratamento uniforme
*     de todas as falhas possivelmente encontradas.
*
*     O m�dulo disponibiliza ainda a fun��o TST_ExibirPrefixo
*     que gera uma linha de sa�da obedecendo aos padr�es de interface
*     estabelecidos. Sendo desejado pode-se adicionar textos � linha
*     usando a funcao fprintf( TST_ObterArqLog( ) , ... ). Um exemplo da
*     utilidade disso � quando se deseja imprimir informa��o complementar
*     ao interpretar uma determinada diretiva. A fun��o TST_ObterArqLog( )
*     retorna a refer�ncia ao arquivo log em uso no momento.
*
*     Finalmente, o m�dulo disponibiliza a fun��o TST_NotificarFalha
*     que incrementa o contador de falhas al�m de emitir uma mensagem
*     de erro.
*
***************************************************************************/
 
#if defined( GENERICO_OWN )
   #define GENERICO_EXT
#else
   #define GENERICO_EXT extern
#endif

/***** Declara��es exportadas pelo m�dulo *****/

#include    "TST_ESPC.H"


/***********************************************************************
*
*  $FC Fun��o: TSTG &Controlar uma sess�o de teste
*
*  $ED Descri��o da fun��o
*     Esta fun��o � o ponto de entrada para a realiza��o do teste.
*     Permite que se adicione o arcabou�o de teste a um sistema qualquer.
*
*  $EP Par�metros
*     $P NomeConstrutoParm
*     $P NomeArqScriptParm - deve ser n�o nulo
*     $P NomeArqLogParm    - se for nulo, gera a sa�da na console
*     $P NomeArqAcumParm   - se n�o for nulo, acumula as estat�sticas no
*                            arquivo denominado
*
*  $FV Valor retornado
*     0 - se o teste ocorreu OK
*     4 - se foram observados erros de dados ou falhas nos testes
*
***********************************************************************/

   int TST_ControlarSessaoTeste( char * NomeConstrutoParm ,
                                 char * NomeArqScriptParm ,
                                 char * NomeArqLogParm    ,
                                 char * NomeArqAcumParm    ) ;


/***********************************************************************
*
*  $FC Fun��o: TSTG &Comparar booleano
*
*  $ED Descri��o da fun��o
*     Compara valor booleano esperado com valor booleano obtido,
*     reportamdo uma falha caso n�o sejam iguais.
*     Note que "!= 0" corresponde a TRUE e "== 0" corresponde a FALSE
*
*  $EP Par�metros
*     ValorEsperado  - � um valor fornecido como par�metro do comando
*                      de teste
*     ValorObtido    - � o valor retornado pela fun��o em teste
*     pMensagem      - mensagem explanat�ria da falha encontrada
*
*  $FV Valor retornado
*     TST_CondRetErro  se o valor esperado n�o for consistente com o obtido
*     TST_CondRetOK    se os valores forem consistentes
*
***********************************************************************/

   TST_tpCondRet TST_CompararBool( int    ValorEsperado ,
                                   int    ValorObtido   ,
                                   char * pMensagem      ) ;


/***********************************************************************
*
*  $FC Fun��o: TSTG &Comparar caractere
*
*  $ED Descri��o da fun��o
*     Compara um caractere esperado com um caractere obtido, emitindo mensagem de
*     erro caso n�o sejam iguais.
*
*  $EP Par�metros
*     ValorEsperado  - � um valor fornecido como par�metro do comando
*                      de teste
*     ValorObtido    - � o valor retornado pela fun��o em teste
*     pMensagem      - mensagem explanat�ria da falha encontrada
*
*  $FV Valor retornado
*     TST_CondRetErro  se o valor esperado n�o for igual ao obtido
*     TST_CondRetOK    se os valores forem iguais
*
***********************************************************************/

   TST_tpCondRet TST_CompararChar( char   ValorEsperado ,
                                   char   ValorObtido   ,
                                   char * pMensagem       ) ;


/***********************************************************************
*
*  $FC Fun��o: TSTG &Comparar inteiro
*
*  $ED Descri��o da fun��o
*     Compara valor inteiro long esperado com valor inteiro obtido,
*     reportamdo uma falha caso n�o sejam iguais.
*
*  $EP Par�metros
*     ValorEsperado  - � um valor fornecido como par�metro do comando
*                      de teste
*     ValorObtido    - � o valor retornado pela fun��o em teste
*     pMensagem      - mensagem explanat�ria da falha encontrada
*
*  $FV Valor retornado
*     TST_CondRetErro  se o valor esperado n�o for igual ao obtido
*     TST_CondRetOK    se os valores forem iguais
*
***********************************************************************/

   TST_tpCondRet TST_CompararInt( long   ValorEsperado ,
                                  long   ValorObtido   ,
                                  char * pMensagem       ) ;


/***********************************************************************
*
*  $FC Fun��o: TSTG &Comparar flutuante
*
*  $ED Descri��o da fun��o
*     Compara valor flutuante esperado com valor flutuante obtido,
*     reportando uma falha caso
*     1 - Tolerancia > esperado / obtido ou
*     1 + Tolerancia < esperado / obtido
*
*  $EP Par�metros
*     ValorEsperado  - � um valor fornecido como par�metro do comando
*                      de teste
*     ValorObtido    - � o valor retornado pela fun��o em teste
*     Toler�ncia     - indica os limites da rela��o entre esperado e obtido.
*                      Deve ser um valor positivo, menor do que 1.
*                      Use um valor do tipo 10 ** -n, no qual n � o
*                      n�mero de d�gitos significativos desejado.
*     pMensagem      - mensagem explanat�ria da falha encontrada
*
*  $FV Valor retornado
*     TST_CondRetErro  se o valor obtido n�o estiver dentro do dom�nio da
*                      toler�ncia em torno do valor esperado
*     TST_CondRetOK    se os valores estiverem dentro da toler�ncia
*
***********************************************************************/

   TST_tpCondRet TST_CompararFloat( double   ValorEsperado ,
                                    double   ValorObtido   ,
                                    double   Tolerancia    ,
                                    char *   pMensagem      ) ;


/***********************************************************************
*
*  $FC Fun��o: TSTG &Comparar string
*
*  $ED Descri��o da fun��o
*     Compara string esperado com string obtido, reportamdo uma falha
*     caso n�o sejam iguais. Ambos os strings devem obedecer o
*     padr�o C, terminando em zero.
*
*  $EP Par�metros
*     ValorEsperado  - � um string fornecido como par�metro do comando
*                      de teste.
*     ValorObtido    - � o string retornado pela fun��o em teste
*     pMensagem      - mensagem explanat�ria da falha encontrada
*
*  $FV Valor retornado
*     TST_CondRetErro  se o valor esperado n�o for igual ao obtido
*     TST_CondRetOK    se os valores forem iguais
*
***********************************************************************/

   TST_tpCondRet TST_CompararString( char * ValorEsperado ,
                                     char * ValorObtido   ,
                                     char * pMensagem       ) ;


/***********************************************************************
*
*  $FC Fun��o: TSTG &Comparar espa�o
*
*  $ED Descri��o da fun��o
*     Compara espa�os de dados, reportando uma falha caso n�o sejam iguais.
*     Esta fun��o pode ser utilizada para comparar qualquer coisa,
*     inclusive valores do tipo  "struct"
*     O tipo do valor � indefinido e os valores ser�o exibidos em
*     formato hexadecimal, caso n�o sejam iguais
*
*  $EP Par�metros
*     ValorEsperado  - ponteiro para o espa�o que cont�m o valor esperado
*     ValorObtido    - ponteiro para o espa�o que cont�m o valor obtido
*     TamEspaco      - tamanho do espa�o a ser comparado
*     pMensagem      - mensagem explanat�ria da falha encontrada
*
*  $FV Valor retornado
*     TST_CondRetErro  se o valor esperado n�o for igual ao obtido
*     TST_CondRetOK    se os valores forem iguais
*
***********************************************************************/

   TST_tpCondRet TST_CompararEspaco( void * ValorEsperado ,
                                     void * ValorObtido   ,
                                     int    TamEspaco     ,
                                     char * pMensagem       ) ;


/***********************************************************************
*
*  $FC Fun��o: TSTG &Comparar ponteiro
*
*  $ED Descri��o da fun��o
*     Compara dois ponteiros emitindo uma mensagem caso n�o sejam iguais
*
*  $FV Valor retornado
*     TST_CondRetErro  se os dois ponteiros forem diferentes
*     TST_CondRetOK    se forem iguais
*
***********************************************************************/

   TST_tpCondRet TST_CompararPonteiro( void * PonteiroEsperado ,
                                       void * PonteiroObtido   ,
                                       char * pMensagem       ) ;


/***********************************************************************
*
*  $FC Fun��o: TSTG &Comparar ponteiro nulo
*
*  $ED Descri��o da fun��o
*     Verifica
*       - quando ModoEsperado == 0 se o PonteiroObtido � nulo
*       - quando ModoEsperado != 0 se o PonteiroObtido � n�o nulo
*
*  $FV Valor retornado
*     TST_CondRetErro  se os dois ponteiros forem diferentes
*     TST_CondRetOK    se forem iguais
*
***********************************************************************/

   TST_tpCondRet TST_CompararPonteiroNulo( int    ModoEsperado ,
                                           void * PonteiroObtido   ,
                                           char * pMensagem       ) ;


/***********************************************************************
*
*  $FC Fun��o: TSTG &Assertiva controlada
*
*  $ED Descri��o da fun��o
*     Esta fun��o substitui a fun��o assert de C.
*     � utilizada no contexto do arcabou�o de teste.
*     Ela imprime mensagens mais vis�veis tanto no log como na
*     janela de comando.
*
*     Recomenda-se, no entanto, utilizar a fun��o assert de C em aplica��es.
*
*  $FV Valor retornado
*     Termina a execu��o do programa com c�digo de retorno == 4
*
***********************************************************************/

   #define  TST_ASSERT( Expressao ) \
      TST_Assert( Expressao , __LINE__ , __FILE__ )

   void TST_Assert( int Expressao , int Linha , char * NomeArq ) ;


/***********************************************************************
*
*  $FC Fun��o: TSTG &Exibir prefixo da mensagem
*
*  $ED Descri��o da fun��o
*     Imprime o prefixo de mensages de falha ou de informa��o.
*     N�o incrementa o contador de falhas.
*     Esta fun��o estabelece o padr�o de sa�da das mensagens geradas
*     durante os testes.
*
*  $EP Par�metros
*     $P Indicador - informa a natureza da mensagem,
*                    Utilize preferenciamente as constantes definidas em
*                    TST_ESPEC.H:
*                       SINALIZA_ERRO       ">>>"
*                       SINALIZA_RECUPERA   "<<<"
*                       SINALIZA_COMENTARIO "!!!"
*     $P pMensagem - � a mensagem prefixo a ser gerada.
*
*  $EIU Interface com usu�rio pessoa
*     Podem ser acrescentadas mensagens ao prefixo. A fun��o
*     TST_ObterArqLog retorna o ponteiro para o arquivo log em uso
*     (stdout ou definido pelo usu�rio)
*
***********************************************************************/

   void TST_ExibirPrefixo( char * Indicador , char * pMensagem ) ;


/***********************************************************************
*
*  $FC Fun��o: TSTG &Notificar falha
*
*  $ED Descri��o da fun��o
*     Exibe uma mensagem de falha no formato padr�o do arcabou�o.
*     A fun��o n�o exibe os valores esperado e obtido.
*
*  $EP Par�metros
*     $P pMensagem - mensdagem explicativa da falha.
*
*  $FV Valor retornado
*     TST_CondRetErro  sempre
*
***********************************************************************/

   TST_tpCondRet TST_NotificarFalha( char * pMensagem ) ;


/***********************************************************************
*
*  $FC Fun��o: TSTG &Incrementar contador de falhas
*
*  $ED Descri��o da fun��o
*     Esta fun��o deve ser usada quando, por alguma raz�o, precisar-se
*     utilizar TST_ExibirPrefixo para exibir uma mensagem de falha.
*     As outras fun��es (compara��o, TST_NotificarFalha, etc.)
*     incrementam o contador automaticamente.
*
***********************************************************************/

   void TST_ContarFalhas( ) ;


/***********************************************************************
*
*  $FC Fun��o: TSTG &Obter n�mero de casos de teste efetuados
*
*  $ED Descri��o da fun��o
*     Retorna o n�mero de casos de teste lidos.
*     Cada caso de teste inicia com o comando "== <nome do caso de teste>".
*
***********************************************************************/

   int TST_ObterNumCasosTeste( ) ;


/***********************************************************************
*
*  $FC Fun��o: TSTG &Obter n�mero de comandos espec�ficos corretamente interpretados
*
*  $ED Descri��o da fun��o
*     Retorna o n�mero de comandos de teste espec�ficos lidos e
*     corretamente interpretados.
*     N�o conta comandos que retornem n�o OK independentemento do motivo,
*        inclusive os comandos saltados ap�s uma falha.
*     Tamb�m n�o conta comandos recuperar.
*
***********************************************************************/

   int TST_ObterNumComandosExecutados( void ) ;


/***********************************************************************
*
*  $FC Fun��o: TSTG &Obter n�mero de falhas encontradas
*
*  $ED Descri��o da fun��o
*     Retorna o n�mero de falhas encontrados.
*     O m�dulo de teste espec�fico sinaliza falhas atrav�s de
*     uma condi��o de retorno diferente de TST_CondRetOK.
*     Al�m de compara��es em erro, s�o tratados como falhas:
*       - comandos de teste em erro,
*       - comandos de teste de um dado caso de teste ignorados (saltados)
*       - erros encontrados nos instrumentos
*       - erros de sintaxe dos comandos de teste
*       - qualquer chamada �s fun��es TST_NotificarFalha, ou TST_ContarFalha.
*     Portanto, � poss�vel que o n�mero de falhas venha a ser maior do
*     que o n�mero de casos de teste existente no script.
*
***********************************************************************/

   int TST_ObterNumFalhas( void ) ;


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

   char * TST_ObterInicioExtensao( char * NomeArqParm ) ;


/***********************************************************************
*
*  $FC Fun��o: TSTG &Obter ponteiro para arquivo LOG
*
*  $ED Descri��o da fun��o
*     Retorna o ponteiro para o FILE do log em uso no momento.
*     Ser� stdout caso n�o seja definido explicitamente ao chamar a funcao
*     TST_ControlarSessaoTeste
*
*  $FV Valor retornado
*     Ponteiro v�lido para um arquivo de texto de sa�da
*
***********************************************************************/

   FILE * TST_ObterArqLog( void ) ;


/***********************************************************************
*
*  $FC Fun��o: TSTG &Exibir um espa�o qualquer em formato hexadecimal e char
*
*  $ED Descri��o da fun��o
*     Exibe o conte�do de um espa�o de tamEspaco bytes. A sa�da cont�m
*     uma coluna em hexadecimal e outra em ASCII
*     S�o exibidos 16 bytes por linha.
*
*  $EP Par�metros
*     $P tamEspaco - tamanho em bytes do espa�o a exibir
*     $P Ponteiro  - ponteiro para o primeiro byte do espa�o a exibir
*
***********************************************************************/

   void TST_ExibirEspacoHexa( int    tamEspaco ,
                              void * Ponteiro   ) ;

#undef GENERICO_EXT

/********** Fim do m�dulo de defini��o: TSTG Controlador de teste gen�rico **********/

#else
#endif
