/*
	Compilar: go build primos.go
	Executar: ./primos <N> <M>
*/

package main

import (
	"fmt"
	"math"
	"os"
	"strconv"
)

func ehPrimo(n int) int {
	if n <= 1 {
		return 0
	}
	if n == 2 {
		return 1
	}
	if n%2 == 0 {
		return 0
	}
	for i := 3; i < int(math.Sqrt(float64(n)))+1; i += 2 {
		if n%i == 0 {
			return 0
		}
	}
	return 1
}

func main() {
	if len(os.Args) < 3 {
		fmt.Printf("Digite: %s <N> <M>\n", os.Args[0])
		os.Exit(1)
	}

	N, errN := strconv.Atoi(os.Args[1])
	M, errM := strconv.Atoi(os.Args[2])

	if errN != nil || errM != nil {
		fmt.Println("--ERRO: <N> e <M> devem ser inteiros")
		os.Exit(1)
	}

	numeros := make(chan int, N)
	resultadosEhPrimo := make(chan int, N)

	for i := 0; i < M; i++ {
		go func() {
			for {
				j, more := <-numeros
				if !more {
					return
				}
				resultadosEhPrimo <- ehPrimo(j)
			}
		}()
	}

	for i := 1; i <= N; i++ {
		numeros <- i
	}
	close(numeros)

	primos := 0

	for i := 1; i <= N; i++ {
		primos += <-resultadosEhPrimo
	}

	fmt.Println("Quantidade total de números primos encontrados:", primos)
}
