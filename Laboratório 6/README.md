### Como compilar:

gcc -o lab6 lab6.c list_int.c -Wall

### Como executar:

./lab6 numDeThreads

### Demonstrações da prioridade de escrita:

#### Parte do primeiro log com 5 threads:
Thread 0 está lendo\
Fila de leitura: list = \
Fila de escrita: list = \

Thread 2 está lendo\
Fila de leitura: list = \
Fila de escrita: list = \

Thread 2 parou de ler\

Thread 2 está esperando para escrever\
Fila de leitura: list = \
Fila de escrita: list = 2 \

Thread 1 está esperando para escrever\
Fila de leitura: list = \
Fila de escrita: list = 1 2 \

Thread 3 está esperando para ler\
Fila de leitura: list = 3 \
Fila de escrita: list = 1 2 \

Thread 0 parou de ler\

Thread 0 está esperando para ler\
Fila de leitura: list = 0 3 \
Fila de escrita: list = 1 2 \

Thread 2 está escrevendo\
Fila de leitura: list = 0 3 \
Fila de escrita: list = 1 \

Thread 2 parou de escrever\

Thread 2 está esperando para ler\
Fila de leitura: list = 0 2 3 \
Fila de escrita: list = 1 \

Thread 1 está escrevendo\
Fila de leitura: list = 0 2 3 \
Fila de escrita: list = \

Thread 1 parou de escrever\

Thread 1 está lendo\
Fila de leitura: list = 0 2 3 \
Fila de escrita: list = \

Thread 1 parou de ler\

Thread 2 está lendo\
Fila de leitura: list = 0 3 \
Fila de escrita: list = \

Thread 2 parou de ler\

Thread 2 está lendo\
Fila de leitura: list = 0 3 \
Fila de escrita: list = \

Thread 0 está lendo\
Fila de leitura: list = 3 \
Fila de escrita: list = \

Thread 0 parou de ler\

#### Parte do terceiro log com 5 threads:
Thread 4 está escrevendo
Fila de leitura: list = 
Fila de escrita: list = 

Thread 2 está esperando para ler
Fila de leitura: list = 2 
Fila de escrita: list = 

Thread 3 está esperando para ler
Fila de leitura: list = 2 3 
Fila de escrita: list = 

Thread 0 está esperando para ler
Fila de leitura: list = 0 2 3 
Fila de escrita: list = 

Thread 4 parou de escrever

Thread 4 está lendo
Fila de leitura: list = 0 2 3 
Fila de escrita: list = 

Thread 4 parou de ler

Thread 4 está lendo
Fila de leitura: list = 0 2 3 
Fila de escrita: list = 

Thread 0 está lendo
Fila de leitura: list = 2 3 
Fila de escrita: list = 

Thread 0 parou de ler

Thread 0 está esperando para escrever
Fila de leitura: list = 2 3 
Fila de escrita: list = 0 

Thread 4 parou de ler

Thread 4 está esperando para ler
Fila de leitura: list = 2 3 4 
Fila de escrita: list = 0 

Thread 1 está escrevendo
Fila de leitura: list = 2 3 4 
Fila de escrita: list = 0 

Thread 1 parou de escrever

Thread 1 está esperando para ler
Fila de leitura: list = 1 2 3 4 
Fila de escrita: list = 0 

Thread 0 está escrevendo
Fila de leitura: list = 1 2 3 4 
Fila de escrita: list = 

Thread 0 parou de escrever

Thread 0 está lendo
Fila de leitura: list = 1 2 3 4 
Fila de escrita: list = 

Thread 0 parou de ler

Thread 4 está lendo
Fila de leitura: list = 1 2 3 
Fila de escrita: list = 

Thread 4 parou de ler

#### Parte do segundo log com 7 threads:
Thread 0 está escrevendo
Fila de leitura: list = 
Fila de escrita: list = 

Thread 5 está esperando para ler
Fila de leitura: list = 5 
Fila de escrita: list = 

Thread 2 está esperando para ler
Fila de leitura: list = 2 5 
Fila de escrita: list = 

Thread 6 está esperando para ler
Fila de leitura: list = 2 5 6 
Fila de escrita: list = 

Thread 1 está esperando para escrever
Fila de leitura: list = 2 5 6 
Fila de escrita: list = 1 

Thread 3 está esperando para ler
Fila de leitura: list = 2 3 5 6 
Fila de escrita: list = 1 

Thread 0 parou de escrever

Thread 0 está esperando para ler
Fila de leitura: list = 0 2 3 5 6 
Fila de escrita: list = 1 

Thread 1 está escrevendo
Fila de leitura: list = 0 2 3 5 6 
Fila de escrita: list = 

Thread 1 parou de escrever

Thread 3 está lendo
Fila de leitura: list = 0 2 5 6 
Fila de escrita: list = 

Thread 3 parou de ler
