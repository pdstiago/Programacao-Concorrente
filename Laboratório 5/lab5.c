/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Comunicação entre threads usando variável compartilhada e exclusao mutua com bloqueio */

/*
Compilar:
gcc -o lab5 lab5.c -Wall

Executar:
./lab5
*/

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

long int soma = 0; //variavel compartilhada entre as threads
pthread_mutex_t mutex; //variavel de lock para exclusao mutua
int contador = 0; // contador de quantos múltiplos eu já atingi
pthread_cond_t condPodePrintar, condPodeSomar; // condições se eu posso printar o múltiplo e se eu posso somar

//funcao executada pelas threads
void *ExecutaTarefa (void *arg) {
    long int id = (long int) arg;
    printf("Thread : %ld esta executando...\n", id);

    for (int i=0; i<100000; i++) {
        //--entrada na SC
        pthread_mutex_lock(&mutex);
        //--SC (seção critica)

        if(soma%10==0 && contador<20){ // se soma for múltiplo de 10 e ainda não printei os 20 primeiros
            pthread_cond_signal(&condPodePrintar); // aviso que posso printar
            pthread_cond_wait(&condPodeSomar, &mutex); // espero a impressão
        }

        soma++; //incrementa a variavel compartilhada 
        
        //--saida da SC
        pthread_mutex_unlock(&mutex);
    }
    printf("Thread : %ld terminou!\n", id);
    pthread_exit(NULL);
}

//funcao executada pela thread de log
void *extra (void *args) {
    printf("Extra : esta executando...\n");
    while(contador<20){
        pthread_mutex_lock(&mutex);

        if ((soma%10) || soma != contador*10){ // se soma não for múltiplo de 10 ou ainda não atingimos o próximo múltiplo
            pthread_cond_wait(&condPodePrintar, &mutex); // preciso esperar a condição de printar
        }

        printf("soma = %ld \n", soma);
        contador++;

        pthread_cond_signal(&condPodeSomar); // aviso que a ExecutaTarefa pode voltar a somar

        pthread_mutex_unlock(&mutex);
    }
    printf("Extra : terminou!\n");
    pthread_exit(NULL);
}

//fluxo principal
int main(int argc, char *argv[]) {
    pthread_t *tid; //identificadores das threads no sistema
    int nthreads; //qtde de threads

    nthreads = 1;

    //--aloca as estruturas
    tid = (pthread_t*) malloc(sizeof(pthread_t)*(nthreads+1));
    if(tid==NULL) {puts("ERRO--malloc"); return 2;}

    //--inicilaiza o mutex (lock de exclusao mutua)
    pthread_mutex_init(&mutex, NULL);

    //--inicializa as condições
    pthread_cond_init(&condPodePrintar, NULL);
    pthread_cond_init(&condPodeSomar, NULL);

    //--cria as threads
    for(long int t=0; t<nthreads; t++) {
        if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void *)t)) {
        printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }

    //--cria thread de log
    if (pthread_create(&tid[nthreads], NULL, extra, NULL)) {
        printf("--ERRO: pthread_create()\n"); exit(-1);
    }

    //--espera todas as threads terminarem
    for (int t=0; t<nthreads+1; t++) {
        if (pthread_join(tid[t], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        } 
    } 

    //--finaliza o mutex
    pthread_mutex_destroy(&mutex);

    //--finaliza as condições
    pthread_cond_destroy(&condPodePrintar);
    pthread_cond_destroy(&condPodeSomar);
    
    printf("Valor de 'soma' = %ld\n", soma);

    return 0;
}
