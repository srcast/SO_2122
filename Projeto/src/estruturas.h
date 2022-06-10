#ifndef SO_TRABALHO_ESTRUTURAS_H
#define SO_TRABALHO_ESTRUTURAS_H

typedef struct Transf *t;

typedef struct Transf {
    char *transf;
    int ocorr;
    int ocorrmax;
} * Transformacao;

typedef struct Lista *l;

typedef struct Lista {
    Transformacao t;
    struct Lista *prox;
}* Lista;

typedef struct Pedidos *p;

typedef struct Pedidos {
    char *mutacoes;
    int nTransf;
    int idCliente;
    int pid;
    int prioridade;
    char *source;
    char *destination;
} * Pedidos;

typedef struct ListaPedidos *lp;

typedef struct ListaPedidos {
    Pedidos p;
    struct ListaPedidos *prox;
}* ListaPedidos;


struct Transf *createTransf(char *, int);
struct Lista* createList(Transformacao);
struct Lista* insereTransf(Transformacao, Lista);
struct Pedidos *createPedido(int, int, int, int, char*, char*, char*);
struct Pedidos *clonePedido(Pedidos);
struct ListaPedidos* createListPedidos(Pedidos);
struct ListaPedidos* insereTransfPedidos(Pedidos, ListaPedidos);
struct ListaPedidos* insereTransfPedidosPrioridade(Pedidos, ListaPedidos);
struct ListaPedidos* retiraPedido (ListaPedidos, int);
char ** preencheArray (int , char *);
char ** InitArray(int);
struct Pedidos * getPedidoCabeca (ListaPedidos);
struct ListaPedidos * retiraListaCabeca (ListaPedidos);
struct Pedidos* devolvePedido (ListaPedidos, int);
int contabarraNs (const char *);




#endif //SO_TRABALHO_ESTRUTURAS_H
