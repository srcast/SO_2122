#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


// ls -a ---> para ver os processos a correr

// lsof -p [numero processo] ----> para ver descritores do processo

int main(int argc, char const *argv[]){

	int pid;
	int pipe_fd[2];

	pipe(pipe_fd);


	if(fork() == 0){

		close(pipe_fd[1]); // fecha o descritor de escreita do pipe ---> nao se pode esquecer

		dup2(pipe_fd[0], 0);
		close(pipe_fd[0]);

		execlp("wc", "wc", NULL);

		_exit(0);
	}

	close(pipe_fd[0]);

	char buffer[1024];

	int bytes_read = read(0, buffer, 1024);

	while(bytes_read > 0){ // ctrl + D para terminar
		write(pipe_fd[1], buffer, bytes_read);
		bytes_read = read(0, buffer, 1024);

	}

	close(pipe_fd[1]);

	wait(NULL);	


	return 0;
}