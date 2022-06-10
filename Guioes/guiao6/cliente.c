#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[]){

	int fifo_fd = open("servidor_fifo", O_WRONLY);

	char buf[128];

	int read_bytes = 0;

	printf("fifo is open\n"); // só aparece quando o de leitura é aberto


	while((read_bytes = read(0, buf, 128)) > 0){
		write(fifo_fd, buf, read_bytes);
	}

	close(fifo_fd);
	
	return 0;
}