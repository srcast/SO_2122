#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int seconds = 0;
int counter = 0;
int r = 1;


void inc_seconds(int signum){
	seconds++;
}

void print_seconds(int signum){
	printf("seconds: %d\n", seconds);
	counter++;
}

void exit(int signum){
	printf("counter = %d\n", counter);
	r = 0;
	//kill(getpid(), signum);
}

int main(int argc, char const *argv[]){
	
	if(signal(SIGALRM, inc_seconds) == SIG_ERR){
		printf("erro\n");
		return 1;
	}


	if(signal(SIGINT, print_seconds) == SIG_ERR){
		printf("erro\n");
		return 1;
	}

	if(signal(SIGQUIT, exit) == SIG_ERR){
		printf("erro\n");
		return 1;
	}

	while(r){
		alarm(1);
		pause(); //para até receber um sinal --> quando dá pause é que vai ao inc_seconds
		// senão tivesse o pause, nunca iria incrementar os seconds pois o alrm iria estar sempre a reiniciar
		//printf("seconds: %d\n", seconds);
	}



	return 0;
}