#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char const *argv[]){


	int pipe_fd[2];

	pipe(pipe_fd);


	// ls / etc

	if (fork() == 0){
		close(pipe_fd[0]); // só precisa da escrita
		dup2(pipe_fd[1], 1); //a escrita vai passar a ser no pipe em vez do teclado
		close(pipe_fd[1]);

		// stdin = keyboard
		// stdout = pipe_fd[1]
		// stderr = screan

		execlp("ls", "ls", "/etc/", NULL);

		_exit(1);
	}

	close(pipe_fd[1]); // só precisa da leitura o wc logo fecha se antes de criar o processo

	// wc -l
	if(fork() == 0){
		dup2(pipe_fd[0], 0); //a leitura vai passar a ser no pipe em vez do ecra
		close(pipe_fd[0]);

		// stdin = pipe_fd[0]
		// stdout = ecra
		// stderr = screan

		execlp("wc", "wc", "-l", NULL);

		_exit(1);
	}

	close(pipe_fd[0]);

	// esperar que o main espere pelos 2 processos
	wait(NULL);
	wait(NULL);
	
	return 0;
}