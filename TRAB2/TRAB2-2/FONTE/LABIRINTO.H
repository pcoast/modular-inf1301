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

	LAB_CondRetNoNaoExiste = 1,
	/* Condição de retorno da Matriz convertida para Labirinto. Tentou mudar o nó corrente para um nó inexistente (NULL) */

	LAB_CondRetFaltouMemoriaNaMatriz = 2,
	/* Condição de retorno da Matriz convertida para Labirinto. Não houve memória para mallocar um dos nós da matriz ou a cabeça da matriz */

	LAB_CondRetNoOcupado = 3,
	/* Condição de retorno da Matriz convertida para Labirinto. Tentou adicionar um elemento a um nó que já possuía elemento */

	LAB_CondRetNoVazio = 4,
	/* Condição de retorno da Matriz convertida para Labirinto. Tentou obter o elemento de um nó que não possuía elemento algum */

	LAB_CondRetArquivoTextoNulo = 5,
	/* Não foi possível abrir arquivo de texto */

	LAB_CondRetFaltouMemoriaNoLabirinto = 6,
	/* Não houve memória para mallocar um elemento do labirinto */


} LAB_tpCondRet;

/***************************************************
*
*   $FC Função: LAB Criar uma matriz com as dimensões necessárias e armazenar o labirinto nela.
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
//LAB_tpCondRet LAB_criaLabirinto(void (*destruirElemento)(void *elemento), MAT_tppMatriz LabirintoASerCriado);

/*******************************************************
*
*	$FC Função: LAB Imprime o labirinto.
*
*   $EP Parâmetros
*       $P CabecaDoLabirinto - O parâmetro que receberá o ponteiro para o labirinto.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       LAB_CondRetOK
*
********************************************************/
//LAB_tpCondRet LAB_imprimeLabirinto(MAT_tppMatriz CabecaDaLabirinto);

/*******************************************************
*
*	$FC Função: LAB Destroi o labirinto.
*
*   $EP Parâmetros
*       $P CabecaDoLabirinto - O parâmetro que receberá o ponteiro para o labirinto.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       LAB_CondRetOK
*
********************************************************/
void LAB_destroiLabirinto(MAT_tppMatriz CabecaDaLabirinto);