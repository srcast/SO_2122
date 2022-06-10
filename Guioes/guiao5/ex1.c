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

		close(pipe_fd[1]); // fecha o descritor de escreita do pipe

		char buf[10];

		int read_bytes = read(pipe_fd[0], buf, 10);

		write(1, buf, read_bytes);

		_exit(0);
	}

	close(pipe_fd[0]);

	write(pipe_fd[1], "data\n", 5);

	wait(NULL);




	return 0;
}