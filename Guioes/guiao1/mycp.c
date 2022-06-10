#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <stdio.h> //para utilizar o perror


//argv[0] é o ńome do programa ----> não usar


int main(int argc, char const *argv[]){

	//1. abrir ficheiros
	int source_fd = open(argv[1], O_RDONLY);

	if(source_fd < 0) {

		perror("erro");

		return -1;
	}


	int target_fd = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0640);


	if(target_fd < 0) {

		perror("erro");

		return -1;
	}


	//2. criar um buffer de tamanho X (X = 1)
	char buffer[1024];


	//3. ler/escrever até read = 0
	int bytes_read = 0;

	if((bytes_read = read(source_fd, buffer, 1024)) > 0) {

		write(target_fd, buffer, bytes_read); //não escrevemos 1024 porque se tiver lido menos bytes, ia passar lixo. Assim apenas escreve o que lê
	}


	//4. fechar descritores
	close(target_fd);
	close(source_fd);	


	return 0;
}