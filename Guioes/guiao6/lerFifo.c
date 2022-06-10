#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[]){

	int fifo_fd = open("fifo", O_RDONLY);

	char buf[10];

	int read_bytes = 0;

	while((read_bytes = read(fifo_fd, buf, 10)) > 0){
		write(1, buf, read_bytes);
	}


	close(fifo_fd);
	
	return 0;
}