#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



int main(int argc, char const *argv[]){

	char inputfile[20];
	char outputfile[20];

	int input_desc, output_descr, pid;

	strcpy(inputfile, argv[2]);
	strcpy(outputfile, argv[4]);

	input_desc = open(inputfile, O_RDONLY);
	output_descr = open(outputfile, O_CREAT | O_TRUNC | O_WRONLY, 0640);

	dup2(input_desc, 0);
	close(input_desc);

	dup2(output_descr, 1);
	close(output_descr);

	/*
	char *comandos[64];
	int i, pos = 0;
	comandos[pos] = strdup(argv[5]);
	pos++;



	for(i = 6; i < argc; i++){
		char aux[10];

		strcpy(aux, argv[i]);
		comandos[pos] = strdup(aux);
		pos++;
	}


	for(i = 0; i < argc - 6; i++){
		printf("%s\n", comandos[i]);
	}
	*/


    // o exec está a executar um endereço inválido
	if ((pid = fork()) == 0){
		/* code */
		if(execvp(argv[5], argv + 5) < 0)
			perror("exec");

		_exit(1);
	}


	
	return 0;
}
