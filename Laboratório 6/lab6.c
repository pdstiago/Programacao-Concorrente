//Programa concorrente que cria e faz operacoes sobre uma lista de inteiros

#include <stdio.h>
#include <stdlib.h>
#include "list_int.h"
#include <pthread.h>

#define QTDE_OPS 50 //quantidade de operacoes sobre a lista (insercao, remocao, consulta)
#define QTDE_INI 100 //quantidade de insercoes iniciais na lista
#define MAX_VALUE 100 //valor maximo a ser inserido

//lista compartilhada iniciada 
struct list_node_s *head_p = NULL, *head_lei = NULL, *head_esc = NULL;
//qtde de threads no programa
int nthreads;

//lock de exclusao mutua
pthread_mutex_t mutex;

pthread_cond_t liberaEscrita, liberaLeitura;

int lei, esc, escEsperando;

void printarFilas(){
    printf("Fila de leitura: ");
    Print(head_lei);

    printf("Fila de escrita: ");
    Print(head_esc);

    printf("\n");
}

void entraLeitura(int id){
    pthread_mutex_lock(&mutex);

    int entrou = 0;
    while(esc || escEsperando){
        if(!entrou){
            printf("Thread %d está esperando para ler\n", id);
            Insert(id, &head_lei);
            printarFilas();
            entrou = 1;
        }
        pthread_cond_wait(&liberaLeitura, &mutex);
    }

    if(entrou) Delete(id, &head_lei);

    printf("Thread %d está lendo\n", id);
    printarFilas();

    lei++;

    pthread_mutex_unlock(&mutex);
}

void saiLeitura(int id){
    pthread_mutex_lock(&mutex);
    printf("Thread %d parou de ler\n\n", id);
    lei--;
    if(!lei) pthread_cond_broadcast(&liberaEscrita);
    pthread_mutex_unlock(&mutex);
}

void entraEscrita(int id){
    pthread_mutex_lock(&mutex);

    int entrou = 0;

    while(esc || lei){
        if(!entrou){
            escEsperando++;
            entrou = 1;
            Insert(id, &head_esc);
            printf("Thread %d está esperando para escrever\n", id);
            printarFilas();
        }
        pthread_cond_wait(&liberaEscrita, &mutex);
    }

    if(entrou){
        Delete(id, &head_esc);
        escEsperando--;
    }

    printf("Thread %d está escrevendo\n", id);
    printarFilas();

    esc++;

    pthread_mutex_unlock(&mutex);
}

void saiEscrita(int id){
    pthread_mutex_lock(&mutex);

    printf("Thread %d parou de escrever\n\n", id);

    esc--;

    if(!escEsperando) pthread_cond_broadcast(&liberaLeitura);

    pthread_cond_broadcast(&liberaEscrita);

    pthread_mutex_unlock(&mutex);
}

//tarefa das threads
void* tarefa(void* arg) {
    long int id = (long int) arg;
    int op;
    int in, out, read; 
    in=out=read = 0; 

    //realiza operacoes de consulta (80%), insercao (10%) e remocao (10%)
    for(long int i=id; i<QTDE_OPS; i+=nthreads) {
        op = rand() % 100;
        if(op<80){
            entraLeitura(id); /* lock de LEITURA */    
            Member(i%MAX_VALUE, head_p);   /* Ignore return value */
            saiLeitura(id);
            read++;
        } else if(80<=op && op<90) {
            entraEscrita(id); /* lock de ESCRITA */    
            Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
            saiEscrita(id);
            in++;
        } else if(op>=90) {
            entraEscrita(id); /* lock de ESCRITA */     
            Delete(i%MAX_VALUE, &head_p);  /* Ignore return value */
            saiEscrita(id);
            out++;
        }
    }
    //registra a qtde de operacoes realizadas por tipo
    printf("Thread %ld: in=%d out=%d read=%d\n\n", id, in, out, read);
    pthread_exit(NULL);
}

/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {
    pthread_t *tid;
    
    //verifica se o numero de threads foi passado na linha de comando
    if(argc<2) {
        printf("Digite: %s <numero de threads>\n", argv[0]); return 1;
    }
    nthreads = atoi(argv[1]);

    //insere os primeiros elementos na lista
    for(int i=0; i<QTDE_INI; i++)
        Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
    

    //aloca espaco de memoria para o vetor de identificadores de threads no sistema
    tid = malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL) {  
        printf("--ERRO: malloc()\n"); return 2;
    }

    //inicializa a variavel mutex
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&liberaLeitura, NULL);
    pthread_cond_init(&liberaEscrita, NULL);
    
    //cria as threads
    for(long int i=0; i<nthreads; i++) {
        if(pthread_create(tid+i, NULL, tarefa, (void*) i)) {
            printf("--ERRO: pthread_create()\n"); return 3;
        }
    }
    
    //aguarda as threads terminarem
    for(int i=0; i<nthreads; i++) {
        if(pthread_join(*(tid+i), NULL)) {
            printf("--ERRO: pthread_join()\n"); return 4;
        }
    }

    //libera o mutex
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&liberaLeitura);
    pthread_cond_destroy(&liberaEscrita);
    //libera o espaco de memoria do vetor de threads
    free(tid);
    //libera o espaco de memoria da lista
    Free_list(&head_p);
    Free_list(&head_lei);
    Free_list(&head_esc);

    return 0;
}  /* main */
