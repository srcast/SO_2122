#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
	
	char *trans[3] = {"gcompress", "nop", "bcompress"};
	int nt = 3;
	int npipes = nt - 1;
	int tranfAtual = 0;

	int pipe_fd[2][2];

	if (pipe(pipe_fd[0]) < 0)
		perror("pipe 0");

	if(fork() == 0){
		int leitura;

		if((leitura = open("../SDStore-transf/said.txt", O_RDONLY)) < 0)
			perror("leitura");

		

		dup2(leitura, 0);
		close(leitura);
		close(pipe_fd[0][0]);
		printf("aqui\n");
		dup2(pipe_fd[0][1], 1);
		close(pipe_fd[0][1]);


		if(execl("../SDStore-transf/gdecompress", "../SDStore-transf/gdecompress", NULL) < 0)
			perror("bdecompress");

		_exit(1);
	}

	close(pipe_fd[0][1]);

	if(pipe(pipe_fd[1]) < 0)
		perror("pipe 1");

	if(fork() == 0){
		close(pipe_fd[1][0]);
		dup2(pipe_fd[0][0], 0);
		close(pipe_fd[0][0]);
		dup2(pipe_fd[1][1], 1);
		close(pipe_fd[1][1]);

		if(execl("../SDStore-transf/nop", "../SDStore-transf/nop", NULL) < 0)
			perror("nop");

		_exit(1);
	}

	close(pipe_fd[0][0]);
	close(pipe_fd[1][1]);

	if(fork() == 0){
		int escrita;
		if((escrita = open("../SDStore-transf/resposta.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
			perror("escrita");
		dup2(escrita, 1);
		close(escrita);

		dup2(pipe_fd[1][0], 0);
		close(pipe_fd[1][0]);

		if(execl("../SDStore-transf/bdecompress", "../SDStore-transf/bdecompress", NULL) < 0)
			perror("gdecompress");

		_exit(1);
	}

	close(pipe_fd[1][0]);



	return 0;


	} 






