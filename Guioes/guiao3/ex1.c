#include <unistd.h>
#include <stdio.h>

int main(int argc, char const *argv[]){


//	if(execl("/bin/ls", "/bin/ls", "-l", NULL) < 0) //caso dê erro
//		perror("exec");

//	if(execlp("ls", "ls", "-l", NULL) < 0) //caso dê erro // se tivessemos um ls no bin ele corria o nosso porque não estamos a especificar o caminho /bin/ls
//		perror("exec");

/*char *exec_args[] = {"/bin/ls", "-l", NULL};

	if(execv("/bin/ls", exec_args) < 0) //caso dê erro
		perror("exec");
*/

	sleep(10);

	execlp("sleep", "sleep", "10", NULL); // o nome do programa vai passar a ser sleep 10 porque não estamos a usar um fork para correr noutro processo, logo vai substituir ex1 por sleep 10

	printf("feito\n"); // o printf como está não vai correr devido ao programa principal passar a sleep 

	// ps ----> conseguimos ver os processos
	
	return 0;
}