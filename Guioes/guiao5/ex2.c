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

		char buf[1024];

		int read_bytes;

		 while((read_bytes = read(pipe_fd[0], buf, 1024)) > 0){

			write(1, buf, read_bytes);
		 }

		 if(read_bytes == 0)
		 	write(1, "EOF\n", 4);

		_exit(0);
	}

	close(pipe_fd[0]);

	int acc = 0;

	/*while( acc < 5){
		write(pipe_fd[1], "data\n", 5);
		acc++;
	}
	*/

	char buffer[1024];

	int bytes_read = read(0, buffer, 1024);

	while(bytes_read > 0){ // ctrl + D para terminar
		write(pipe_fd[1], buffer, bytes_read);
		bytes_read = read(0, buffer, 1024);

	}

	close(pipe_fd[1]);

	wait(NULL); // se o wait estiver antes do close, vai haver deadlock, pois o filho ainda vai estar à espera de ler coisas
				// quando é terminado, sem ter indicação que já não há mais nada para ler




	return 0;
}