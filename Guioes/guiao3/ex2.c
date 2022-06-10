#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char const *argv[]){

	int pid;

	if((pid = fork()) == 0){
		if(execl("/bin/ls", "/bin/ls", "-l", NULL) < 0) // se der erro no exec, os dois processos vao dar done, precisamos do _exit
			perror("exec\n");

		_exit(0);
	} 
	else{
		wait(NULL);
	}

	printf("DONE\n");
	
	return 0;
}