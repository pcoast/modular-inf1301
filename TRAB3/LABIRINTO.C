/***************************************************************************
*   $MCI Módulo de implementação: Módulo Labirinto
*
*   Arquivo gerado:              LABIRINTO.C
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
*       1.00    jpp     22/09/2019  Início do desenvolvimento
*       2.00    jpp     27/09/2019  LAB_criaLabirinto()
*       3.00    jpp     29/09/2019  LAB_imprimeLabirinto()
*       4.00    jpp     02/10/2019  Acertar retornos e comentários
*		5.00	pc		08/10/2019	Adiciona estrutura do labirinto e
*                                   adequa a implementação das funções
*		6.00	jpp		30/10/2019	Struct elemento de labirinto, função
*                                   resolve labirinto, free na função
*                                   imprime labirinto e função altera
*                                   trecho labirinto
*		6.01	pc		07/11/2019	Adiciona tracer e corrige alguns erros
*
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "MATRIZ.H"
#include "LABIRINTO.H"

//#define _DEBUG_RESOLVE

/***********************************************************************
*
*   $TC Tipo de dados:
*       ARV Descritor da cabeça de uma matriz.
*
*
*   $ED Descrição do tipo:
*       A cabeça da matriz é o ponto de acesso para uma determinada
*       matriz.
*       Por intermédio da referência para o nó corrente e dos
*       ponteiros
*       adjascentes pode-se navegar a matriz sem necessitar de uma
*       pilha.
*
***********************************************************************/

/***** Tipos encapsulados no módulo *****/

typedef struct tgLabirinto
{
    MAT_tppMatriz matriz;
} tpLabirinto;

typedef struct tgElementoLabirinto
{
    char paredeCaminhoEntradaOuSaida;
    unsigned char numPassadas;
    MAT_tpDir volta;

} tpElementoLabirinto;

/***** Variáveis encapsuladas no módulo *****/

static char colunas, linhas, colunaInicio, linhaInicio;

/***** Protótipos das funções encapsuladas no módulo *****/

static LAB_tpCondRet LAB_converteCondRet(MAT_tpCondRet CondicaoDeRetornoMatriz);

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************
*
*	$FC Função:
*       LAB Converter condições de retorno do
*       módulo de matriz para condições de retorno
*       do módulo de labirinto.
*
*
*   $AE Assertivas de entrada esperadas:
*       A condição de retorno de matriz está
*       correta.
*
*
*	$AS Assertivas de saída esperadas:
*       Condição de retorno foi convertida
*       corretamente.
*
****************************************************/
LAB_tpCondRet LAB_converteCondRet(MAT_tpCondRet CondicaoDeRetornoMatriz)
{
    if (CondicaoDeRetornoMatriz == MAT_CondRetFaltouMemoria)
        return LAB_CondRetFaltouMemoria;
    return LAB_CondRetProblemaMatriz;
}

/***************************************************
*
*	$FC Função:
*       LAB Criar o Labirinto armazenando os
*       caracteres corretos (inferidos a partir do
*       arquivo de texto) numa matriz criada
*       pela função MAT_criaMatriz().
*       Definir os valores globais das
*       dimensões do labirinto.
*
*
*   $AE Assertivas de entrada esperadas:
*       Arquivo de texto possui no máximo 10 linhas
*       e 11 colunas (sendo a última coluna composta
*       por caracteres de linebreak).
*       Labirinto possui possição de início (demarcada
*       pelo caractere 'I' no arquivo de texto) mas
*       não precisa de posição final (demarcada pelo
*       caractere 'F' no arquivo de texto).
*       Labirinto possui perímetro de paredes,
*       (demarcadas pelos caracteres '|' ou '-' no
*       arquivo de texto). 
*		Valem as assertivas estruturais do
*       labirinto.
*
*
*	$AS Assertivas de saída esperadas:
*       Labirinto foi criado com as dimensões
*       corretas.
*       Caracteres corretos foram adicionados à
*       matriz.
*       Dimensões corretas do labirinto foram
*       armazenadas nas variáveis globais
*       "linhas" e "colunas".
*       Posição de inicio do labirinto foi
*       armazenada nas variáveis "colunaInicio"
*       e "linhaInicio".
*       Nó corrente da matriz aponta para a posição
*       de inicio do labirinto.
*		Valem as assertivas estruturais do
*       labirinto.
*
****************************************************/
LAB_tpCondRet LAB_criaLabirinto(char *arquivo, LAB_tppLabirinto *LabirintoASerCriado)
{
	int i, colCount = 0, lineCount = 0, existeInicio = 0, existeFim = 0;
    FILE *fp;
    char CaractereTexto, CaractereLabirinto;
    MAT_tpCondRet CondRetDeMatriz;
    tpElementoLabirinto *elementoLabirinto;

    MAT_tppMatriz *matriz = (MAT_tppMatriz *)malloc(sizeof(MAT_tppMatriz));

    fp = fopen(arquivo, "r"); /* Abre arquivo de texto no modo leitura */

    if (!fp)
        return LAB_CondRetArquivoTextoNulo; /* fp == NULL retorna condição de arquivo de texto nulo */

#ifdef _DEBUG
	printf("\n(LAB_criaLabirinto) Abriu o arquivo;");
#endif

    colunas = 0;
    linhas = 0; /* Inicia contadores de coluna e de linha */

    while ((CaractereTexto = fgetc(fp)) != EOF) /* Loop para saber as dimensões do labirinto contando caracteres */
    {

        if (CaractereTexto == '\n')
        {
            linhas++; /* Conta número de linhas pelo caractere de linebreak (\n) no final de cada linha do texto */
            if (colCount != colunas)
                return LAB_CondRetTamanhoInvalido;
            colCount = 0;
        }
        else
            colCount++;
        if (!linhas)
            colunas++; /* Conta número de colunas pelo número de carácteres da primeira linha do texto */
    }

    if (++linhas > 10 || colunas > 10)
        return LAB_CondRetTamanhoInvalido; /* Soma 1 ao número de linhas porque o contador não conta com a última */

#ifdef _DEBUG
	printf("\n(LAB_criaLabirinto) Leu o numero de linhas/colunas;");
#endif

	CondRetDeMatriz = MAT_cria(linhas, colunas, free, matriz); /* Cria matriz para armazenar o labirinto */

#ifdef _DEBUG
	printf("\n(LAB_criaLabirinto) Matriz que armazena o labirinto criada;");
#endif

    if (CondRetDeMatriz != MAT_CondRetOK)
        return LAB_converteCondRet(CondRetDeMatriz); /* Retorna condição de retorno de matriz se algo estiver incorreto */

    (*LabirintoASerCriado) = (LAB_tppLabirinto)malloc(sizeof(tpLabirinto));
    if (!(*LabirintoASerCriado))
        return LAB_CondRetFaltouMemoria;

    (*LabirintoASerCriado)->matriz = *matriz;

    rewind(fp);

    colCount = 0;
    while ((CaractereTexto = fgetc(fp)) != EOF) /* Loop de preencher labirinto */
    {

        if (CaractereTexto == '\n') /* Loop coloca nó corrente no primeiro nó da linha de baixo quando chegar no
									final da linha atual (caractere de linebreak) */
        {
            if (++lineCount < linhas)
            {
                MAT_vaiParaBaixo((*LabirintoASerCriado)->matriz);

                for (i = 1; i < colunas; i++)
                    MAT_vaiParaEsquerda((*LabirintoASerCriado)->matriz);

                colCount = 0;
            }
        }
        else if (colCount < colunas)
        {

            if (CaractereTexto == '|' || CaractereTexto == '-')
                CaractereLabirinto = 'p'; /* Parede */

            else if (CaractereTexto == 'I')
            {
                CaractereLabirinto = 'i'; /* Início do labirinto */
                colunaInicio = colCount;
                linhaInicio = lineCount;
				existeInicio = 1;
            }

			else if (CaractereTexto == 'F')
			{
                CaractereLabirinto = 'f'; /* Fim do labirinto */
				existeFim = 1;
			}

            else if (CaractereTexto == ' ')
                CaractereLabirinto = 'c'; /* Caminho */

            else
                return LAB_CondRetCaractereInvalido;

            elementoLabirinto = (tpElementoLabirinto *)malloc(sizeof(tpElementoLabirinto)); /* Malloc do Elemento do Labirinto */

            if (!elementoLabirinto)
                return LAB_CondRetFaltouMemoria; /* Se elementoLabirinto == NULL, retorna condição de insufuciência de memória */

            elementoLabirinto->numPassadas = 0;

            elementoLabirinto->volta = MAT_DirNenhuma;

            elementoLabirinto->paredeCaminhoEntradaOuSaida = CaractereLabirinto;

            MAT_inserir((*LabirintoASerCriado)->matriz, elementoLabirinto);

            if (++colCount < colunas)
                MAT_vaiParaDireita((*LabirintoASerCriado)->matriz);
        }
    }

#ifdef _DEBUG
	printf("\n(LAB_criaLabirinto) Loop para preencher a matriz realizado;");
#endif

    fclose(fp); /* Fecha arquivo de texto */

	if (!existeInicio | !existeFim) {
		LAB_destroiLabirinto(*LabirintoASerCriado);
		return LAB_CondRetCaractereInvalido;
	}

    MAT_vaiParaPos((*LabirintoASerCriado)->matriz, 0, 0); /* Coloca nó corrente na mesma posição que o primeiro nó */

#ifdef _DEBUG
	printf("\n(LAB_criaLabirinto) Colocou a posição junto em (%d, %d);", linhaInicio, colunaInicio);
#endif

    return LAB_CondRetOK;
}

/***************************************************
*
*	$FC Função:
*       LAB Descobre se o labirinto possui solução
*       e demarca a solução ou declara que não há
*       solução.
*
*
*   $AE Assertivas de entrada esperadas:
*       Nó corrente da matriz aponta para a
*       posição inicial do labirinto.
*       Cabeça do labirinto != NULL.
*		Valem as assertivas estruturais do
*       labirinto.
*
*
*	$AS Assertivas de saída esperadas:
*       A solução do labirinto foi demarcada ou
*       declarada inexistente.
*		Valem as assertivas estruturais do
*       labirinto.
*
****************************************************/
LAB_tpCondRet LAB_resolveLabirinto(LAB_tppLabirinto CabecaDoLabirinto)
{

    MAT_tpDirOuInt direcaoCandidata;
    MAT_tpDirOuInt direcaoMenorNumPassadas;
    MAT_tpDirOuInt Volta;

    tpElementoLabirinto *ElementoDaPosicaoCandidata;
    tpElementoLabirinto *ElementoDaPosicaoAtual;

    int respostaRandom;

#ifdef _DEBUG_RESOLVE
	int count =0;
	printf("(LAB_resolveLabirinto) Começo vai!");
#endif // _DEBUG


    unsigned char menorNumPassadas;
    unsigned short labirintoPossuiSolucao;

	MAT_vaiParaPos(CabecaDoLabirinto->matriz, colunaInicio, linhaInicio); /* Coloca nó corrente na mesma posição que o primeiro nó */

    labirintoPossuiSolucao = USHRT_MAX * (linhas * colunas);

    MAT_obterElemento(CabecaDoLabirinto->matriz, &ElementoDaPosicaoAtual); /* Obtem elemento da posição inicial do labirinto */

    while (labirintoPossuiSolucao--) /* Se completamos um certo número de movimentos e não encomtramos a saída, então ela não existe ou não é alcançâvel */
    /* Loop de caminhar pelo Labirinto, primeiro decidimos a direção de menor passadas, depois damos um passo nesta direção */
    {
#ifdef _DEBUG_RESOLVE
		printf("\n(LAB_resolveLabirinto) Iter: %d", ++count);
#endif // _DEBUG_RESOLVE

        direcaoCandidata.tipoDirecao = MAT_DirCima;           /* Primeira direção a ser analizada é a de cima */
        menorNumPassadas = UCHAR_MAX;                         /* Menor número de passadas (espécie de mínimo) é, inicialmente, o maior número possível */
        direcaoMenorNumPassadas.tipoDirecao = MAT_DirNenhuma; /* Direção de menor número de passadas é, inicialmente, direção nenhuma */
        /* Se nenhuma direção de menor número de passadas for escolhida, então ela permanece como direção nenhuma e saberemos que estamos numa posição sem saída.
        Neste caso, desempilhamos ou voltamos para a posição inicial */

        do /* Loop que compara o número de passadas das quatro direções e escolhe a direção com o menor número de passadas.  */
        {

            /* Funciona da seguinte forma: Caminhamos na direção sendo analizada no ciclio atual, indo para a posição candidata. Pegamos o elemento da posição candidata. Voltamos para
            o nó que estávamos, indo para a posição atual. Comparamos o número de passadas contido no elemento da posição candidata com o número de passadas mínimo (menorNumPassadas) */

			switch (direcaoCandidata.tipoDirecao)
			{
				case MAT_DirCima:
					Volta.tipoDirecao = MAT_DirBaixo;
					break;
				case MAT_DirBaixo:
					Volta.tipoDirecao = MAT_DirCima;
					break;
				case MAT_DirDireita:
					Volta.tipoDirecao = MAT_DirEsquerda;
					break;
				case MAT_DirEsquerda:
					Volta.tipoDirecao = MAT_DirDireita;
					break;
			} /* Define a direção para voltar da posição candidata para a posição atual */

            MAT_vaiParaDir(CabecaDoLabirinto->matriz, direcaoCandidata.tipoDirecao);   /* Vai para a posição candidata */
            MAT_obterElemento(CabecaDoLabirinto->matriz, &ElementoDaPosicaoCandidata); /* Obtem elemento da posição candidata */
            MAT_vaiParaDir(CabecaDoLabirinto->matriz, Volta.tipoDirecao);              /* Volta da posição candidata para a posição atual */

            if (ElementoDaPosicaoAtual->volta != direcaoCandidata.tipoDirecao && ElementoDaPosicaoCandidata->paredeCaminhoEntradaOuSaida != 'p') /* Se a direção da posição candidata não é a
            direção de volta para a posição atual anterior, ou seja, não estamos voltando para o caminho do qual viemos, e se não estamos indo de encontro com uma parede */
            {
                if (ElementoDaPosicaoCandidata->numPassadas <= menorNumPassadas)
                /* Se o número de passadas da posição candidata for menor do que o menor número de passadas de outras posições candidatas */
                {
                    menorNumPassadas = ElementoDaPosicaoCandidata->numPassadas;         /* O mínimo de passadas se torna o número de passadas da posição candidata */
                    direcaoMenorNumPassadas.tipoDirecao = direcaoCandidata.tipoDirecao; /* A direção do mínimo de passadas se torna a direção da posição candidata */
                }
            }

            direcaoCandidata.tipoInteiro++; /* Muda direção candidata (Cima -> Direita -> Baixo -> Esquerda -> Direção Nenhuma) */

        } while (direcaoCandidata.tipoDirecao != MAT_DirNenhuma); /* Condição é quebrada quando o ciclo das direções candidatas é completo (chegamos na última direção == direção nenhuma) */



        if (direcaoMenorNumPassadas.tipoDirecao == MAT_DirNenhuma) /* Se não trocou a direção de menor número de passadas, ou seja, se a posição atual só pode ir de encontro com uma 
        parede ou voltar para o caminho de onde viemos, então sesempilhamos ou voltamos para a posição de início do labirinto */
        {

            while (ElementoDaPosicaoAtual->paredeCaminhoEntradaOuSaida != 'i') /* Desempilha ou volta até a posição inicial */
            {
                ElementoDaPosicaoAtual->paredeCaminhoEntradaOuSaida = 'c';             /* Posição atual é marcada como caminho */
                ElementoDaPosicaoAtual->numPassadas++;                                 /* Aumenta número de passadas da posição atual */
                Volta.tipoDirecao = ElementoDaPosicaoAtual->volta;                     /* Guarda direção de volta na variável 'Volta' */
                ElementoDaPosicaoAtual->volta = MAT_DirNenhuma;                        /* Volta da posição atual é direção nenhuma */
                MAT_vaiParaDir(CabecaDoLabirinto->matriz, Volta.tipoDirecao);          /* Volta para a posição de onde viemos */
                MAT_obterElemento(CabecaDoLabirinto->matriz, &ElementoDaPosicaoAtual); /* Obtem elemento da nova posição atual */
            }
        }

        else /* Existe caminho em alguma direção que não é a de volta */
        {

            MAT_vaiParaDir(CabecaDoLabirinto->matriz, direcaoMenorNumPassadas.tipoDirecao); /* Dá um passo na direção de menor número de passadas */
            MAT_obterElemento(CabecaDoLabirinto->matriz, &ElementoDaPosicaoAtual);          /* Obtem elemento da nova posição atual */

            if (ElementoDaPosicaoAtual->paredeCaminhoEntradaOuSaida == 'f') /* Se posição para a qual andamos é o final do labirinto, então há solução e ela foi demarcada. 
            Labirinto foi resolvido */
            {
                printf("\nChegamos ao final do Labirinto");
                return LAB_CondRetOK;
            }

            /* Se posição para a qual andamos não é o final do labirinto */

            Volta.tipoInteiro = (direcaoMenorNumPassadas.tipoInteiro + 2) % 4; /* Definimos a direção de volta para a posição anterior */

            ElementoDaPosicaoAtual->volta = Volta.tipoDirecao; /* Armazaena direção de volta no nó para o qual caminhamos */

            ElementoDaPosicaoAtual->paredeCaminhoEntradaOuSaida = '*'; /* Marca trajeto */

            /* Vamos escolher uma nova direção para continuar caminhando */
        }
    }

    printf("\nLabirinto nao possui solucao");

    return LAB_CondRetOK;
}

/***************************************************
*
*	$FC Função:
*       LAB Troca trecho da posição atual do
*           labirinto. Se for parede, vira caminho
*           se for caminho, vira parede.
*
*
*   $AE Assertivas de entrada esperadas:
*       Cabeça do labirinto != NULL.
*       Nó corrente aponta para a posição cujo
*       trecho deseja-se que seja modificado.
*		Valem as assertivas estruturais do
*       labirinto.
*
*
*	$AS Assertivas de saída esperadas:
*       O trecho no nó corrente foi alterado.
*		Valem as assertivas estruturais do
*       labirinto.
*
****************************************************/
LAB_tpCondRet LAB_alteraTrechoDoLabirinto(LAB_tppLabirinto CabecaDoLabirinto, char i, char j)
{

    tpElementoLabirinto *ElementoDaPosicaoCorrente;

	MAT_vaiParaPos(CabecaDoLabirinto->matriz, j, i);
    MAT_obterElemento(CabecaDoLabirinto->matriz, &ElementoDaPosicaoCorrente); /* Obtem elemento da posição inicial do labirinto */

    if (ElementoDaPosicaoCorrente->paredeCaminhoEntradaOuSaida == 'p') /* Se posição atual do labirinto é parede */

        ElementoDaPosicaoCorrente->paredeCaminhoEntradaOuSaida = 'c'; /* A parede se torna um caminho */

    else if (ElementoDaPosicaoCorrente->paredeCaminhoEntradaOuSaida == 'c' || ElementoDaPosicaoCorrente->paredeCaminhoEntradaOuSaida == '*') /* Se posição atual do labirinto é caminho */

        ElementoDaPosicaoCorrente->paredeCaminhoEntradaOuSaida = 'p'; /* O caminho se torna uma parede */

    MAT_inserir(CabecaDoLabirinto->matriz, ElementoDaPosicaoCorrente);

    return LAB_CondRetOK;
}

/***************************************************
*
*	$FC Função:
*       LAB Imprimir o labirinto.
*
*
*   $AE Assertivas de entrada esperadas:
*       Cabeça do labirinto != NULL.
*		Valem as assertivas estruturais do
*       labirinto.
*
*
*	$AS Assertivas de saída esperadas:
*       Labirinto foi impresso.
*		Valem as assertivas estruturais do
*       labirinto.
*
****************************************************/
LAB_tpCondRet LAB_imprimeLabirinto(LAB_tppLabirinto CabecaDoLabirinto)
{
    int l, c, i;

    tpElementoLabirinto **ptElementoLabirinto = (tpElementoLabirinto **)malloc(sizeof(tpElementoLabirinto *)); /* Malloc do Elemento do Labirinto */

#ifdef _DEBUG
	printf("\n(LAB_imprimeLabirinto) Alocou o espaco para os elementos do labirinto;");
#endif

    if (!ptElementoLabirinto)
        return LAB_CondRetFaltouMemoria; /* Se ptElementoLabirinto == NULL, retorna condição de insufuciência de memória */

    MAT_vaiParaPos(CabecaDoLabirinto->matriz, 0, 0); /* Coloca nó corrente na mesma posição que o primeiro nó */

#ifdef _DEBUG
	printf("\n(LAB_imprimeLabirinto) Colocou o 'cursor' no inicio da matriz;");

	printf("\n(LAB_imprimeLabirinto) linhas: %d, colunas: %d;", linhas, colunas);
#endif
	printf("\n");

    for (l = 0; l < linhas; l++)
    {
#ifdef _DEBUG
		c = 0;
#endif // _DEBUG

		do {
#ifdef _DEBUG
			printf("\n(LAB_imprimeLabirinto) (%d,%d)", l,c);
#endif
			MAT_obterElemento(CabecaDoLabirinto->matriz, ptElementoLabirinto);

#ifdef _DEBUG
			printf("\n(LAB_imprimeLabirinto) Obteve o elemento da matriz;");
#endif
			printf("%c ", (*ptElementoLabirinto)->paredeCaminhoEntradaOuSaida);

			if ((*ptElementoLabirinto)->paredeCaminhoEntradaOuSaida == '*')
				(*ptElementoLabirinto)->paredeCaminhoEntradaOuSaida = 'c';

#ifdef _DEBUG
			printf("(LAB_resolveLabirinto) Limpou os caminhos antigos.");
			c++;
#endif // _DEBUG

		} while (MAT_vaiParaDireita(CabecaDoLabirinto->matriz) != MAT_CondRetNoNaoExiste);

        if (l < (linhas - 1))
        {
            printf("\n");

            MAT_vaiParaBaixo(CabecaDoLabirinto->matriz);

            for (i = 1; i < colunas; i++)
                MAT_vaiParaEsquerda(CabecaDoLabirinto->matriz);
        }
    }

	/* Limpa caminhos antigos */
	MAT_vaiParaPos(CabecaDoLabirinto->matriz, 0, 0);

    free(ptElementoLabirinto);

    return LAB_CondRetOK;
}

/***************************************************
*
*	$FC Função:
*       LAB Destruir o labirinto.
*
*
*   $AE Assertivas de entrada esperadas:
*       Cabeça do labirinto != NULL.
*		Valem as assertivas estruturais do
*       labirinto.
*
*
*	$AS Assertivas de saída esperadas:
*       Labirinto foi destruído.
*		Valem as assertivas estruturais do
*       labirinto.
*
****************************************************/
void LAB_destroiLabirinto(LAB_tppLabirinto CabecaDoLabirinto)
{
    MAT_destroi(CabecaDoLabirinto->matriz);
    free(CabecaDoLabirinto);
}