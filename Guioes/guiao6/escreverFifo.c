#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[]){

	int fifo_fd = open("fifo", O_WRONLY);

	char buf[10];

	int read_bytes = 0;

	printf("fifo is open\n"); // só aparece quando o de leitura é aberto


	while((read_bytes = read(0, buf, 10)) > 0){
		write(fifo_fd, buf, read_bytes);
	}

	// terminando a escrita com ctrl + D = EOF a leitura encerra sozinha e o fifo é eliminado com o unlink
	if(unlink("fifo") < 0)
		perror("unlink");

	
	return 0;
}