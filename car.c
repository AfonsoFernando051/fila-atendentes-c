#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SUPERIOR 1310171
#define VELOCIDADE 9806

typedef struct {
    int id;  // identificador do atendente
    Fila fila;  // tamanho da fila do atendente
} Atendente;

typedef struct Node {
    int cliente;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
} Fila;

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

Atendente* criarAtendentes(int numAtendentes) {
    Atendente* atendentes = (Atendente*)malloc(numAtendentes * sizeof(Atendente));
    for (int i = 0; i < numAtendentes; i++) {
        atendentes[i].id = i + 1;
        atendentes[i].fila = *criarFila();
    }
    return atendentes;
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

Node* desenfileirar(Fila* fila) {
    if (fila->front == NULL) {
        return NULL;  // fila vazia
    }

    Node* temp = fila->front;

    if (fila->front == fila->rear) {
        fila->front = NULL;
        fila->rear = NULL;
    } else {
        fila->front = fila->front->next;
    }

    return temp;
}

void simular(int numAtendentes, int tempoTotal, int tempoChegada, int tempoOperacao, int semente) {
    srand(semente);

    Atendente* atendentes = criarAtendentes(numAtendentes);

    printf("Rodando simulacao de %d minuto(s) com %d fila(s)\n", tempoTotal, numAtendentes);
    printf("Tempos medios: chegada = %d; operacao = %d\n", tempoChegada, tempoOperacao);
    printf("EVENTOS\n-------\n");

    int tempoAtual = 0;
    int clienteId = 1;

    while (tempoAtual < tempoTotal) {
        int chegadaCliente = chegada(tempoChegada, semente);
        int atendenteDisponivel = -1;
        int menorFila = numAtendentes + 1;

        // Encontrar atendente com a menor fila
        for (int i = 0; i < numAtendentes; i++) {
            if (atendentes[i].fila < menorFila) {
                menorFila = atendentes[i].fila;
                atendenteDisponivel = i;
            }
        }

        if (chegadaCliente < tempoTotal - tempoAtual) {
            printf("%06d\nChegada do cliente %d no box %d (tamanho fila: %d)\n", tempoAtual, clienteId, atendenteDisponivel + 1, menorFila);
            enfileirar(&atendentes[atendenteDisponivel].fila, clienteId);
            clienteId++;
        }

        // Verificar se algum atendente está livre
        for (int i = 0; i < numAtendentes; i++) {
            if (atendentes[i].fila > 0) {
                Node* clienteAtendido = desenfileirar(&atendentes[i].fila);
                    if (clienteAtendido != NULL) {
                        int clienteId = clienteAtendido->cliente;
                        printf("%06d\nInicio de atendimento do cliente %d box %d (tempo na fila: %d)\n", tempoAtual, clienteId, i + 1, tempoAtual - chegadaCliente);
                        int tempoAtendimento = atendimento(tempoOperacao, semente);
                        tempoAtual += tempoAtendimento;
                        printf("%06d\nFinal de atendimento do cliente %d no box %d (fila: %d; total: %d)\n", tempoAtual, clienteId, i + 1, atendentes[i].fila.front, tempoAtendimento);
                        free(clienteAtendido);  // Liberar a memória alocada para o nó do cliente
                    }
                }
        }

        tempoAtual += chegadaCliente;
    }

    printf("RESULTADOS\n----------\n");
    printf("Total de clientes atendidos: %d\n", clienteId - 1);
    // Calcule e imprima as médias como necessário

    // Liberar memória alocada
    for (int i = 0; i < numAtendentes; i++) {
        free(atendentes[i].fila.front);
    }
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
