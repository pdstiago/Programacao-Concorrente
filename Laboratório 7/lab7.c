/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: As tres threads executam de acordo com as transições de estado (t1-t2-t3) */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 3

FILE *fileEntrada, *fileSaida;

// Variaveis globais
sem_t jaImprimiu, jaProcessou, podeProcessar;     //semaforos para coordenar a ordem de execucao das threads

int N, posBuffer1, posBuffer2;

int n, cont, total, impressos, pos;

char *buffer1, *buffer2, *entrada;

//funcao executada pela thread 1
void *t1 (void *arg) {
    while(impressos<total){
        sem_wait(&jaImprimiu);
        
        while(pos<total && posBuffer1<N){
            printf("%c\n", entrada[pos]);
            buffer1[posBuffer1++] = entrada[pos++];
        }

        printf("hear %d\n", posBuffer1);

        sem_post(&podeProcessar);
    }
    printf("acabei 1\n");
    pthread_exit(NULL);
}

//funcao executada pela thread 2
void *t2 (void *arg) {
    while(impressos<total){
        sem_wait(&podeProcessar);

        printf("aqui %d\n", posBuffer1);

        for(int i=0; i<posBuffer1; i++){
            buffer2[posBuffer2++] = buffer1[i];
            cont++;

            if(n==10){
                if(cont == 10){
                    buffer2[posBuffer2++]='\n';
                    cont = 0;
                }
            }else if (cont == 2*n+1){
                buffer2[posBuffer2++]='\n';
                cont = 0;
                if(n<10) n++;
            }
        }

        printf("aqui 2 %d\n", posBuffer2);

        sem_post(&jaProcessou);
    }

    pthread_exit(NULL);
}

//funcao executada pela thread 3
void *t3 (void *arg) {
    while(impressos<total){
        sem_wait(&jaProcessou);

        printf("ue\n");

        for(int i=0; i<posBuffer2; i++){
            if(buffer2[i]!='\n') impressos++;
            fprintf(fileSaida, "%c", buffer2[i]);
        }

        posBuffer1 = posBuffer2 = 0;

        sem_post(&jaImprimiu);
    }

    pthread_exit(NULL);
}

//funcao principal
int main(int argc, char *argv[]) {
    pthread_t tid[NTHREADS];

    //--le e avalia os parametros de entrada
    if(argc<2) {
        printf("Digite: %s <N>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);

    buffer1 = (char*)malloc(sizeof(char) * N);
    buffer2 = (char*)malloc(sizeof(char) * 2 * N);

    if(!buffer1 || !buffer2){
        printf("--ERRO: malloc()\n"); exit(-1);
    }

    fileEntrada = fopen("entrada.txt", "r");
    fileSaida = fopen("saida.txt", "w");

    if(!fileEntrada){
        printf("--ERRO: não é possível abrir o arquivo\n"); exit(-1);
    }
    if(!fileSaida){
        printf("--ERRO: não é possível escrever no arquivo\n"); exit(-1);
    }

    char c;
    while((c = fgetc(fileEntrada)) != EOF){
        total++;
    }

    entrada = (char*)malloc(sizeof(char)*total);

    rewind(fileEntrada);

    int pos = 0;
    while((c = fgetc(fileEntrada)) != EOF){
        entrada[pos++] = c;
    }

    //inicia os semaforos
    sem_init(&jaImprimiu, 0, 1);
    sem_init(&jaProcessou, 0, 0);
    sem_init(&podeProcessar, 0, 0);

    //cria as tres threads
    if (pthread_create(&tid[2], NULL, t3, NULL)) { 
        printf("--ERRO: pthread_create()\n"); exit(-1); }
    if (pthread_create(&tid[1], NULL, t2, NULL)) { 
        printf("--ERRO: pthread_create()\n"); exit(-1); }
    if (pthread_create(&tid[0], NULL, t1, NULL)) { 
        printf("--ERRO: pthread_create()\n"); exit(-1); }

    //--espera todas as threads terminarem
    for (int t=0; t<NTHREADS; t++) {
        if (pthread_join(tid[t], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        } 
    } 

    sem_destroy(&jaImprimiu);
    sem_destroy(&jaProcessou);
    sem_destroy(&podeProcessar);

    fclose(fileEntrada);
    fclose(fileSaida);

    return 0;
}