#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char const *argv[]){


	int pipe_fd[3][2]; // 3 pipes

	pipe(pipe_fd[0]); // criação do pipe 0

	if(fork() == 0){
		close(pipe_fd[0][0]);
		dup2(pipe_fd[0][1], 1);
		close(pipe_fd[0][1]);

		execlp("grep", "grep", "-v", "^#", "/etc/passwd", NULL);

		_exit(1);
	}

	close(pipe_fd[0][1]); // não vamos escrever mais no pipe 0

	pipe(pipe_fd[1]); //criação do pipe 1

	if(fork() == 0){
		close(pipe_fd[1][0]); // apenas vamos escrever no pipe 1 o que lemos do 0

		dup2(pipe_fd[0][0], 0);
		close(pipe_fd[0][0]);

		dup2(pipe_fd[1][1]

		exec
	}



	
	return 0;
}