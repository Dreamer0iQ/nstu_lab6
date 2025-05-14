package main

import (
	"fmt"
	"math/rand"
	"sort"
	"time"
)

type Pair struct {
	Sum   int
	Index int
}

type BySumDesc []Pair

func (a BySumDesc) Len() int           { return len(a) }
func (a BySumDesc) Swap(i, j int)      { a[i], a[j] = a[j], a[i] }
func (a BySumDesc) Less(i, j int) bool { return a[i].Sum > a[j].Sum }

func main() {
	var N int
	fmt.Print("Введите порядок квадратной матрицы (N): ")
	fmt.Scan(&N)

	if N <= 0 {
		fmt.Println("Порядок матрицы должен быть положительным числом.")
		return
	}

	matrix := make([][]int, N)
	sumWithIndex := make([]Pair, N)

	rand.NewSource(time.Now().UnixNano())

	fmt.Println("\nСгенерированная матрица:")
	for i := 0; i < N; i++ {
		matrix[i] = make([]int, N)
		sum := 0
		for j := 0; j < N; j++ {
			matrix[i][j] = rand.Intn(101) + 100
			sum += matrix[i][j]
			fmt.Printf("%d\t", matrix[i][j])
		}
		sumWithIndex[i] = Pair{Sum: sum, Index: i}
		fmt.Printf("(Сумма: %d)\n", sum)
	}

	sort.Sort(BySumDesc(sumWithIndex))

	if N == 1 {
		fmt.Println("\nНедостаточно строк для определения второй максимальной суммы.")
		return
	}

	secondMaxIndex := sumWithIndex[1].Index
	secondMaxRow := matrix[secondMaxIndex]

	fmt.Printf("\nСтрока с второй по величине суммой элементов (%d): ", sumWithIndex[1].Sum)
	for _, val := range secondMaxRow {
		fmt.Printf("%d ", val)
	}
	fmt.Println()
}
