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

    LAB_CondRetNoNaoExiste = 1,
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
*   $FC Função: LAB Criar a estrutura da labirinto,
*       criando os nós com a LAB_criaNo() e os
*       interconectando.
*
*   $EP Parâmetros
*       $P LinhasEColunas - O parâmetro que que passará o número de linhas e colunas da labirinto.
*           Este parâmetro é passado por valor.
*       $P LabirintoCriada - O parâmetro que receberá o ponteiro para a cabeça da labirinto.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       LAB_CondRetOK
*       LAB_CondRetFaltouMemoria
*
****************************************************/
LAB_tpCondRet LAB_cria(char LinhasEColunas,void (*destroiElemento) (void *elemento) ,LAB_tppLabirinto *LabirintoCriada);

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
LAB_tpCondRet LAB_vaiParaEsquerdaSuperior(LAB_tppLabirinto CabecaDaLabirinto);

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
LAB_tpCondRet LAB_vaiParaCima(LAB_tppLabirinto CabecaDaLabirinto);

/*******************************************************
*
*	$FC Função: LAB Nó corrente da Labirinto se torna o nó
*       à Direita Superior do nó corrente atual.
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
LAB_tpCondRet LAB_vaiParaDireitaSuperior(LAB_tppLabirinto CabecaDaLabirinto);

/*******************************************************
*
*	$FC Função: LAB Nó corrente da Labirinto se torna o nó
*       à Esquerda do nó corrente atual.
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
LAB_tpCondRet LAB_vaiParaEsquerda(LAB_tppLabirinto CabecaDaLabirinto);

/*******************************************************
*
*	$FC Função: LAB Nó corrente da Labirinto se torna o nó
*       à Direita do nó corrente atual.
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
LAB_tpCondRet LAB_vaiParaDireita(LAB_tppLabirinto CabecaDaLabirinto);

/*******************************************************
*
*	$FC Função: LAB Nó corrente da Labirinto se torna o nó
*       à Esquerda Inferior do nó corrente atual.
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
LAB_tpCondRet LAB_vaiParaEsquerdaInferior(LAB_tppLabirinto CabecaDaLabirinto);

/*******************************************************
*
*	$FC Função: LAB Nó corrente da Labirinto se torna o nó
*       Abaixo do nó corrente atual.
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
LAB_tpCondRet LAB_vaiParaBaixo(LAB_tppLabirinto CabecaDaLabirinto);

/*******************************************************
*
*	$FC Função: LAB Nó corrente da Labirinto se torna o nó
*       à Direita Inferior do nó corrente atual.
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
LAB_tpCondRet LAB_vaiParaDireitaInferior(LAB_tppLabirinto CabecaDaLabirinto);

/***************************************************
*
*	$FC Função: LAB Destruir labirinto linha por linha.
*
*   $EP Parâmetros
*       $P CabecaDaLabirinto - O parâmetro que receberá o ponteiro para a labirinto.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       LAB_CondRetOK
*       LAB_CondRetNoNaoExiste
*
****************************************************/
void LAB_destroi(LAB_tppLabirinto CabecaDaLabirinto);

/*******************************************************
*
*	$FC Função: LAB Inserir elemento no nó corrente da 
*       labirinto
*
*   $EP Parâmetros
*       $P CabecaDaLabirinto - O parâmetro que receberá o ponteiro para a labirinto.
*           Este parâmetro é passado por referência.
*       $P elemento - O parâmetro que passará o ponteiro para o elemento a ser incoporado no nó corrente.
*           Este parâmetro é passado por valor.
*
*   $FV Valor retornado
*       LAB_CondRetOK
*       LAB_CondRetNoOcupado
*
********************************************************/
LAB_tpCondRet LAB_inserir(LAB_tppLabirinto CabecaDaLabirinto, void *elemento);

/*******************************************************
*
*	$FC Função: LAB Obtém o elemento do nó corrente da labirinto
*
*   $EP Parâmetros
*       $P CabecaDaLabirinto - O parâmetro que receberá o ponteiro para a cabeça da labirinto.
*           Este parâmetro é passado por referência.
*       $P elem - O parâmetro que receberá o ponteiro para o elemento a ser obtida.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       LAB_CondRetOK
*       LAB_CondRetNoVazio
*
********************************************************/
LAB_tpCondRet LAB_obterElemento(LAB_tppLabirinto CabecaDaLabirinto, void **elemento);
