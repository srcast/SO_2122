#include <sys/types.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <stdio.h> //para utilizar o perror
#include <string.h>

// 1. abrir descritor de leitura

// 2. passar o buffer e o descritor à função

// 3. quando a função encontrar um \n devolver quantos caracteres leu

// 4. escrever no ecra a linha 

ssize_t readln(int fd, char *line, size_t size);
ssize_t readlnProf(int fd, char *line, size_t size)


int main(int argc, char const *argv[]){

	int fd = open(argv[1], O_RDONLY);
	char line[1024];

	int pos = 0, count = 1;


	int res = readln(fd, line, 1024);

	if (res == 0) printf("Vazio\n");

	else {

		pos = res + 1;;

		while( res > 0){

			char numLinha[100];
			lseek(fd, pos, SEEK_SET);

			sprintf(numLinha, "      %d  ", count);
			//sprintf(numLinha, "%*d  ", 6, count); //esta é a correta
			write(1, numLinha, strlen(numLinha));
			write(1, line, res);
			printf("\n");

			res = readln(fd, line, 1024);  //invocar o readln com line + res para começar a ler na posição em que devolveu anteriormente
			count++;
			pos += res + 1;;
		}

	}
		
	close(fd);

	return 0;
}


ssize_t readln(int fd, char *line, size_t size){

	int bytes_read;
	int i;
	int total = 0;


	while((bytes_read = read(fd, line, size)) > 0) { 
		i = 0;
		while(i < bytes_read){
			if(line[i] != '\n') i++;
			else break;

		}
		total += i;
	}


	return total;
}




//------------------------------------------------------------ Propostas do professor --------------------------------------------------------------------------

// EXERCICIO 3

ssize_t readlnProf(int fd, char *line, size_t size){

	int i = 0;

	while(i < size && read(fd, line + i, 1) > 0){ //para não estar a escrever sempre na posição 0 do line ----> vamos preenchendo o line com o passar do tempo

		// verifica conteudo da posiçao line[i]

		if (line[i] == '\n') return i + 1; //retorna a posição seguinte à mudança de linha

		i++;

	}

	return i;
}


#define size 1024

char aux[size];
int buffer_pos = 0;
int buffer_end = 0;

// EXERCICIO 5

ssize_t readlnOtimizado(int fd, char *line, size_t size){

	// usar um array auxiliar global, preenchelo com tamanho size diretamente do read() e ir copiando para o line[] e retornar este

	int bytes_read;

	if (buffer_pos == buffer_end){

		bytes_read = read(fd, aux, size);
		buffer_end = size - 1;

		
	}





	// se pos == end, lê do ficheiro para o buffer global (atualiza end = numero de bytes lidos)

	// *c = buffer[pos]

	//return 1;


}
