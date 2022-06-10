#include "estruturas.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct Transf * createTransf(char *nomeTranf, int ocorrenciamax){
    t T = malloc(sizeof(Transformacao));
    T->transf = nomeTranf;
    T->ocorr = 0;
    T->ocorrmax = ocorrenciamax;

    return T;
}

struct Lista* createList(Transformacao a){
    l L = malloc(sizeof(Lista));
    L->t = a;
    L->prox=NULL;

    return L;
}



struct Lista* insereTransf(Transformacao transf, Lista l){
    if(l == NULL){
        l = malloc(sizeof(Lista));
        l->t = transf;
        l->prox=NULL;
    }
    
    else{
    Lista aux=l;
    while(aux->prox != NULL){
    aux=aux->prox;
    }
    aux->prox = malloc(sizeof(Lista));
    aux->prox->t = transf;
    aux->prox->prox=NULL;
    }

    return l;
}



char ** preencheArray (int N, char *transformacao) {

        char **array = (char**)malloc(N * sizeof(char**));

        for (int c = 0; c < N; c++) {
            array[c] = malloc(100 * sizeof(char *));
        }

        char *aux = strdup(transformacao);

        char *fileLineTokenized = strtok(aux, " ");
        char *token[N];


        for (int i = 0; i < N; i++) {
            token[i] = NULL;
        }

        for (int t = 0; t < N; t++) {
            token[t] = strdup(fileLineTokenized);
            fileLineTokenized = strtok(NULL, " ");
        }

        for(int a=0; a<N; a++){
            strcpy(array[a], token[a]);
        }


    return array;
    }

struct Pedidos * clonePedido(Pedidos aux){
    p P = malloc(sizeof(struct Pedidos));
    P->pid = aux->pid;
    P->prioridade = aux->prioridade;
    P->source = strdup(aux->source);
    P->destination = strdup(aux->destination);
    P->idCliente = aux->idCliente;
    P->nTransf = aux->nTransf;
    P->mutacoes = strdup(aux->mutacoes);

    return P;
}



struct Pedidos * createPedido(int numeroTransf, int idCliente, int pid, int prioridade, char *source, char *destination, char transformacoes[]){
    p P = malloc(sizeof(struct Pedidos));
    P->pid = pid;
    P->prioridade = prioridade;
    P->source = strdup(source);
    P->destination = strdup(destination);
    P->idCliente = idCliente;
    P->nTransf = numeroTransf;
    P->mutacoes = strdup(transformacoes);
    
    return P;
}

struct ListaPedidos* createListPedidos(Pedidos p){
    lp l = malloc(sizeof(struct ListaPedidos));
    l->p = p;
    l->prox = NULL;

    return l;
}

struct ListaPedidos* insereTransfPedidos(Pedidos pedido, ListaPedidos lpedidos){
    
    if(lpedidos==NULL){
        lpedidos=malloc(sizeof(struct ListaPedidos));
        lpedidos->p = pedido;
        lpedidos->prox = NULL;
        
    } else{
        
    ListaPedidos aux = lpedidos;
    
    while(aux->prox != NULL){
    aux = aux->prox;
    }
    
    aux->prox = malloc(sizeof(ListaPedidos));
    aux->prox->p = pedido;
    aux->prox->prox = NULL;
    
  }
 return lpedidos;
}
    
struct ListaPedidos* insereTransfPedidosPrioridade(Pedidos pedido, ListaPedidos lpedidos){

    if(lpedidos==NULL){
        lpedidos=malloc(sizeof(struct ListaPedidos));
        lpedidos->p = pedido;
        lpedidos->prox = NULL;

    } else{
        if(pedido->prioridade > lpedidos->p->prioridade){
        ListaPedidos aux = malloc(sizeof(struct ListaPedidos));
        aux->p = pedido;
        aux->prox = lpedidos;
        lpedidos = aux;
        return lpedidos;
        }

        ListaPedidos aux = lpedidos, aux1=lpedidos;
        while(aux != NULL && pedido->prioridade <= aux->p->prioridade){
           aux1=aux;
           aux=aux->prox;
        }

        aux1->prox = malloc(sizeof(ListaPedidos));
        aux1->prox->p = pedido;
        aux1->prox->prox=aux;

    }
    return lpedidos;
}



struct ListaPedidos* retiraPedido (ListaPedidos p, int id )
{
    ListaPedidos temp = p;
    ListaPedidos prev;
    if (temp->p->idCliente == id) {
        p = temp->prox;
        free(temp);
        return p;
    }
    while (temp != NULL && temp->p->idCliente != id) {
        prev = temp;
        temp = temp->prox;
    }
    if (temp == NULL) return p;
    prev->prox = temp->prox;
    free(temp);
    return p;
}


/*
struct Pedidos* retiraPedido (struct Pedidos* p, int pedido )
{

    struct Pedidos *temp = p, *prev;


    if (temp != NULL && temp->pid == pedido) {
        p = temp->next;
        free(temp);
        return p;
    }


    while (temp != NULL && temp->pid != pedido) {
        prev = temp;
        temp = temp->next;
    }


    if (temp == NULL)
        return NULL;


    prev->next = temp->next;

    free(temp);
    return p;
}
*/


char ** InitArray(int N){
     char **array = (char**)malloc(N * sizeof(char**));

    for (int c = 0; c < N; c++) {
        array[c] = malloc(30 * sizeof(char *));
    }
    return array;
}

struct Pedidos * getPedidoCabeca (ListaPedidos p){
    Pedidos P = malloc(sizeof( struct Pedidos));
    P->prioridade = -1;
    P->pid = p->p->pid;
    P->nTransf = p->p->nTransf;
    P->idCliente = p->p->idCliente;
    P->destination = strdup(p->p->destination);
    P->source = strdup(p->p->source);
    P->mutacoes = strdup(p->p->mutacoes);

    return P;
}

struct ListaPedidos * retiraListaCabeca (ListaPedidos p){
    p=p->prox;

    return p;
}

struct Pedidos* devolvePedido (ListaPedidos p, int id )
{

    Pedidos pedido;
    if (p->p->idCliente == id) {
        pedido = clonePedido(p->p);
        return pedido;
    }
    while (p->prox != NULL && p->p->idCliente != id) {
        p=p->prox;
    }

    pedido = clonePedido(p->p);

    return pedido;
}

int contabarraNs (const char *s){
    int r=0;

    if(s){
    for(int i=0; s[i]!='\0';i++){
        if(s[i]=='\n') r++;
    }
    }

    return r;
}

