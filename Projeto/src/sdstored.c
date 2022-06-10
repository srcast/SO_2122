// SERVIDOR
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "estruturas.h"

#define SERVIDOR_FIFO "../tmp/servidor_fifo"
#define BUFFERSIZE 1024
#define NTOKEN 2
#define TOKENPEDIDOS 7
#define PIDSPEDIDOS 128
#define PEDIDOSMAX 5


int readln (int fd, char* linha, int tamBuffer);
int parse_config (const char *filepath);

// VARIÁVEIS GLOBÁIS

l listaTransf; // tem que ser global para termos sempre acesso
lp pedidos;

p pedidoExecutar;

int pidPedidoAtual; //último pid do pedido a ser lido no servidor para executar
int pids[PIDSPEDIDOS];
int posPidsPedidos = 0;
int servidorAtivo = 1;
int serverPID;
int servidor;
int escrita;
int pedidoAtual = 0;
char camAux[100]; // = "../bin/sdstore-transformations/";

void writeAux(char *mensagem){
    write(1, mensagem, strlen(mensagem));
}



// ler linha a linha o ficheiro e devolve o numero de bytes de cada linha
int readln (int fd, char* linha, int tamBuffer){

    int bytes_read;
    int i;
    int total = 0;


    while((bytes_read = read(fd, linha, tamBuffer)) > 0) { 
        i = 0;
        while(i < bytes_read){
            if(linha[i] != '\n') i++;
            else break;

        }
        total += i;
    }

    return total;

}




char * readlnBuffer (int fd, int tamBuffer){

    int bytes_read;
    int i;
    int j = 1;
    int total = 0;
    char *aux = malloc(j * tamBuffer * sizeof(char));
    int tamTotal = 0;


    while((bytes_read = read(fd, aux, j * tamBuffer)) > 0) {
        char *aux = (char*) realloc(aux, i * tamBuffer * sizeof(char)); 
        i = 0;
        while(i < bytes_read){
            if(aux[i] != '\n') i++;
            else break;

        }
        total += i;
        tamTotal += i; //controla o numero de posiçoes do aux que tem elementos
        j++; //controla quantas vezes ocorreu o while
    }

    for(int k = tamTotal + 1; k < j * tamBuffer; k++){
        aux[k] = '\0';
    }

    return aux;

}


int parse_config (const char *filepath){

    int fd_config;
    
    if((fd_config = open(filepath, O_RDONLY, 644)) < 0)
        perror("Erro na abertura do config");
    char linha[BUFFERSIZE];

    int res = readln(fd_config, linha, BUFFERSIZE);

    int pos = 0, i= 1;
    char aux[100];
    char *transf;
    int nocor;

    
    if (res == 0)
        write(1, "Ficheiro de configuracao vazio\n", 32);

    else {

        pos = res + 1;

        while( res > 0){

            lseek(fd_config, pos, SEEK_SET);

            memcpy(aux, linha, res);       

            t transformacaoTemp;
            char *token[NTOKEN];

            for (int i = 0; i < NTOKEN; i++) {
                token[i] = NULL;
            }
            

            transf = strtok(aux, " ");

            for (int t = 0; t < NTOKEN; t++) {
                token[t] = strdup(transf);

                transf = strtok(NULL, " ");
            } 
            
           nocor = atoi(token[1]);
           transformacaoTemp = createTransf(token[0], nocor);
           listaTransf = insereTransf(transformacaoTemp, listaTransf);
    
            for(i = 0; i < 100; i++){
                aux[i] = '\0';
            }

            res = readln(fd_config, linha, BUFFERSIZE);  //invocar o readln com line + res para começar a ler na posição em que devolveu anteriormente

            pos += res + 1;
            i++;
        }

    }
        
    close(fd_config);

    return 0;
}




int getIdProximaTask(){

    int idTask = 0;

    if(pedidos == NULL) //se a lista de pedidos for nula
        ;
    
    else {
        for(lp pedidosAux = pedidos; pedidosAux != NULL; pedidosAux = pedidosAux->prox){
            if(pedidosAux->p->idCliente > idTask)
                idTask = pedidosAux->p->idCliente;
        }
    }

    return idTask + 1;
}

void executarTransformacoes(){ 

    int nPipes = pedidoExecutar->nTransf - 1;

    int pipe_fd[nPipes][2];
    int i = 0;


    char *tokenTransf[pedidoExecutar->nTransf]; // numero de transformcoes do pedido a executar
    char *transf;
    char *auxMutacoes = strdup(pedidoExecutar->mutacoes);

    for (int i = 0; i < pedidoExecutar->nTransf; i++) {
        tokenTransf[i] = NULL;
    }
            

    transf = strtok(auxMutacoes, " ");

    for (int t = 0; t < pedidoExecutar->nTransf; t++) {
        tokenTransf[t] = strdup(transf);
        transf = strtok(NULL, " ");
    } 

    while(i < pedidoExecutar->nTransf){


        if(i == 0){ //primeira transformção ---> lê da source

            if (pipe(pipe_fd[i]) < 0)
                perror("pipe");

            if(fork() == 0){
                int leitura;

                if((leitura = open(pedidoExecutar->source, O_RDONLY)) < 0)
                    perror("leitura");

                

                dup2(leitura, 0);
                close(leitura);
                close(pipe_fd[i][0]);
                dup2(pipe_fd[i][1], 1);
                close(pipe_fd[i][1]);

    
                strcat(camAux, tokenTransf[i]);
                char *caminho = strdup(camAux);


                if(execl(caminho, caminho, NULL) < 0)
                    perror(caminho);

            }

            close(pipe_fd[i][1]);

        }
        else if(i == pedidoExecutar->nTransf - 1){

            if(fork() == 0){
                int escrita;
                if((escrita = open(pedidoExecutar->destination, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
                    perror("escrita");
                dup2(escrita, 1);
                close(escrita);

                dup2(pipe_fd[i-1][0], 0);
                close(pipe_fd[i-1][0]);

                strcat(camAux, tokenTransf[i]);
                char *caminho = strdup(camAux);

                if(execl(caminho, caminho, NULL) < 0)
                    perror(caminho);


            }

            close(pipe_fd[i-1][0]);


        }

        else {

            if(pipe(pipe_fd[i]) < 0)
                perror("pipe");

            if(fork() == 0){
                close(pipe_fd[i][0]);
                dup2(pipe_fd[i-1][0], 0);
                close(pipe_fd[i-1][0]);
                dup2(pipe_fd[i][1], 1);
                close(pipe_fd[i][1]);

                strcat(camAux, tokenTransf[i]);
                char *caminho = strdup(camAux);

                if(execl(caminho, caminho, NULL) < 0)
                    perror(caminho);


            }

            close(pipe_fd[i-1][0]);
            close(pipe_fd[i][1]);
        }




        i++;


        }



}


int executarPedidos(){

    int pid;

    posPidsPedidos++; //atualiza o numero de pedidos executados

    int pidPedidoCompleto, status;

    // for(lp auxPedidos = pedidos; auxPedidos != NULL; auxPedidos = auxPedidos->prox){
    //     if(auxPedidos->p->pid == pidPedidoAtual){
    //         pedidoExecutar = clonePedido(auxPedidos->p);

    //     }
    // }



        char *tokenTransf[pedidoExecutar->nTransf]; // numero de transformcoes do pedido a executar
        char *transf;
        char *auxMutacoes = strdup(pedidoExecutar->mutacoes);

        for (int i = 0; i < pedidoExecutar->nTransf; i++) {
            tokenTransf[i] = NULL;
        }
            

        transf = strtok(auxMutacoes, " ");

        for (int t = 0; t < pedidoExecutar->nTransf; t++) {
            tokenTransf[t] = strdup(transf);

            transf = strtok(NULL, " ");
        } 



        int algumOcupado = 1;

        while (algumOcupado){
            algumOcupado = 0;

            for(l auxTrans = listaTransf; auxTrans != NULL; auxTrans = auxTrans->prox){
                for(int j = 0; j < pedidoExecutar->nTransf; j++){
                    if((strcmp(auxTrans->t->transf, tokenTransf[j])) == 0){ // se forem iguais
                        if (auxTrans->t->ocorr == auxTrans->t->ocorrmax) // não tem espaço para correr mais nenhuma dessas transformações
                            algumOcupado = 1;
                    } 
                }
            }
        }

        char auxClienteFifoNome[BUFFERSIZE] = "../tmp/cliente_";
        char pidStringAux[BUFFERSIZE];
        char *clienteFifoNome, *pidString;

        pidPedidoAtual = pedidoExecutar->pid;

        sprintf(pidStringAux, "%d", pidPedidoAtual);
        pidString = strdup(pidStringAux);
        clienteFifoNome = strdup(auxClienteFifoNome);

        strcat(clienteFifoNome, pidString); //clienteFifoNome = "../tmp/cliente_PID"

        int cliente;

        if((cliente = open(clienteFifoNome, O_WRONLY)) < 0){
            perror("Erro a abrir cliente fifo pelo servidor");
        }

        write(cliente, "Processing\n", 12);

        close(cliente);



        // se puder correr as transformações, atualiza as
        for(l auxTrans = listaTransf; auxTrans != NULL; auxTrans = auxTrans->prox){
            for(int j = 0; j < pedidoExecutar->nTransf; j++){
                if((strcmp(auxTrans->t->transf, tokenTransf[j])) == 0){ // se forem iguais
                    auxTrans->t->ocorr++;
                } 
            }
        }

        //sleep(8);

        if((pid = fork()) == 0){
            executarTransformacoes();
            _exit(pedidoExecutar->idCliente);
        }

    



    pid_t child = wait(&status);


    return WEXITSTATUS(status);

}


int getBytes(char *ficheiro){

    int pipe_fd[2][2];

    if(pipe(pipe_fd[0]) < 0)
        perror("Erro a abrir o pipe 0");

    if(fork() == 0){
        close(pipe_fd[0][0]);
        dup2(pipe_fd[0][1], 1);
        close(pipe_fd[0][1]);


        if((execlp("cat", "cat", ficheiro, NULL)) < 0)
            perror("Erro no exec");

    

    }

    close(pipe_fd[0][1]);

    if(pipe(pipe_fd[1]) < 0)
        perror("Erro a abrir o pipe 1");

    if(fork() == 0){
        close(pipe_fd[1][0]);
        dup2(pipe_fd[1][1], 1);
        close(pipe_fd[1][1]);
        dup2(pipe_fd[0][0], 0);
        close(pipe_fd[0][0]);


        if((execlp("wc", "wc", "-c", NULL)) < 0)
            perror("Erro no exec");

    

    }

    close(pipe_fd[0]);

    dup2(pipe_fd[1][0], 0);
    close(pipe_fd[1]);

    char tam[20];
    int bytes_read;

    if((bytes_read = read(0, tam, 20)) < 0)
        perror("Erro a ler do ultimo pipe");

    char *aux = strdup(tam);

    int bytes = atoi(aux);

    // int fd = open(ficheiro, O_RDONLY);
    // int bytes = lseek(fd, 0, SEEK_END);

    return bytes;
    
}


void atualizaTransf(char *transformacoes, int nTransf){
    
    char *tokenTransf[nTransf]; 
    char *transf;
    char *auxTransformacoes = strdup(transformacoes);

    for (int i = 0; i < nTransf; i++) {
        tokenTransf[i] = NULL;
    }
            

    transf = strtok(auxTransformacoes, " ");

    for (int t = 0; t < nTransf; t++) {
        tokenTransf[t] = strdup(transf);
        transf = strtok(NULL, " ");
    }

    for(int k = 0; k < nTransf; k++){
        for(l auxListaT = listaTransf; auxListaT != NULL; auxListaT = auxListaT->prox){
            if((strcmp(tokenTransf[k], auxListaT->t->transf)) == 0)
                auxListaT->t->ocorr--;
        }
    }
}


void parse_pedidos(){

    int bytes;
    char buf[BUFFERSIZE];


    while(servidorAtivo == 1){

        servidor = open(SERVIDOR_FIFO, O_RDONLY);
        escrita = open(SERVIDOR_FIFO, O_WRONLY);


        while((bytes = read(servidor, buf, BUFFERSIZE)) > 0){
            pedidoAtual++;


            char *token2[TOKENPEDIDOS];
            char *ped;
            char auxClienteFifoNome[BUFFERSIZE] = "../tmp/cliente_";
            char pidStringAux[BUFFERSIZE];
            char *clienteFifoNome, *pidString;


            if(buf[0] == 's'){
                // exec status

                char *camposStatus[2];
                char *buffzinho = strdup(buf);
                int t;


                for (int p = 0; p < 2; p++) {
                    camposStatus[p] = NULL;
                }
                        
                ped = strtok(buffzinho, " ");

                int k;

                for (t = 0; t < 2; t++) {
                        camposStatus[t] = strdup(ped);
                        ped = strtok(NULL, " ");
                }

                // char men1[200];
                // sprintf(men1, "Recebido o pedido do processo %s\n", camposStatus[1]);
                // char *men1Aux = strdup(men1);
                // write(1, men1Aux, strlen(men1Aux));


                pidPedidoAtual = atoi(camposStatus[1]);

                sprintf(pidStringAux, "%d", pidPedidoAtual);
                pidString = strdup(pidStringAux);
                clienteFifoNome = strdup(auxClienteFifoNome);

                strcat(clienteFifoNome, pidString); //clienteFifoNome = "../tmp/cliente_PID"
                int cliente;

                if((cliente = open(clienteFifoNome, O_WRONLY)) < 0){
                    perror("Erro a abrir cliente fifo pelo servidor");
                }

                

                char mensagemTransf[BUFFERSIZE];


                for(lp pedidosAux = pedidos; pedidosAux != NULL; pedidosAux = pedidosAux->prox){

                    for(t = 0; t < BUFFERSIZE; t++){
                        mensagemTransf[t] = '\0';
                    }


                    sprintf(mensagemTransf, "task #%d: proc-file %d %s %s %s\n", pedidosAux->p->idCliente, pedidosAux->p->prioridade, pedidosAux->p->source, pedidosAux->p->destination, pedidosAux->p->mutacoes);
                    char *aux = strdup(mensagemTransf);

                    //write(1, aux, strlen(aux));
 
                    write(cliente, aux, strlen(aux));
                }







    
                for(l listaAux = listaTransf; listaAux != NULL; listaAux = listaAux->prox){

                    for(t = 0; t < BUFFERSIZE; t++){
                        mensagemTransf[t] = '\0';
                    }

                    sprintf(mensagemTransf, "transf %s: %d/%d (running/max)\n", listaAux->t->transf, listaAux->t->ocorr, listaAux->t->ocorrmax);
                    char *aux = strdup(mensagemTransf);
                    //write(1, aux, strlen(aux));
                    write(cliente, aux, strlen(aux));
                } 


                close(cliente);

                char message[50];
                sprintf(message, "Pedido referente ao PID %s concluído com sucesso\n", camposStatus[1]);
                write(1, message, strlen(message));

                if(pedidoAtual == PEDIDOSMAX) // se chegarmos ao limite máximo ----> termina o servidor
                    kill(serverPID, SIGTERM);


            }
            else { // exec proc

                //token[0] = proc-file ou status
                //token[1] = pidPedido
                //token[2] = numero transformações
                //token[3] = prioridade
                //token[4] = source
                //token[5] = destino
                //token[6] = string de transformcaçoes

                for (int i = 0; i < TOKENPEDIDOS; i++) {
                    token2[i] = NULL;
                }
                        
                ped = strtok(buf, " ");


                for (int t = 0; t < TOKENPEDIDOS; t++) {
                    if(t==6){
                        int token=atoi(token2[2]);
                        int aux=t;
                        while(token>0){
                            token2[aux] = strdup(ped);
                            ped = strtok(NULL, " ");
                            token--;
                            aux++;
                        }
                    }
                    else {
                        token2[t] = strdup(ped);
                        ped = strtok(NULL, " ");
                    }
                }

                int t=6;

                int nTransf = atoi(token2[2]);

                for(int i=0; i<nTransf; i++){
                    strcat(token2[t], " ");
                    t++;
                }

                t=6;
                char cat[1000];
                strcpy(cat,token2[6]);

                for(int t=7; t<6+nTransf; t++){
                    strcat(cat,token2[t]);
                }

                pidPedidoAtual = atoi(token2[1]); //pid atribuido às tarefas

                // determinar qual o id da tarefa a executar
                int idCliente; //id Task
                idCliente = getIdProximaTask();

                pidPedidoAtual = atoi(token2[1]);
                
                p pedidoTemp;
                
                pedidoTemp = createPedido(nTransf, idCliente, pidPedidoAtual, atoi(token2[3]), token2[4], token2[5], cat);
                
                pedidos = insereTransfPedidosPrioridade(pedidoTemp, pedidos); //atualiza os pedidos


                // pedido a tratar
                p pedidoTratar;

                pedidoTratar = getPedidoCabeca(pedidos);

                pedidos = retiraListaCabeca(pedidos); //retira o primeiro pedido

                pedidos = insereTransfPedidosPrioridade(pedidoTratar, pedidos); // inserir pedido com prioridade = -1 

                pedidoExecutar = clonePedido(pedidoTratar); //define o pedido da variável global como o pedidoTratar          


                // realizar os execs com os forks 

                sprintf(pidStringAux, "%d", pidPedidoAtual);
                pidString = strdup(pidStringAux);
                clienteFifoNome = strdup(auxClienteFifoNome);

                strcat(clienteFifoNome, pidString); //clienteFifoNome = "../tmp/cliente_PID"

                int cliente;

                if((cliente = open(clienteFifoNome, O_WRONLY)) < 0){
                     perror("Erro a abrir cliente fifo pelo servidor");
                 }

                //char mensagemTransf[BUFFERSIZE];

                //sprintf(mensagemTransf, "Recebemos o seu pedido com o PID = %d\n", pidPedidoAtual);

                //pending
                write(cliente, "Pending\n", 9);

                int pidEliminar = executarPedidos(); //é o idCliente

               p pedidoRetirar;
               pedidoRetirar = devolvePedido(pedidos, pidEliminar);


                // concluded
                // tamanho do ficheiro original e do ficheiro transformado
                char *ficheiroOriginal = strdup(pedidoRetirar->source);
                char *ficheiroFinal = strdup(pedidoRetirar->destination);

                int tamanhoOriginal;
                int tamanhoFinal;
                char *mensagem = malloc(1024 * sizeof(char*));

                tamanhoOriginal = getBytes(ficheiroOriginal);
                tamanhoFinal = getBytes(ficheiroFinal);


                sprintf(mensagem, "Concluded (bytes-input: %d, bytes-output: %d)\n", tamanhoOriginal, tamanhoFinal);

                char *men = strdup(mensagem);

                write(cliente, men, strlen(men));

                char message[50];
                sprintf(message, "Pedido referente ao PID %d concluído com sucesso\n", pedidoRetirar->pid);
                write(1, message, strlen(message));

                
                
                //atualizar as transformacoes
                atualizaTransf(pedidoRetirar->mutacoes, pedidoRetirar->nTransf);

                //retirar o pedido
                //pedidos = retiraPedido(pedidos, pidEliminar); //retira o pedido tratado da lista

                close(cliente);

                if(pedidoAtual == PEDIDOSMAX) // se chegarmos ao limite máximo ----> termina o servidor
                    kill(serverPID, SIGTERM);


            }

                for(int l = 0; l < BUFFERSIZE; l++){
                    buf[l] = '\0';
                }

    }


    
}


// void terminaPedido(int pidChild){

//     printf("Recebi o %d\n", pidChild);

//     int posicao, i;

//     posicao = getPosicao(pidChild);


//     int id = pidsAssociaID[posicao]; //o id do cliente processado está na posição 
//     printf("ID: %d\n", id);

//     for(lp auxLista = pedidos; auxLista != NULL; auxLista = auxLista->prox){
//         printf("ID PEDIDOS: %d\n", auxLista->p->idCliente);
//         }
    


//         //printf("idcliente %d\n", id);

//                 p pedidoRetirar;
//                pedidoRetirar = devolvePedido(pedidos, id);

//                printf("Cheguei\n");
               
//                printf("%s\n", pedidoRetirar->source);
//                printf("%s\n", pedidoRetirar->destination);

//                 // concluded
//                 // tamanho do ficheiro original e do ficheiro transformado
//                 char *ficheiroOriginal = strdup(pedidoRetirar->source);
//                 char *ficheiroFinal = strdup(pedidoRetirar->destination);

//                 int tamanhoOriginal;
//                 int tamanhoFinal;
//                 char *mensagem = malloc(1024 * sizeof(char*));

//                 printf("Cheguei\n");

//                 tamanhoOriginal = getBytes(ficheiroOriginal);
//                 tamanhoFinal = getBytes(ficheiroFinal);

//                 printf("original :%d\n", tamanhoOriginal);
//                 printf("final: %d\n", tamanhoFinal);

//                 char auxClienteFifoNome[BUFFERSIZE] = "../tmp/cliente_";
//                 char pidStringAux[BUFFERSIZE];
//                 char *clienteFifoNome, *pidString;


//                 sprintf(pidStringAux, "%d", pedidoRetirar->pid);
//                 pidString = strdup(pidStringAux);
//                 clienteFifoNome = strdup(auxClienteFifoNome);

//                 strcat(clienteFifoNome, pidString); //clienteFifoNome = "../tmp/cliente_PID"


//                 // int cliente;

//                 // if((cliente = open(clienteFifoNome, O_WRONLY)) < 0){
//                 //      perror("Erro a abrir cliente fifo pelo servidor");
//                 //  }


//                 sprintf(mensagem, "Concluded (bytes-input: %d, bytes-output: %d)\n", tamanhoOriginal, tamanhoFinal);

//                 char *men = strdup(mensagem);
//                 int pos;

//                 for(int q = 0; q < 50; q++){
//                     if(ped[q] == pedidoRetirar->pid)
//                         pos = q;
//                 }

//                 int descritor = descPed[pos];

//                 write(descritor, men, strlen(men));

//                 printf("%s\n", men);
                
                




//                 atualizaTransf(pedidoRetirar->mutacoes, pedidoRetirar->nTransf);

//                 //retirar o pedido
//                 pedidos = retiraPedido(pedidos, id); //retira o pedido tratado da lista

//                 //printf("Pedido PID %d completo!!!!!!\n", pidEliminar);




//                 close(descritor);
//                 printf("\nTerminei\n");
// }



// void sigchld_handler(int signum) {
//     int status;
//     int id;
//     pid_t child = wait(&status);

//     if (WIFEXITED(status) && (WEXITSTATUS(status) == 20) && serverPID == getpid()){ //se o status não for 0 e estivermos no processo do servidor
//         for(int i = 0; i < MAXPEDIDOS; i++){
//             if(child == pids[i]){
//                 terminaPedido(child); //o status é o idCliente do pedido concluido
//                 printf("Child com o pid  --------------> %d / %d\n", child, pids[i]);
//                 pids[i] = 0; //para nao aparecer futuramente
//             }
//         }
//     }



// }



}

void sigterm_handler (int signum){

    writeAux("\nRecebido o sinal SIGTERM\n");

    servidorAtivo = 0;


    // wait para os pids todos dos fork()
    for(int i = 0; i < posPidsPedidos; i++)
        wait(NULL);
    

    close(servidor);
    close(escrita);
    unlink(SERVIDOR_FIFO);



}


int main(int argc, char const *argv[]){

    char inicio[BUFFERSIZE];
    char *inicioAux;

    serverPID = getpid();

    sprintf(inicio, "SERVIDOR INICIADO COM O PID %d\n\n", serverPID);
    inicioAux = strdup(inicio);
    writeAux(inicioAux);

    strcat(camAux, argv[2]); // = "../bin/sdstore-transformations/";

    int r = parse_config(argv[1]);

    write(1, "Transformacoes e a sua capacidade:\n", 36);
    for(l transfAux = listaTransf; transfAux != NULL; transfAux = transfAux->prox){
        char t[30];
        sprintf(t, "    %s: %d\n", transfAux->t->transf, transfAux->t->ocorrmax);
        char *a = strdup(t);
        write(1, t, strlen(t));
    }

    int numT = 0;
    
    if(signal(SIGTERM, sigterm_handler) == SIG_ERR){
        write(1, "Erro\n", 6);
        return 1;
    }


    if(mkfifo(SERVIDOR_FIFO, 0644))
        perror("Erro a criar o servidor fifo");


    parse_pedidos();

    
    write(1, "\nSERVIDOR ENCERRADO\n", 20);


    return 0;
}