#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{

	//enquanto não tiver criado X processos
	// ----- cria processo
	// ----- espera por processo


	int i = 0, pid, status;


	while(i < 10){

		i++;

		if((pid = fork()) == 0){
			// código do filho
			printf("Filho %d -> pid: %d\n", i, getpid());

			printf("Filho %d -> ppid: %d\n", i, getppid());

			_exit(i);

		}

		else {
			// código do pai
			pid_t child = wait(&status);

			printf("Codigo de saida do %d: %d ------> %d\n", i, WEXITSTATUS(status), child);
		}



	}
	



	return 0;
}