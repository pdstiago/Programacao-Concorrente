/* Programa que cria arquivo com uma matriz de valores do tipo float, gerados aleatoriamente 
 * Entrada: dimensões da matriz (linhas e colunas) e nome do arquivo de saída
 * Saida: arquivo binário com as dimensões (valores inteiros) da matriz (linhas e colunas), 
 * seguido dos valores (float) de todas as células da matriz gerados aleatoriamente
 * */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//descomentar o define abaixo caso deseje imprimir uma versao truncada da matriz gerada no formato texto
//#define TEXTO 

int main(int argc, char*argv[]) {
   float *matriz; // matriz que será gerada
   int linhas, colunas; // dimensões da matriz
   long long int tam; // qtd de elementos na matriz
   FILE *descritorArquivo; // descritor do arquivo de saída
   size_t ret; // retorno da função de escrita no arquivo de saída
   
   // recebe os argumentos de entrada
   if(argc<4){
      fprintf(stderr, "Digite: %s <linhas> <colunas> <arquivo saida>\n", argv[0]);
      return 1;
   }

   linhas = atoi(argv[1]); 
   colunas = atoi(argv[2]);
   tam = linhas * colunas;

   // aloca memória para a matriz
   matriz = (float*)malloc(sizeof(float) * tam);
   if(!matriz){
      fprintf(stderr, "Erro de alocação da memória da matriz\n");
      return 2;
   }

   // preenche a matriz com valores float aleatórios
   // randomiza a sequância de números aleatórios
   srand(time(NULL));
   for(long int i=0; i<tam; i++){
      float div = rand()%5 + 1;

      *(matriz+i) = (rand() % 1000) / div;
   }

   // imprimir na saída padrão a matriz gerada
   #ifdef TEXTO
   for(int i=0; i<linhas; i++){
      for(int j=0; j<colunas; j++)
      fprintf(stdout, "%.6f ", matriz[i*colunas+j]);
      fprintf(stdout, "\n");
   }
   #endif

   // escreve a matriz no arquivo
   // abre o arquivo para escrita binária
   descritorArquivo = fopen(argv[3], "wb");
   if(!descritorArquivo){
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 3;
   }
   // escreve número de linhas e de colunas
   ret = fwrite(&linhas, sizeof(int), 1, descritorArquivo);
   ret = fwrite(&colunas, sizeof(int), 1, descritorArquivo);
   // escreve os elementos da matriz
   ret = fwrite(matriz, sizeof(float), tam, descritorArquivo);
   if(ret<tam){
      fprintf(stderr, "Erro de escrita no arquivo\n");
      return 4;
   }

   // finaliza o uso das variáveis
   fclose(descritorArquivo);
   free(matriz);
   return 0;
}