#ifndef SIMULACAO_H
#define SIMULACAO_H

#define SUPERIOR 1310171
#define VELOCIDADE 9806

typedef struct Node {
    int cliente;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
} Fila;

typedef struct {
    int id;  // identificador do atendente
} Atendente;

unsigned randomico(unsigned anterior);

int chegada(int media, int semente);

int atendimento(int media, int semente);

Fila* criarFila();

void enfileirar(Fila* fila, int cliente);

int desenfileirar(Fila* fila);

void simular(int numAtendentes, int tempoTotal, int tempoChegada, int tempoOperacao, int semente);

#endif /* SIMULACAO_H */
