#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char const *argv[]){
	

	int fd_input = open("/etc/passwd", O_RDONLY);
	int fd_erro = open("erros.txt", O_CREAT | O_TRUNC |O_WRONLY, 0640);
	int fd_saida = open("saida.txt", O_CREAT | O_TRUNC |O_WRONLY, 0640);

	int pid;

	// asseguramos uma copia do stdout para escrever terminei no terninal no final do programa
	int terminal = dup(1); 

	dup2(fd_input, 0);
	// como não precisamos mais dos 3 descritores iniciais, fecha-se
	close(fd_input);

	dup2(fd_saida, 1);
	close(fd_input);

	dup2(fd_erro, 2);
	close(fd_erro);

	if((pid = fork()) == 0){

		execlp("wc", "wc", NULL);
		
		_exit(1);
	}


	// voltamos a meter o stdout como o descritor e assim já podemos fechar o descritor terminal
	dup2(terminal, 1);
	close(terminal);

	write(1, "Terminei.\n", 10);

// LSOF SERVE PARA VER OS DESCRITORES

// se direcionar mos o STD_ERROR para um ficheiro, o perror vai escrever nesse ficheiro


	return 0;
}