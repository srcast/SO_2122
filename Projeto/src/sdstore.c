// CLIENTE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define SERVIDOR_FIFO "../tmp/servidor_fifo"
#define BUFFERSIZE 1024


void writeAux(char *mensagem){
    write(1, mensagem, strlen(mensagem));
}

int main(int argc, char const *argv[]){


    if(argc == 1){
        writeAux("./sdstore status\n./sdstore proc-file priority input-filename output-filename transformation-id-1 transformation-id-2 ...\n");
    }

    else{

        int pid = getpid();
        int servidor_fifo;
        int cliente_fifo;
        char *pidString, *clienteFifoNome;

        char auxClienteFifoNome[BUFFERSIZE] = "../tmp/cliente_";
        char pidStringAux[BUFFERSIZE];

        sprintf(pidStringAux, "%d", pid);
        pidString = strdup(pidStringAux);
        clienteFifoNome = strdup(auxClienteFifoNome);

        strcat(clienteFifoNome, pidString); //clienteFifoNome = "../tmp/cliente_PID"

        if(mkfifo(clienteFifoNome, 0644))
            perror("Erro a criar o cliente fifo");



        
        if(strcmp(argv[1], "status") == 0){
            // status
            

            char *status = "status";

            int pid = getpid();
            char *men = "Pid: ";

            if((servidor_fifo = open(SERVIDOR_FIFO, O_WRONLY)) < 0)
                perror("Erro ao abrir o servidor_fifo");

            
            char *buffer[BUFFERSIZE];

            sprintf(buffer, "%s %d", status, pid);

            char *aux = strdup(buffer);


            write(servidor_fifo, aux, strlen(aux)); // escreve no servidor fifo "status PID"


            int bytes_read;
            char buf[BUFFERSIZE];

            if((cliente_fifo = open(clienteFifoNome, O_RDONLY)) < 0)
                perror("Erro a abrir o cliente fifo");

            while((bytes_read = read(cliente_fifo, buf, BUFFERSIZE)) > 0)
                write(1, buf, bytes_read); // escreve no ecra o que lẽ do cliente fifo


            
        }

        else if(strcmp(argv[1], "proc-file") == 0){
            // transformacoes


            char buffer[BUFFERSIZE];
            char trans[BUFFERSIZE];

            for(int l = 0; l < BUFFERSIZE; l++){
                trans[l] = NULL;
            }

            if((servidor_fifo = open(SERVIDOR_FIFO, O_WRONLY)) < 0)
                perror("Erro a abrir o servidor_fifo");

            int numTransformacoes;

            // ./sdstore proc-file <priority> samples/file-a outputs/file-a-output bcompress nop gcompress encrypt nop ...

            if(strcmp(argv[2], "-p") == 0){
                numTransformacoes = argc - 6;

                for(int i = 6; i < argc; i++){
                    strcat(trans, argv[i]);
                    strcat(trans, " ");
                }

                sprintf(buffer, "proc-file %d %d %s %s %s ", pid, numTransformacoes, argv[3], argv[4], argv[5]);
            }
            else{
            
                numTransformacoes = argc - 5;

                for(int i = 5; i < argc; i++){
                    strcat(trans, argv[i]);
                    strcat(trans, " ");
                }

                sprintf(buffer, "proc-file %d %d %s %s %s ", pid, numTransformacoes, "0", argv[3], argv[4]);
            }

            char *aux;
            char *aux2;

            aux = strdup(buffer);

            aux2 = strdup(trans);

            strcat(aux, aux2);

            write(servidor_fifo, aux, strlen(aux));


            int bytes_read;
            char buf[BUFFERSIZE];

           if((cliente_fifo = open(clienteFifoNome, O_RDONLY)) < 0)
                perror("Erro a abrir o cliente fifo");

            while((bytes_read = read(cliente_fifo, buf, BUFFERSIZE)) > 0)
               write(1, buf, bytes_read); // escreve no ecra o que lẽ do cliente fifo

        }

        else {
            writeAux("Erro na execucao do sdstore\n");
        }

        
        close(servidor_fifo);
        close(cliente_fifo);
        unlink(clienteFifoNome);
    	
    }




    return 0;
}
