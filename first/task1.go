package main

import (
	"fmt"
	"math/rand"
	"time"
)

func main() {
	var M, N int
	fmt.Print("Введите количество строк (M): ")
	fmt.Scan(&M)
	fmt.Print("Введите количество столбцов (N): ")
	fmt.Scan(&N)

	if M <= 0 || N <= 0 {
		fmt.Println("Размеры матрицы должны быть положительными числами.")
		return
	}

	matrix := make([][]int, M)
	result := []int{}
	sum := 0

	rand.NewSource(time.Now().UnixNano())

	for i := 0; i < M; i++ {
		matrix[i] = make([]int, N)
		for j := 0; j < N; j++ {
			matrix[i][j] = rand.Intn(101) - 50
		}
	}

	for i := 1; i < M; i += 2 {
		for j := 0; j < N; j++ {
			if j%2 == 0 && matrix[i][j] >= 0 {
				sum += matrix[i][j]
				result = append(result, matrix[i][j])
			}
		}
	}

	fmt.Printf("\nСумма неотрицательных элементов в чётных строках на нечётных местах: %d\n", sum)
	fmt.Print("Новый массив из этих элементов: ")
	for _, val := range result {
		fmt.Printf("%d ", val)
	}
	fmt.Println()
}
