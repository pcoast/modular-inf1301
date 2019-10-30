/***************************************************************************
*   $MCI Módulo de definição: Módulo Labirinto
*
*   Arquivo gerado:              LABIRINTO.c
*   Letras identificadoras:      LAB
*
*   Projeto: Disciplina INF1301
*   Departamento: DI/PUC-Rio
*   Autores: avc - Antônio Chaves
*            jpp - João Pedro Paiva
*            pc	 - Pedro Costa
*
*   $HA Histórico de evolução:
*       Versão  Autor   Data        Observações
*       1.00    jpp     22/09/2019  Início do desenvolvimento (LAB_cria)
*       2.00    jpp     02/10/2019  Acertar retornos e comentários
*		3.00	pc		08/10/2019	Adiciona estutura do labirinto e adequa as assinaturas das funções
*
*   $ED Descrição do módulo
*       Este módulo implementa um conjunto de funções para criar e
*       explorar labirinto.
*
***************************************************************************/

/***********************************************************************
*
*  $TC Tipo de dados: LAB Condicoes de retorno
*
*
***********************************************************************/
typedef enum
{

	LAB_CondRetOK = 0,
	/* Executou corretamente */

	LAB_CondRetProblemaMatriz = 1,
	/* Houve algum problema em operações com a matriz */

	LAB_CondRetArquivoTextoNulo = 2,
	/* Não foi possível abrir arquivo de texto */

	LAB_CondRetFaltouMemoria = 3,
	/* Não houve memória para mallocar um elemento do labirinto */

	LAB_CondRetTamanhoInvalido = 4,
	/* Execedeu-se o número de linhas ou colunas */

	LAB_CondRetCaractereInvalido = 5,
	/* Utilizou-se um caractere inválido na criação do labirinto */


} LAB_tpCondRet;


/***** Declarações exportadas pelo módulo *****/

/* Tipo referência para um labirinto */

typedef struct tgLabirinto* LAB_tppLabirinto;

/***************************************************
*
*   $FC Função: LAB Criar uma matriz com as dimensões necessárias e armazenar o labirinto nela.
*
*   $AE Assertivas de entrada esperadas:
*       Arquivo de texto possui no máximo 10 linhas
*       e 11 colunas (sendo a última coluna composta
*       por caracteres de linebreak).
*		Valem as assertivas estruturais do
*       labirinto.
*
*	$AS Assertivas de saída esperadas:
*       Labirinto foi criado com as dimensões
*       corretas.
*       Caracteres corretos foram adicionados à
*       matriz.
*       Dimensões corretas do labirinto foram
*       armazenadas nas variáveis globais
*       "linhas" e "colunas".
*		Valem as assertivas estruturais do
*       labirinto.
*
*   $EP Parâmetros
*       $P destruirElemento - O parâmetro que que passará a função de destruição do elemento dentro do labirinto.
*           Este parâmetro é passado por referência.
*       $P LabirintoASerCriado - O parâmetro que receberá o ponteiro para o labirinto.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       LAB_CondRetOK
*       LAB_CondRetArquivoTextoNulo
*       LAB_CondRetFaltouMemoria
*
****************************************************/
LAB_tpCondRet LAB_criaLabirinto(char *arquivo, LAB_tppLabirinto *LabirintoASerCriado);

/***************************************************
*
*	$FC Função:
*       LAB Descobre se o labirinto possui solução
*       e demarca a solução ou declara que não há
*       solução.
*
*   $AE Assertivas de entrada esperadas:
*       Nó corrente da matriz aponta para o nó de
*       inicio do labirinto.
*       Cabeça do labirinto != NULL.
*		Valem as assertivas estruturais do
*       labirinto.
*
*	$AS Assertivas de saída esperadas:
*       A solução do labirinto foi demarcada ou
*       declarada inexistente.
*		Valem as assertivas estruturais do
*       labirinto.
*
*   $EP Parâmetros
*       $P CabecaDoLabirinto - O parâmetro que receberá o ponteiro para o labirinto.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       LAB_CondRetOK
*       LAB_CondRetFaltouMemoria
*
****************************************************/
LAB_tpCondRet LAB_resolveLabirinto(LAB_tppLabirinto CabecaDoLabirinto);

/*******************************************************
*
*	$FC Função: LAB Imprime o labirinto.
*
*   $AE Assertivas de entrada esperadas:
*       Cabeça do labirinto != NULL.
*		Valem as assertivas estruturais do
*       labirinto.
*
*	$AS Assertivas de saída esperadas:
*       Labirinto foi impresso.
*		Valem as assertivas estruturais do
*       labirinto.
*
*   $EP Parâmetros
*       $P CabecaDoLabirinto - O parâmetro que receberá o ponteiro para o labirinto.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       LAB_CondRetOK
*		LAB_CondRetProblemaMatriz
*
********************************************************/
LAB_tpCondRet LAB_imprimeLabirinto(LAB_tppLabirinto CabecaDaLabirinto);

/*******************************************************
*
*	$FC Função: LAB Destroi o labirinto.
*
*   $AE Assertivas de entrada esperadas:
*       Cabeça do labirinto != NULL.
*		Valem as assertivas estruturais do
*       labirinto.
*
*	$AS Assertivas de saída esperadas:
*       Labirinto foi destruído.
*		Valem as assertivas estruturais do
*       labirinto.
*
*   $EP Parâmetros
*       $P CabecaDoLabirinto - O parâmetro que receberá o ponteiro para o labirinto.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       LAB_CondRetOK
*
********************************************************/
void LAB_destroiLabirinto(LAB_tppLabirinto CabecaDaLabirinto);