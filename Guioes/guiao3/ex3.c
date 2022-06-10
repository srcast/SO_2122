#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char const *argv[]){

	int pid, i, j, status;

	sleep(10);

	for(i = 1; i < argc; i++){
		if( (pid = fork()) == 0){
			execlp(argv[i], argv[i], NULL);

			_exit(i);
		}
	}

	for (j = 1; i < argc; j++){

		wait(NULL);
		//pid_t child = wait(&status);

		//printf("Codigo de saida do %d: %d\n", j, WEXITSTATUS(status));
	}
	
	return 0;
}