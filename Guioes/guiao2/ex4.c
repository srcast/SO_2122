#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{

	int i = 0, pid, status;

	// criar um ciclo para forks e um ciclo para waits

	while(i < 10){

		i++;

		if((pid = fork()) == 0){
			// código do filho
			printf("Filho %d -> pid: %d\n", i, getpid());

			printf("Filho %d -> ppid: %d\n", i, getppid());

			_exit(i);
		}
	}

	for (int j = 1; j <= 10; j++){
		// código do pai
			pid_t child = wait(&status);

			printf("Codigo de saida do %d: %d\n", j, WEXITSTATUS(status));
	}
	



	return 0;
}