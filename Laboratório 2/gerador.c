/*
Programa auxiliar para gerar os casos de teste

Compilar:
gcc -o gerador gerador.c -Wall

Executar:
./gerador N nomeDoArquivo

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000 // valor máximo de um elemento dos vetores

// descomentar o define abaixo caso deseje imprimir os vetores gerados e o produto interno no formato texto
//#define TEXTO 

int main(int argc, char*argv[]){
    int n; // qtd de elementos dos vetores que serão gerados
    int fator; // fator multiplicador para gerar números negativos

    float *vetorA; // vetor A que será gerado
    float *vetorB; // vetor B que será gerado
    float elem; // valor gerado para incluir no vetor
    double prodInter = 0; // produto interno dos elementos gerados

    FILE * descritorArquivo; // descritor do arquivo de saída
    size_t ret; // retorno da função de escrita no arquivo de saída

    // recebe os argumentos de entrada
    if(argc<3){
        fprintf(stderr, "Digite: %s <dimensão> <arquivo saída>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);

    // aloca memória para o vetorA
    vetorA = (float*)malloc(sizeof(float)*n);
    if(!vetorA){
        fprintf(stderr, "Erro de alocação da memória do vetor A\n");
        return 2;
    }

    // aloca memória para o vetorB
    vetorB = (float*)malloc(sizeof(float)*n);
    if(!vetorB){
        fprintf(stderr, "Erro de alocação da memória do vetor B\n");
        return 2;
    }

    // preenche o vetorA com valores float aleatórios
    srand(time(NULL));
    for(int i=0; i<n; i++){
        int isNeg = rand()%2; // indica se o número vai ser negativo ou não com 50% de chance
        fator = 1;
        if(isNeg) fator = -1;

        float div = rand()%5+1; // indica qual será o denominador do elemento, varia de 1 a 5

        elem = (rand() % MAX) / div * fator;
        vetorA[i] = elem;
    }

    // preenche o vetorB com valores float aleatórios
    for(int i=0; i<n; i++){
        int isNeg = (rand()%2); // indica se o número vai ser negativo ou não com 50% de chance
        fator = 1;
        if(isNeg) fator = -1;

        float div = rand()%5+1; // indica qual será o denominador do elemento, varia de 1 a 5

        elem = (rand() % MAX) / div * fator;
        vetorB[i] = elem;
    }

    for(int i=0; i<n; i++){
        prodInter += vetorA[i] * vetorB[i];
    }

    // abre o arquivo para escrita binária
    descritorArquivo = fopen(argv[2], "wb");
    if(!descritorArquivo){
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 3;
    }

    // imprimir na saída padrão os vetores gerados e o produto interno
    #ifdef TEXTO
    fprintf(stdout, "%d\n", n);
    for(int i=0; i<n; i++) {
        fprintf(stdout, "%f ",vetorA[i]);
    }
    fprintf(stdout, "\n");
    for(int i=0; i<n; i++) {
        fprintf(stdout, "%f ",vetorB[i]);
    }
    fprintf(stdout, "\n");
    fprintf(stdout, "soma=%lf\n", prodInter);
    #endif

    // escreve a dimensão
    ret = fwrite(&n, sizeof(int), 1, descritorArquivo);

    // escreve os elementos do vetorA
    ret = fwrite(vetorA, sizeof(float), n, descritorArquivo);
    if(ret<n){
        fprintf(stderr, "Erro de escrita no arquivo\n");
        return 4;
    }

    // escreve os elementos do vetorB
    ret = fwrite(vetorB, sizeof(float), n, descritorArquivo);
    if(ret<n){
        fprintf(stderr, "Erro de escrita no arquivo\n");
        return 4;
    }

    // escreve o produto interno
    ret = fwrite(&prodInter, sizeof(double), 1, descritorArquivo);

    // finaliza o uso das variáveis
    fclose(descritorArquivo);
    free(vetorA);
    free(vetorB);

    return 0;
} 
