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

	srand(2022);

	int matriz[LINHAS][COLUNAS];

	for(i = 0; i < LINHAS; i++){
		for (j = 0; j < COLUNAS; j++)
		{
			matriz[i][j] = rand() % 150;
			//valAtual++;
		}
	}

	num = rand() % 100;

	for (i = 0; i < LINHAS; i++){
		
		if ((pid = fork()) == 0){ 
			//vai procurar por linhas
			for (j = 0; j < COLUNAS; j++){
				if (matriz[i][j] == num){
					_exit(1);
				}
			}
			_exit(0);
		}
	}

	int ocorrencias = 0;

	for(i = 0; i < LINHAS; i++){
		int valor = wait(&status);

		if(WEXITSTATUS(status) == 1)
			ocorrencias++;
	}

	printf("O %d foi encontrado %d vezes.\n", num, ocorrencias);

	printf("\nTerminei\n");


	return 0;
}