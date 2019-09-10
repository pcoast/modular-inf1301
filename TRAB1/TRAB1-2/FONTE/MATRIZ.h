/***************************************************************************
*   $MCI Módulo de definição: Módulo Matriz
*
*   Arquivo gerado:              MATRIZ.c
*   Letras identificadoras:      MAT
*
*   Projeto: Disciplina INF1301
*   Departamento: DI/PUC-Rio
*   Autores: avc - Antônio Chaves
*            jpp - João Pedro Paiva
*
*   $HA Histórico de evolução:
*       Versão  Autor   Data        Observações
*       1.00    avc     25/08/2019  Início do desenvolvimento (primeiros 
*                                   protótipos de funções)
*       2.00    jpp     03/09/2019  Continuação do desenvolvimento 
*                                   (Modificação e adição de protótipos)
*       3.00    jpp     04/09/2019  Uniformização da interface das funções e
*                                   das condições de retorno.
*
*   $ED Descrição do módulo
*       Este módulo implementa um conjunto de funções para criar e
*       explorar matrizes.
*       A matriz possui uma cabeça que contém uma referência para o primeiro 
*       nó da matriz e outra referência para um nó corrente da matriz.
*       O módulo permite a existência de múltiplas matrizes simultâneamente.
*       Ao iniciar a execução do programa não existe matriz. A matriz nunca 
*       está vazia.
*
***************************************************************************/


/***** Declarações exportadas pelo módulo *****/

/* Tipo referência para uma matriz */

typedef struct tgMatriz * MAT_tppMatriz;

/***********************************************************************
*
*  $TC Tipo de dados: MAT Condicoes de retorno
*
*
***********************************************************************/
typedef enum
{

    MAT_CondRetOK = 0,
    /* Executou corretamente */

    MAT_CondRetNoNaoExiste = 1,
    /* Tentou mudar o nó corrente para um nó inexistente (NULL) */

    MAT_CondRetFaltouMemoria = 2,
    /* Não houve memória para mallocar um dos nós da matriz */

    MAT_CondRetNoOcupado = 3,
    /* Tentou adicionar uma lista a um nó que já possuía lista */

    MAT_CondRetNoVazio = 4,
    /* Tentou obter a lista de um nó que não possuía lista nenhuma */

} MAT_tpCondRet;

/***************************************************
*
*   $FC Função: MAT Criar a estrutura da matriz,
*       criando os nós com a MAT_criaNo() e os
*       interconectando.
*
*  $EP Par�metros
*       $P LinhasEColunas - O parâmetro que que passará o número de linhas e colunas da matriz.
*           Este parâmetro é passado por valor.
*       $P MatrizCriada - O parâmetro que receberá o ponteiro para a cabeça da matriz.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       MAT_CondRetOK
*       MAT_CondRetFaltouMemoria
*
****************************************************/
MAT_tpCondRet MAT_cria(char LinhasEColunas,void (*destroiElemento) (void *elemento) ,MAT_tppMatriz *MatrizCriada);

/*******************************************************
*
*   $FC Função: MAT Nó corrente da Matriz se torna o nó
*       à Esquerda Superior do nó corrente atual.
*
*   $EP Parâmetros
*       $P CabecaDaMatriz - O parâmetro que receberá o ponteiro para a matriz.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       MAT_CondRetOK
*
********************************************************/
MAT_tpCondRet MAT_vaiParaEsquerdaSuperior(MAT_tppMatriz CabecaDaMatriz);

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       Acima do nó corrente atual.
*
*   $EP Parâmetros
*       $P CabecaDaMatriz - O parâmetro que receberá o ponteiro para a matriz.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       MAT_CondRetOK
*       MAT_CondRetNoNaoExiste
*
********************************************************/
MAT_tpCondRet MAT_vaiParaCima(MAT_tppMatriz CabecaDaMatriz);

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       à Direita Superior do nó corrente atual.
*
*   $EP Parâmetros
*       $P CabecaDaMatriz - O parâmetro que receberá o ponteiro para a matriz.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       MAT_CondRetOK
*       MAT_CondRetNoNaoExiste
*
********************************************************/
MAT_tpCondRet MAT_vaiParaDireitaSuperior(MAT_tppMatriz CabecaDaMatriz);

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       à Esquerda do nó corrente atual.
*
*   $EP Parâmetros
*       $P CabecaDaMatriz - O parâmetro que receberá o ponteiro para a matriz.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       MAT_CondRetOK
*       MAT_CondRetNoNaoExiste
*
********************************************************/
MAT_tpCondRet MAT_vaiParaEsquerda(MAT_tppMatriz CabecaDaMatriz);

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       à Direita do nó corrente atual.
*
*   $EP Parâmetros
*       $P CabecaDaMatriz - O parâmetro que receberá o ponteiro para a matriz.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       MAT_CondRetOK
*       MAT_CondRetNoNaoExiste
*
********************************************************/
MAT_tpCondRet MAT_vaiParaDireita(MAT_tppMatriz CabecaDaMatriz);

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       à Esquerda Inferior do nó corrente atual.
*
*   $EP Parâmetros
*       $P CabecaDaMatriz - O parâmetro que receberá o ponteiro para a matriz.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       MAT_CondRetOK
*       MAT_CondRetNoNaoExiste
*
********************************************************/
MAT_tpCondRet MAT_vaiParaEsquerdaInferior(MAT_tppMatriz CabecaDaMatriz);

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       Abaixo do nó corrente atual.
*
*   $EP Parâmetros
*       $P CabecaDaMatriz - O parâmetro que receberá o ponteiro para a matriz.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       MAT_CondRetOK
*       MAT_CondRetNoNaoExiste
*
********************************************************/
MAT_tpCondRet MAT_vaiParaBaixo(MAT_tppMatriz CabecaDaMatriz);

/*******************************************************
*
*	$FC Função: MAT Nó corrente da Matriz se torna o nó
*       à Direita Inferior do nó corrente atual.
*
*   $EP Parâmetros
*       $P CabecaDaMatriz - O parâmetro que receberá o ponteiro para a matriz.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       MAT_CondRetOK
*       MAT_CondRetNoNaoExiste
*
********************************************************/
MAT_tpCondRet MAT_vaiParaDireitaInferior(MAT_tppMatriz CabecaDaMatriz);

/***************************************************
*
*	$FC Função: MAT Destruir matriz linha por linha.
*
*   $EP Parâmetros
*       $P CabecaDaMatriz - O parâmetro que receberá o ponteiro para a matriz.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       MAT_CondRetOK
*       MAT_CondRetNoNaoExiste
*
****************************************************/
void MAT_destroi(MAT_tppMatriz CabecaDaMatriz);

/*******************************************************
*
*	$FC Função: MAT Inserir elemento no nó corrente da 
*       matriz
*
*   $EP Parâmetros
*       $P CabecaDaMatriz - O parâmetro que receberá o ponteiro para a matriz.
*           Este parâmetro é passado por referência.
*       $P elemento - O parâmetro que passará o ponteiro para o elemento a ser incoporado no nó corrente.
*           Este parâmetro é passado por valor.
*
*   $FV Valor retornado
*       MAT_CondRetOK
*       MAT_CondRetNoOcupado
*
********************************************************/
MAT_tpCondRet MAT_inserir(MAT_tppMatriz CabecaDaMatriz, void *elemento);

/*******************************************************
*
*	$FC Função: MAT Obtém o elemento do nó corrente da matriz
*
*   $EP Parâmetros
*       $P CabecaDaMatriz - O parâmetro que receberá o ponteiro para a cabeça da matriz.
*           Este parâmetro é passado por referência.
*       $P elem - O parâmetro que receberá o ponteiro para o elemento a ser obtida.
*           Este parâmetro é passado por referência.
*
*   $FV Valor retornado
*       MAT_CondRetOK
*       MAT_CondRetNoVazio
*
********************************************************/
MAT_tpCondRet MAT_obterElemento(MAT_tppMatriz CabecaDaMatriz, void **elemento);
