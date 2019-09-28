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
*       2.00    jpp     27/09/2019  LAB_criaLabirinto
*
***************************************************************************/

#include "LABIRINTO.H"
#include "MATRIZ.H"
#include <stdio.h>
#include <stdlib.h>

/***** Protótipos das funções encapsuladas no módulo *****/

/*****  Código das funções exportadas pelo módulo  *****/

LAB_tpCondRet LAB_criaLabirinto(void (*destruirElemento)(void *elemento))
{
    FILE *fp;
    char CaractereTexto, *CaractereLabirinto;
    char colunas, linhas;

    fp = fopen("LABTXT.TXT", "r");

    if (fp==NULL) return LAB_CondRetArquivoTextoNulo;

    colunas = linhas = 0;

    while ((CaractereTexto = fgetc(fp)) != EOF)
    {

        if (!linhas) colunas++;


        if (CaractereTexto=='\n') linhas++;

    }

    MAT_tppMatriz LabirintoASerCriado;

    MAT_criaMatriz(linhas,colunas,destruirElemento,LabirintoASerCriado);

    rewind(fp);

    while ((CaractereTexto = fgetc(fp)) != EOF)
    {

        if (CaractereTexto=='\n') /* Loop coloca nó corrente no primeiro nó da linha de baixo quando chegar no 
        final da linha atual (caractere de linebreak) */
        {
            MAT_vaiParaBaixo(LabirintoASerCriado);

            for (int i = 1; i<colunas; i++) MAT_vaiParaEsquerda(LabirintoASerCriado);
        }

        else
        {
            CaractereLabirinto = (char*) malloc(sizeof(char));

            if (CaractereLabirinto == NULL) return LAB_CondRetFaltouMemoria;

            else if (CaractereTexto=='|' || CaractereTexto=='-') *CaractereLabirinto = 'p'; /* Parede */

            else if (CaractereTexto=='I') *CaractereLabirinto = 'i'; /* Início do labirinto */

            else if (CaractereTexto=='F') *CaractereLabirinto = 'f'; /* Fim do labirinto */

            else *CaractereLabirinto = 'c'; /* Caminho */

            MAT_inserirElemento(LabirintoASerCriado,CaractereLabirinto);

            MAT_vaiParaDireita(LabirintoASerCriado);
        }
        

    }

    fclose(fp);

    return LAB_CondRetOK;
}