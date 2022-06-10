#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
	
	printf("pid: %d\n", getpid());

	printf("ppid: %d\n", getppid());



	return 0;
}