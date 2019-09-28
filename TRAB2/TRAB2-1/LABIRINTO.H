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
*
*   $ED Descrição do módulo
*       Este módulo implementa um conjunto de funções para criar e
*       explorar labirinto.
*
***************************************************************************/


/***** Declarações exportadas pelo módulo *****/

/* Tipo referência para uma labirinto */

typedef struct tgLabirinto * LAB_tppLabirinto;

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

    LAB_CondRetArquivoTextoNulo = 1,
    /* Tentou mudar o nó corrente para um nó inexistente (NULL) */

    LAB_CondRetFaltouMemoria = 2,
    /* Não houve memória para mallocar um dos nós da labirinto */

    LAB_CondRetNoOcupado = 3,
    /* Tentou adicionar uma lista a um nó que já possuía lista */

    LAB_CondRetNoVazio = 4,
    /* Tentou obter a lista de um nó que não possuía lista nenhuma */

} LAB_tpCondRet;

/***************************************************
*
*   $FC Função: LAB Criar uma matriz com as dimensões necessárias e armazenar o labirinto nela.
*
*   $EP Parâmetros
*       $P LinhasEColunas - O parâmetro que que passará o número de linhas e colunas da labirinto.
*           Este parâmetro é passado por valor.
*
*   $FV Valor retornado
*       LAB_CondRetOK
*       LAB_CondRetArquivoTextoNulo
*       LAB_CondRetFaltouMemoria
*
****************************************************/
LAB_tpCondRet LAB_criaLabirinto(void (*destruirElemento)(void *elemento));

/*******************************************************
*
*   $FC Função: LAB Nó corrente da Labirinto se torna o nó
*       à Esquerda Superior do nó corrente atual.
*
*   $EP Parâmetros
*       $P CabecaDaLabirinto - O parâmetro que receberá o ponteiro para a labirinto.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       LAB_CondRetOK
*
********************************************************/
LAB_tpCondRet LAB_resolveLabirinto(LAB_tppLabirinto CabecaDaLabirinto);

/*******************************************************
*
*	$FC Função: LAB Nó corrente da Labirinto se torna o nó
*       Acima do nó corrente atual.
*
*   $EP Parâmetros
*       $P CabecaDaLabirinto - O parâmetro que receberá o ponteiro para a labirinto.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       LAB_CondRetOK
*       LAB_CondRetNoNaoExiste
*
********************************************************/
LAB_tpCondRet LAB_imprimeLabirinto(LAB_tppLabirinto CabecaDaLabirinto);