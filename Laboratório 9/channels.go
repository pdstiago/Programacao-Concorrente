//introduz canais na aplicação Go

/*
	Executar: go run channels.go
*/

package main

import (
	"fmt"
)

func tarefa(str chan string) {
	msg := <-str
	fmt.Println(msg, "(impresso pela Goroutine)")
	str <- "Oi Main, bom dia, tudo bem?"

	msg = <-str
	fmt.Println(msg, "(impresso pela Goroutine)")
	str <- "Certo, entendido."

	msg = <-str
	fmt.Println(msg, "(impresso pela Goroutine)")
	str <- "finalizando"
}

func main() {
	var msg = "" //string vazia
	
	//cria um canal de comunicacao nao-bufferizado
	str := make(chan string)
	
	//cria um goroutine que executará a função 'tarefa'
	go tarefa(str)
	
	str <- "Olá, Goroutine, bom dia!"
	msg = <-str
	fmt.Println(msg, "(impresso pela Main)")
	

	str <- "Tudo bem! Vou terminar tá?"
	msg = <-str
	fmt.Println(msg, "(impresso pela Main)")


	str <- "finalizando"
	msg = <-str
	fmt.Println(msg, "(impresso pela Main)")
}
