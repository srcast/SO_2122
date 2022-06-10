#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{


	int pid;


	if((pid = fork()) == 0){

		printf("Filho -> pid: %d\n", getpid());

		printf("Filho -> ppid: %d\n", getppid()); //o ppid pode não ser o mesmo do pai, este processo pode-se tornar orfao porque não existe wait no pai
	}
	else {
		printf("pid: %d\n", getpid());

		printf("ppid: %d\n", getppid());

		printf("child: %d\n", pid);
	}



	return 0;
}