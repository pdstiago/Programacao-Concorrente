/*
Programa para validar os casos de teste

Compilar:
gcc -o validador validador.c -Wall

Executar:
./validador nomeDoArquivo nThreads

*/

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h> 

// Variáveis Globais

// tamanho do vetores
int n;

// vetores de elementos
float *vetorA;
float *vetorB;

// número de threads
int nthreads;

/*
Pequena explicação de como vai funcionar:

cada thread irá realizar o produto interno de (N / nThreads) posições consecutivas, sem contar com aquelas que sobrarão no final, caso tenha resto

"range" = N / nThreads

Então, a thread de id 0 irá multiplicar as primeiras "range" posições, a de id 1, as próximas "range" posições e assim por diante...
então se tivéssemos M = 3 e N = 9, teríamos, 0 0 0 1 1 1 2 2 2, em que o número na posição indica a thread que multiplicou os valores daquelas posições

porém se resto>0, então vão sobrar posições no final que não serão calculadas, sendo assim,
as últimas "resto" posições serão multiplicadas pelas thread de id 0 até resto-1, respectivamente.
então se tivéssemos M = 3 e N = 8, teríamos, 0 0 1 1 2 2 0 1, em que o número na posição indica a thread que multiplicou os valores daquela posição
*/

// função executada pelas threads
void *calculaProduto(void *tid){
    long int id = (long int) tid;

    int range = n / nthreads;
    int resto = n % nthreads;

    double *ret, prodLocal = 0;

    for(int i = id * range; i < (id+1) * range; i++){ // passo por "range" posições consecutivas
        prodLocal += vetorA[i] * vetorB[i];
    }

    if(resto>0 && id<=resto-1){ // se houver resto, e o indice atual é um daqueles que realizará a modificação, então posResto será a i-ésima posição que sobrou
        
        int posResto = nthreads * range + id; // qual das "resto" posições finais aquela thread vai modificar

        prodLocal += vetorA[posResto] * vetorB[posResto]; // multiplico uma das posições que sobraram do final
    }

    // retorna o resultado do produto
    ret = malloc(sizeof(double));
    if(ret!=NULL) *ret = prodLocal;
    else printf("--ERRO: malloc() thread\n");
    pthread_exit((void*) ret);
}

//função principal do programa
int main(int argc, char *argv[]){
    FILE *arq; // arquivo de entrada
    size_t ret; // retorno da função de leitura no arquivo de entrada
            
    double prodSenq; // produto registrado no arquivo
    double prodConc = 0; // produto calculado pelas threads
    double *prodThread; // auxiliar para retorno das threads

    pthread_t *tid_sistema; // vetor de identificadores das threads no sistema

    // valida e recebe os valores de entrada
    if(argc < 3) { printf("Use: %s <arquivo de entrada> <número de threads> \n", argv[0]); exit(-1); }

    // abre o arquivo de entrada com os valores para calcular o produto interno
    arq = fopen(argv[1], "rb");
    if(arq==NULL) { printf("--ERRO: fopen()\n"); exit(-1); }

    // le o tamanho do vetor (primeira linha do arquivo)
    ret = fread(&n, sizeof(int), 1, arq);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensões da matriz arquivo \n");
        return 3;
    }

    // aloca espaço de memória e carrega os vetores de entrada
    vetorA = malloc(sizeof(float)*n);
    if(vetorA==NULL) { printf("--ERRO: malloc()\n"); exit(-1); }

    ret = fread(vetorA, sizeof(float), n, arq);
    if(ret<n){
        fprintf(stderr, "Erro de leitura dos elementos do vetorA\n");
        return 4;
    }

    vetorB = malloc(sizeof(float)*n);
    if(vetorB==NULL) { printf("--ERRO: malloc()\n"); exit(-1); }
    
    ret = fread(vetorB, sizeof(float), n, arq);
    if(ret<n){
        fprintf(stderr, "Erro de leitura dos elementos do vetorB\n");
        return 4;
    }

    // le o produto registrado no arquivo
    ret = fread(&prodSenq, sizeof(double), 1, arq);

    // le o número de threads da entrada do usúario 
    nthreads = atoi(argv[2]);

    // limita o número de threads ao tamanho do vetor
    if(nthreads>n) nthreads = n;

    // aloca espaço para o vetor de identificadores das threads no sistema
    tid_sistema = (pthread_t*)malloc(sizeof(pthread_t)*nthreads);
    if(tid_sistema==NULL) { printf("--ERRO: malloc()\n"); exit(-1); }

    // cria as threads
    for(long int i=0; i<nthreads; i++) {
        if(pthread_create(&tid_sistema[i], NULL, calculaProduto, (void*) i)){
            printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }

    for(int i=0; i<nthreads; i++){
        if(pthread_join(tid_sistema[i], (void*) &prodThread)){
            printf("--ERRO: pthread_join()\n"); exit(-1);
        }
        prodConc += *prodThread;
        free(prodThread);
    }

    double var = abs((prodSenq - prodConc) / prodSenq);

    //imprime os resultados
    printf("Produto interno Concorrente = %.26lf\n", prodConc);
    printf("Produto interno Sequencial = %.26lf\n", prodSenq);
    printf("Variação relativa = %.26lf\n", var);

    //desaloca os espacos de memoria
    free(vetorA);
    free(vetorB);
    free(tid_sistema);

    //fecha o arquivo
    fclose(arq);
    return 0;
}
