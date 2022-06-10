#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINHAS 5
#define COLUNAS 200

int main(int argc, char const *argv[])
{
	int num;
	int valAtual = 0, i, j;
	int pid, status;

	int matriz[LINHAS][COLUNAS];
	//matriz=(int (*)[COLUNAS])malloc(sizeof(*matriz)*LINHAS);

	// para 3 linhas e 7 colunas
	// se o cpu conseguir correr 8 processos,  por exemplo, não poderia criar processos 
	// célula a célula pois necessitariamos de 21 processos, e o sistema teria de escalonar processos para
	// dar tempo de cpu a cada processo
	//--- se criarmos processos por linha, necessitavamos de 3 processos, que já dava
	//--- se criarmos processos por coluna, necessitavamos de 8 processos que ainda dava, mas talvez nao seja o melhor
	// DEPENDE SEMPRE DO NOSSO SISTEMA	

	// o objetivo é encontrar o SWEETSPOT ---> o qué dá o melhor tempo


	// LEI UNIVERSAL DA ESCALONABILIDADE

	

	for(i = 0; i < LINHAS; i++){
		for (j = 0; j < COLUNAS; j++)
		{
			matriz[i][j] = rand() % 100;
			//valAtual++;
		}
	}


/*
	for(i = 0; i < LINHAS; i++){
		for (j = 0; j < COLUNAS; j++)
		{
			printf("%d ", matriz[i][j]);
		}
		printf("\n");
	}

	printf("\n");
*/

	num = rand() % 100;

	for (i = 0; i < LINHAS; i++){
		
		if ((pid = fork()) == 0){ 
			//vai procurar por linhas
			for (j = 0; j < COLUNAS; j++){
				if (matriz[i][j] == num){
					_exit(i);
				}
			}
			_exit(0);
		}
	}

	for(i = 0; i < LINHAS; i++){
		int valor = wait(&status);

		if(WEXITSTATUS(status) != 0)
			printf("Encontrei o %d na linha %d\n", num, WEXITSTATUS(status));
	}

	printf("\nTerminei\n");


	return 0;
}