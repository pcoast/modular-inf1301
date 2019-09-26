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
*
***************************************************************************/

#include "LABIRINTO.H"
#include "MATRIZ.H"
#include <stdlib.h>

/***** Protótipos das funções encapsuladas no módulo *****/

/*****  Código das funções exportadas pelo módulo  *****/

LAB_tpCondRet LAB_cria(FILE *fp, MAT_tppMatriz* mariz)
{
    return LAB_CondRetOK;
}


int main(void)
{
    FILE *fp;
    int c;

    fp = fopen("LABtxt.TXT", "r"); // error check this!

    // this while-statement assigns into c, and then checks against EOF:

    while ((c = fgetc(fp)) != EOF)
    {
        if (c == '\n')
            printf("a\n");
        else
            printf("%c", c);
    }

    fclose(fp);

    return 0;
}