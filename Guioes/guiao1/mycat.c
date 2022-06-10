#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <stdio.h> //para utilizar o perror


//argv[0] é o ńome do programa ----> não usar


int main(int argc, char const *argv[]){

	char buffer[1024];


	int bytes_read = 0;

	while((bytes_read = read(0, buffer, 1024)) > 0) { //0 indica que lê do teclado

		write(1, buffer, bytes_read); // 1 indica que escreve no ecrã
	}



	return 0;
}