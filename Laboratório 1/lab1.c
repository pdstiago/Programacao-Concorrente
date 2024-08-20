/*

Compilar:
gcc -o lab1 lab1.c -Wall

Executar:
./lab1 M N

*/

// Há uma breve explicação de como o código funciona na função main

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

typedef struct {
    int id, range, posResto;
} t_Args;

int *vetor;

void *sum1ToArray(void* arg){
    t_Args *args = (t_Args*) arg;

    int id = args->id;
    int range = args->range;
    int posResto = args->posResto;

    for(int i = id * range; i < (id+1) * range; i++){ // passo por "range" posições consecutivas
        vetor[i]++;
    }

    if(posResto!=-1){
        vetor[posResto]++; // modifico uma das posição que sobraram do final
    }

    free(arg);
    pthread_exit(NULL);
}

int inicializarVetor(int N){
    vetor = (int*)malloc(sizeof(int) * N);

    if(vetor == NULL) return 2;

    for(int i=0; i<N; i++) vetor[i]=0;

    return 0;
}

int resultadoCorreto(int N){
    for(int i=0; i<N; i++){
        if(vetor[i]!=1) return 5;
    }
    return 0;
}


int main(int argc, char *argv[]) {
    if(argc<3) {
        printf("--ERRO: informe M, a quantidade de threads e N, a quantidade de elementos do vetor <%s>\n", argv[0]);
        return 1;
    }

    int M = atoi(argv[1]);
    int N = atoi(argv[2]);

    // identificadores das threads no sistema
    pthread_t threads[M];

    if(inicializarVetor(N) == 2){
        printf("--ERRO: malloc()\n"); 
        return 2;
    }

    int range = N/M; // range indica a quantidade de posições consecutivas que cada thread vai modificar, sem contar com aquelas que sobrarão no final, caso tenha resto
    int resto = N%M;

    /*
    Pequena explicação de como vai funcionar:

    a thread de id 0 irá modificar as primeiras "range" posições, a de id 1, as próximas "range" posições e assim por diante...
    então se tivéssemos M = 3 e N = 9, teríamos, 0 0 0 1 1 1 2 2 2, em que o número na posição indica a thread que modificou o valor
    
    porém se resto>0, então vão sobrar posições no final que não serão modificadas, sendo assim,
    as últimas "resto" posições serão modificadas pelas thread de id 0 até resto-1, respectivamente.
    então se tivéssemos M = 3 e N = 8, teríamos, 0 0 1 1 2 2 0 1, em que o número na posição indica a thread que modificou o valor
    */

    // cria as threads
    for(int i=0; i<M; i++){
        t_Args *args;
        args = malloc(sizeof(t_Args));

        if(args == NULL){
            printf("--ERRO: malloc()\n"); 
            return 2;
        }

        args->id = i; // identificador da thread
        args->range = range; // quantas posições sequenciais cada thread vai modificar
        args->posResto = -1; // qual posição final aquela thread vai modificar, indico inicialmente que aquela thread não vai modificar uma das "resto" posições finais, declarando com -1

        if(resto>0 && i<=resto-1){ // se houver resto, e o indice atual é um daqueles que realizará a modificação, então posResto será a i-ésima posição que sobrou
            args->posResto = M * range + i;
        }

        if(pthread_create(&threads[i], NULL, sum1ToArray, (void*) args)) {
            printf("--ERRO: pthread_create()\n"); 
            return 3;
        }
    }

    // espera todas as threads terminarem
    for(int i=0; i<M; i++){
        if(pthread_join(threads[i], NULL)){
            printf("--ERRO: pthread_join() da thread %d\n", i);
            return 4;
        }
    }

    // checa se o resultado está correto
    if(resultadoCorreto(N)==5){
        printf("--ERRO: os valores finais do vetor não estão corretos!");
        return 5;
    }

    free(vetor);

    // log da função principal
    printf("--Thread principal terminou\n");

    printf("Todos os valores estão corretos!\n");
}