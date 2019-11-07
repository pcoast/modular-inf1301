/***************************************************************************
*   $MCI Módulo de implementação: Módulo principal
*
*   Arquivo gerado:              PRINCIPAL.C
*
*   Projeto: Disciplina INF1301
*   Departamento: DI/PUC-Rio
*   Autores: avc - Antônio Chaves
*            jpp - João Pedro Paiva
*            pc	 - Pedro Costa
*
*   $HA Histórico de evolução:
*       Versão  Autor   Data        Observações
*       1.00    pc		07/11/2019	Implementação inicial
*
***************************************************************************/
#include <stdio.h>
#include "LABIRINTO.H"
//#define _DEBUG

void printMenu(char menu, char loaded) {
	switch (menu) {
		case 0:
			printf("\n Tecle o numero indicado para selecionar uma opcao");
			printf("\n 1. Carregar um labirinto");
			if (loaded) {
				printf("\n 2. Resolver o labirinto");
				printf("\n 3. Alterar o labirinto");
			}
			printf("\n 0. Sair");
			break;
		case 1:
			printf("\n Insira o nome do arquivo a ser carregado (ex.: labirinto.txt): ");
			break;
		case 2:
			printf("\n Insira a posicao do elemento a ser alterado (ex.: 1,2): ");
			break;
		default:
			break;
	}
	
}

int main(void) {
	char menu = 0, exit = 0, loaded = 0, selection = 'a',
		arquivo[30];
		
	int line = 0, column = 0;
	LAB_tppLabirinto *lab = (LAB_tppLabirinto *)malloc(sizeof(LAB_tppLabirinto));

	while (!exit) {
		system("@cls||clear");
		printf("!!! LABIRINTO !!!\n");

#ifdef _DEBUG
		printf("\n Alterando elemento %d, %d\n", line, column);
#endif // _DEBUG

		switch (selection) {
			case '0':
				exit = 1;
				break;
			case '1':
				/* Logica para carregar labirinto */
				menu = 1;
				break;
			case '2':
				/* Logica para resolver labirinto */
				if (loaded) {
					LAB_resolveLabirinto(*lab);
					selection = getch();
				}
				break;
			case '3':
				/* Logica para alterar labirinto */
				menu = 2;
				break;
			default:
				break;
		}
	
		if (loaded)
			LAB_imprimeLabirinto(*lab);

		printf("\n\n");

		printMenu(menu, loaded);

		switch (menu) {
			case 1:
				scanf("%s", arquivo);
				printf("\n");
				if (LAB_criaLabirinto(arquivo, lab) == LAB_CondRetOK) {
					loaded = 1;
				}
				else {
					printf("(Enter) Erro! Arquivo texto invalido. Verifique as instrucoes de criacao do labirinto.");
					selection = getch();
				}
				menu = 0;
				break;
			case 2:
				scanf("%d,%d", &line, &column);
				printf("\n");
				LAB_alteraTrechoDoLabirinto(*lab, line, column);
				menu = 0;
			default:
				break;
		}


		selection = getch();
	}
}