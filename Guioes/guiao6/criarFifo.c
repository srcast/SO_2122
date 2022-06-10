#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[]){

	if(mkfifo(argv[1], 0644)){
		perror("mkfifo");
	}


	
	return 0;
}