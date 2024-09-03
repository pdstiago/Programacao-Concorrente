/* Multiplicação de matriz-matriz concorrente */

/*
Compilar:
gcc -o concorrente concorrente.c -Wall -O2

Executar:
./concorrente arquivoEntradaDaPrimeiraMatriz arquivoEntradaDaSegundaMatriz arquivoSaída númeroDeThreads
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

float *matA; // matA de entrada
float *matB; // matB de entrada
float *matC; // matC de saída
int nthreads; // número de threads

typedef struct{
    int id; // identificador do elemento que a thread irá processar
    int tam; // qtd de elementos da matriz C
    int col; // qtd de colunas da matriz C
    int dim; // qtd de colunas da matriz A
} tArgs;

// função que as threads executarão
void *tarefa(void *arg){
    tArgs *args = (tArgs*) arg;

    int id = args->id;
    int tam = args->tam;
    int col = args->col;
    int dim = args->dim;

    for(int i=id; i<tam; i+=nthreads){
        int x = i / col; // linha que eu vou calcular da matriz C
        int y = i % col; // coluna que eu vou calcular da matriz C

        for(int j=0; j<dim; j++){
            matC[i] += matA[x * dim + j] * matB[j * col + y];
        }
    }
    pthread_exit(NULL);
}

// inicializa a matriz C com todas as posições iguais a 0
void inicializa(int N, int M){
    for(int i=0; i<N; i++){
        for(int j=0; j<M; j++) matC[i*M + j] = 0;
    }
}

//#define TEXTO

// fluxo principal
int main(int argc, char* argv[]) {
    int linhasA, colunasA; // dimensões da matriz A
    int linhasB, colunasB; // dimensões da matriz B
    int linhasC, colunasC; // dimensões da matriz C
    long long int tamA, tamB, tamC; // qtd de elementos das matrizes
    pthread_t *tid; // identificadores das threads no sistema
    tArgs *args; // identificadores locais das threads e dimensão
    double inicio, fim, delta;
    FILE *descritorArquivo; // descritor dos arquivos de entrada
    size_t ret; // retorno da função de leitura no arquivo de entrada
    
    GET_TIME(inicio);

    // recebe os argumentos de entrada
    if(argc<5){
        fprintf(stderr, "Digite: %s <arquivo entrada da primeira matriz> <arquivo entrada da segunda matriz> <arquivo sáida> <número de threads> \n", argv[0]);
        return 1;
    }

    // abre o arquivo para leitura binária
    descritorArquivo = fopen(argv[1], "rb");
    if(!descritorArquivo){
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 2;
    }

    // le as dimensões da matA
    ret = fread(&linhasA, sizeof(int), 1, descritorArquivo);
    if(!ret){
        fprintf(stderr, "Erro de leitura das dimensões da matriz arquivo \n");
        return 3;
    }
    ret = fread(&colunasA, sizeof(int), 1, descritorArquivo);
    if(!ret){
        fprintf(stderr, "Erro de leitura das dimensões da matriz arquivo \n");
        return 3;
    }
    tamA = linhasA * colunasA; // calcula a qtd de elementos da matA

    // aloca memória para a matA
    matA = (float*)malloc(sizeof(float) * tamA);
    if(!matA){
        fprintf(stderr, "Erro de alocação da memória da matriz\n");
        return 4;
    }
    // carrega a matriz de elementos do tipo float do arquivo
    ret = fread(matA, sizeof(float), tamA, descritorArquivo);
    if(ret<tamA){
        fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
        return 3;
    }

    // abre o arquivo para leitura binária
    descritorArquivo = fopen(argv[2], "rb");
    if(!descritorArquivo){
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 2;
    }

    // le as dimensões da matB
    ret = fread(&linhasB, sizeof(int), 1, descritorArquivo);
    if(!ret){
        fprintf(stderr, "Erro de leitura das dimensões da matriz arquivo \n");
        return 3;
    }
    ret = fread(&colunasB, sizeof(int), 1, descritorArquivo);
    if(!ret){
        fprintf(stderr, "Erro de leitura das dimensões da matriz arquivo \n");
        return 3;
    }
    tamB = linhasB * colunasB; //calcula a qtd de elementos da matB

    // aloca memória para a matB
    matB = (float*)malloc(sizeof(float) * tamB);
    if(!matB){
        fprintf(stderr, "Erro de alocação da memória da matriz\n");
        return 4;
    }
    // carrega a matriz de elementos do tipo float do arquivo
    ret = fread(matB, sizeof(float), tamB, descritorArquivo);
    if(ret<tamB){
        fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
        return 3;
    }

    if(colunasA!=linhasB){
        fprintf(stderr, "Não é possível fazer a multiplicação dessas matrizes, pois a qtd de colunas da primeira deve ser igual a qtd de linhas da segunda!\n");
        return 5;
    }
    
    linhasC = linhasA;
    colunasC = colunasB;
    tamC = linhasC * colunasC;

    // aloca memória para a matC
    matC = (float*)malloc(sizeof(float) * tamC);
    if(!matC){
        fprintf(stderr, "Erro de alocação da memória da matriz\n");
        return 4;
    }

    nthreads = atoi(argv[4]);
    if(nthreads>tamC) nthreads=tamC;

    inicializa(linhasC, colunasC);
    
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo inicialização: %lf\n", delta);

    // multiplicação da matriz pela matriz
    GET_TIME(inicio);

    // alocação das estruturas
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL) {puts("ERRO--malloc"); return 4;}
    args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
    if(args==NULL) {puts("ERRO--malloc"); return 4;}

    // criação das threads
    for(int i=0; i<nthreads; i++){
        (args+i)->id = i;
        (args+i)->tam = tamC;
        (args+i)->col = colunasC;
        (args+i)->dim = colunasA;
        if(pthread_create(tid+i, NULL, tarefa, (void*)(args+i))){
            puts("ERRO--pthread_create"); return 6;
        }
    }

    // espera pelo término da threads
    for(int i=0; i<nthreads; i++) {
        pthread_join(*(tid+i), NULL);
    }

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo multiplicação (dimensão %d) (nthreads %d): %lf\n", linhasC, nthreads, delta);

    // imprimir na saída padrão a matriz gerada
    #ifdef TEXTO
    for(int i=0; i<linhasC; i++){
        for(int j=0; j<colunasC; j++)
        fprintf(stdout, "%.6f ", matC[i*colunasC+j]);
        fprintf(stdout, "\n");
    }
    #endif

    // abre o arquivo para escrita binária
    descritorArquivo = fopen(argv[3], "wb");
    if(!descritorArquivo){
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 2;
    }
    // escreve os elementos da matrizC
    ret = fwrite(matC, sizeof(float), tamC, descritorArquivo);
    if(ret<tamC){
        fprintf(stderr, "Erro de escrita no arquivo\n");
        return 7;
    }

    //liberacao da memoria
    GET_TIME(inicio);
    free(matA);
    free(matB);
    free(matC);
    free(args);
    free(tid);
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo finalização: %lf\n", delta);

    return 0;
}
