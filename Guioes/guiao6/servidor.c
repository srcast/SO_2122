#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[]){

	if(mkfifo("servidor_fifo", 0644) < 0)
		perror("servidor_fifo");

	int log_fd = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	int servidor = open("servidor_fifo", O_RDONLY);
	int escreve = open("servidor_fifo", O_WRONLY);
	

		

		int read_bytes = 0;

		char buf[128];

		while((read_bytes = read(servidor, buf, 128)) > 0){ // ocliente devolvendo EOF vai terminar
			write(log_fd, buf, read_bytes);
		}

		


	close(servidor);
	unlink("servidor_fifo");

	close(log_fd);
	
	return 0;
}