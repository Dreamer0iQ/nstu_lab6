package main

import (
	"fmt"
	"math"
)

const (
	N   = 4
	EPS = 1e-3
)

type Vec []float64
type Mat [][]float64

func printSystem(A Mat, b Vec) {
	for i := 0; i < N; i++ {
		for j := 0; j < N; j++ {
			fmt.Printf("%10.4f ", A[i][j])
		}
		fmt.Printf(" = %10.4f\n", b[i])
	}
	fmt.Println()
}

// Gaussian elimination with partial pivoting
func gaussMain(A Mat, b Vec) Vec {
	for i := 0; i < N; i++ {
		maxRow := i
		for k := i + 1; k < N; k++ {
			if math.Abs(A[k][i]) > math.Abs(A[maxRow][i]) {
				maxRow = k
			}
		}
		A[i], A[maxRow] = A[maxRow], A[i]
		b[i], b[maxRow] = b[maxRow], b[i]

		for k := i + 1; k < N; k++ {
			factor := A[k][i] / A[i][i]
			for j := i; j < N; j++ {
				A[k][j] -= factor * A[i][j]
			}
			b[k] -= factor * b[i]
		}
	}

	x := make(Vec, N)
	for i := N - 1; i >= 0; i-- {
		x[i] = b[i]
		for j := i + 1; j < N; j++ {
			x[i] -= A[i][j] * x[j]
		}
		x[i] /= A[i][i]
	}
	return x
}

// Simple iteration method
func simpleIteration(A Mat, b Vec) Vec {
	x := make(Vec, N)
	prev := make(Vec, N)
	C := make(Mat, N)
	for i := range C {
		C[i] = make(Vec, N)
	}
	f := make(Vec, N)

	for i := 0; i < N; i++ {
		f[i] = b[i] / A[i][i]
		for j := 0; j < N; j++ {
			if i != j {
				C[i][j] = -A[i][j] / A[i][i]
			}
		}
	}

	fmt.Println("\nIterations of simple iteration method (ε = 1e-3):")
	fmt.Print("Iter\t")
	for i := 0; i < N; i++ {
		fmt.Printf("x%d\t\t", i+1)
	}
	fmt.Println("εn")

	iter := 0
	for {
		copy(prev, x)
		maxDiff := 0.0

		for i := 0; i < N; i++ {
			x[i] = f[i]
			for j := 0; j < N; j++ {
				x[i] += C[i][j] * prev[j]
			}
			maxDiff = math.Max(maxDiff, math.Abs(x[i]-prev[i]))
		}

		fmt.Printf("%d\t", iter)
		for _, xi := range x {
			fmt.Printf("%.6f\t", xi)
		}
		fmt.Printf("%.6f\n", maxDiff)

		if maxDiff < EPS {
			break
		}
		iter++
	}

	return x
}

func main() {
	A := Mat{
		{0.25, -1.23, -0.16, -0.09},
		{-0.21, -0.16, 0.8, -0.13},
		{0.15, -1.31, 0.06, 0.84},
		{1.09, -0.04, 0.21, -18},
	}

	b := Vec{
		0.84,
		2.56,
		1.09,
		-1.24,
	}

	fmt.Println("Original system:")
	printSystem(A, b)

	fmt.Println("Solution using Gaussian elimination with partial pivoting:")
	gaussResult := gaussMain(A, b)
	for i := 0; i < N; i++ {
		fmt.Printf("x%d = %.6f\n", i+1, gaussResult[i])
	}

	iterResult := simpleIteration(A, b)
	_ = iterResult // Use result to avoid unused variable warning

	// Alternative systems (commented out as in original)
	/*
		A = Mat{
			{0.0, 0.0367, -0.1927, 16.5138},
			{-0.2033, 0.0, 0.1301, 0.0732},
			{0.2625, 0.2, 0.0, 0.1625},
			{-0.1786, 1.5595, -0.0714, 0.0},
		}

		b = Vec{
			-1.1376,
			-0.6829,
			3.2,
			1.2976,
		}

		A = Mat{
			{4.12, -0.43, 0.54, 0.11},
			{0.32, 5.22, -0.17, 0.25},
			{-0.12, 0.34, 3.76, -0.18},
			{0.05, -0.12, 0.23, 5.11},
		}

		b = Vec{
			1.54,
			-2.12,
			3.17,
			-0.98,
		}
	*/
}
