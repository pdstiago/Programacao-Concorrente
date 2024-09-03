/*
Compilar:
gcc -o sequencial sequencial.c -Wall -O2

Executar:
./sequencial arquivoEntradaDaPrimeiraMatriz arquivoEntradaDaSegundaMatriz arquivoSaída
*/

#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

float *matrizA, *matrizB; // matrizes que serão carregadas dos arquivos
float *matrizC; // matriz da multiplicação

// inicializa a matrizC com todas as posições iguais a 0
void inicializa(int N, int M){
    for(int i=0; i<N; i++){
        for(int j=0; j<M; j++) matrizC[i*M + j] = 0;
    }
}

// realiza a multiplicação das matrizes A e B e guarda em C
void multiplica(int N, int M, int Q){
    for(int i=0; i<N; i++){
        for(int j=0; j<M; j++){
            for(int k=0; k<Q; k++){
                matrizC[i*M + j] += matrizA[i*Q + k] * matrizB[k*M + j];
            }
        }
    }
}

//#define TEXTO

int main(int argc, char*argv[]) {
    int linhasA, colunasA; // dimensões da matriz A
    int linhasB, colunasB; // dimensões da matriz B
    int linhasC, colunasC; // dimensões da matriz C
    long long int tamA, tamB, tamC; // qtd de elementos das matrizes
    double inicio, fim, delta;
    FILE *descritorArquivo; // descritor dos arquivos de entrada
    size_t ret; // retorno da função de leitura no arquivo de entrada
    
    GET_TIME(inicio);

    // recebe os argumentos de entrada
    if(argc<4){
        fprintf(stderr, "Digite: %s <arquivo entrada da primeira matriz> <arquivo entrada da segunda matriz> <arquivo sáida> \n", argv[0]);
        return 1;
    }
    
    // abre o arquivo para leitura binária
    descritorArquivo = fopen(argv[1], "rb");
    if(!descritorArquivo){
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 2;
    }
    
    // le as dimensões da matrizA
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
    tamA = linhasA * colunasA; // calcula a qtd de elementos da matrizA

    // aloca memória para a matrizA
    matrizA = (float*)malloc(sizeof(float) * tamA);
    if(!matrizA){
        fprintf(stderr, "Erro de alocação da memória da matriz\n");
        return 4;
    }
    // carrega a matriz de elementos do tipo float do arquivo
    ret = fread(matrizA, sizeof(float), tamA, descritorArquivo);
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

    // le as dimensões da matrizB
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
    tamB = linhasB * colunasB; //calcula a qtd de elementos da matrizB

    // aloca memória para a matrizB
    matrizB = (float*)malloc(sizeof(float) * tamB);
    if(!matrizB){
        fprintf(stderr, "Erro de alocação da memória da matriz\n");
        return 4;
    }
    // carrega a matriz de elementos do tipo float do arquivo
    ret = fread(matrizB, sizeof(float), tamB, descritorArquivo);
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

    // aloca memória para a matrizC
    matrizC = (float*)malloc(sizeof(float) * tamC);
    if(!matrizC){
        fprintf(stderr, "Erro de alocação da memória da matriz\n");
        return 4;
    }

    inicializa(linhasC, colunasC);

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo inicialização: %lf\n", delta);

    GET_TIME(inicio);

    multiplica(linhasA, colunasB, colunasA);

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo multiplicação (dimensão %d): %lf\n", linhasC, delta);

    // imprimir na saída padrão a matriz gerada
    #ifdef TEXTO
    for(int i=0; i<linhasC; i++){
        for(int j=0; j<colunasC; j++)
        fprintf(stdout, "%.6f ", matrizC[i*colunasC+j]);
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
    ret = fwrite(matrizC, sizeof(float), tamC, descritorArquivo);
    if(ret<tamC){
        fprintf(stderr, "Erro de escrita no arquivo\n");
        return 6;
    }
    GET_TIME(inicio);

    // finaliza o uso das variáveis
    fclose(descritorArquivo);
    free(matrizA);
    free(matrizB);
    free(matrizC);

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo finalização: %lf\n", delta);
    return 0;
}