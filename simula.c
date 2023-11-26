#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "simula.h"

unsigned randomico(unsigned anterior) {
    return ((unsigned long)VELOCIDADE * (unsigned long)anterior) % SUPERIOR + 1;
}

int chegada(int media, int semente) {
    static unsigned ultimoRandomico = 0;
    if (ultimoRandomico < 1) {
        ultimoRandomico = semente;
    }
    ultimoRandomico = randomico(ultimoRandomico);
    return (int)ceil(-media * log((double)ultimoRandomico / SUPERIOR));
}

int atendimento(int media, int semente) {
    static unsigned ultimoRandomico = 0;
    if (ultimoRandomico < 1) {
        ultimoRandomico = semente;
    }
    ultimoRandomico = randomico(ultimoRandomico);
    return (int)ceil(-media * log((double)ultimoRandomico / SUPERIOR));
}

Fila* criarFila() {
    Fila* fila = (Fila*)malloc(sizeof(Fila));
    fila->front = NULL;
    fila->rear = NULL;
    return fila;
}

void enfileirar(Fila* fila, int cliente) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->cliente = cliente;
    newNode->next = NULL;

    if (fila->rear == NULL) {
        fila->front = newNode;
        fila->rear = newNode;
    } else {
        fila->rear->next = newNode;
        fila->rear = newNode;
    }
}

int desenfileirar(Fila* fila) {
    if (fila->front == NULL) {
        return -1;  // fila vazia
    }

    int cliente = fila->front->cliente;
    Node* temp = fila->front;

    if (fila->front == fila->rear) {
        fila->front = NULL;
        fila->rear = NULL;
    } else {
        fila->front = fila->front->next;
    }

    free(temp);
    return cliente;
}

void simular(int numAtendentes, int tempoTotal, int tempoChegada, int tempoOperacao, int semente) {
    srand(semente);

    Atendente* atendentes = (Atendente*)malloc(numAtendentes * sizeof(Atendente));

    printf("Rodando simulacao de %d minuto(s) com %d fila(s)\n", tempoTotal, numAtendentes);
    printf("Tempos medios: chegada = %d; operacao = %d\n", tempoChegada, tempoOperacao);
    printf("EVENTOS\n-------\n");

    int tempoAtual = 0;
    int clienteId = 1;

    Fila* filaGlobal = criarFila();

    while (tempoAtual < tempoTotal) {
        int chegadaCliente = chegada(tempoChegada, semente);

        if (chegadaCliente < tempoTotal - tempoAtual) {
            printf("%06d\nChegada do cliente %d (tamanho fila global: %d)\n", tempoAtual, clienteId, filaGlobal->rear == NULL ? 0 : 1);
            enfileirar(filaGlobal, clienteId);
            clienteId++;
        }

        for (int i = 0; i < numAtendentes; i++) {
            if (atendentes[i].id == 0 && filaGlobal->front != NULL) {
                int clienteAtendido = desenfileirar(filaGlobal);
                atendentes[i].id = clienteAtendido;
                printf("%06d\nInicio de atendimento do cliente %d no box %d (tempo na fila: %d)\n", tempoAtual, clienteAtendido, i + 1, tempoAtual - chegadaCliente);
                int tempoAtendimento = atendimento(tempoOperacao, semente);
                tempoAtual += tempoAtendimento;
                printf("%06d\nFinal de atendimento do cliente %d no box %d (fila: %d; total: %d)\n", tempoAtual, clienteAtendido, i + 1, filaGlobal->rear == NULL ? 0 : 1, tempoAtendimento);
            }
        }

        tempoAtual += chegadaCliente;
    }

    printf("RESULTADOS\n----------\n");
    printf("Total de clientes atendidos: %d\n", clienteId - 1);
    // Calcule e imprima as médias como necessário

    // Liberar memória alocada
    free(filaGlobal);
    free(atendentes);
}

int main(int argc, char* argv[]) {
    if (argc != 6) {
        printf("Utilizacao: %s <filas> <tempototal> <tempochegada> <tempooperacao> <semente>\n", argv[0]);
        printf("<filas>: numero de filas\n<tempototal>: tempo total da simulacao (minutos)\n");
        printf("<tempochegada>: tempo medio entre chegadas de clientes (minutos)\n");
        printf("<tempooperacao>: tempo medio de uma operacao de atendimento (minutos)\n");
        printf("<semente>: numero inteiro para geracao de numeros pseudo-aleatorios\n");
        return 1; // Código de erro
    }

    int filas = atoi(argv[1]);
    int tempototal = atoi(argv[2]);
    int tempochegada = atoi(argv[3]);
    int tempooperacao = atoi(argv[4]);
    int semente = atoi(argv[5]);

    simular(filas, tempototal, tempochegada, tempooperacao, semente);

    return 0;
}