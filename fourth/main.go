package main

import (
	"fmt"
)

func main() {
	var n, m, t int64
	fmt.Scan(&n, &m, &t)

	max_k := min((n-1)/2, (m-1)/2) // плитки дорожки
	low, high := int64(0), max_k
	best_k := int64(0)

	for low <= high {
		mid := low + (high-low)/2
		required := 2 * mid * (n + m - 2*mid)

		if required <= t {
			best_k = mid
			low = mid + 1
		} else {
			high = mid - 1
		}
	}

	fmt.Println(best_k)
}

func min(a, b int64) int64 {
	if a < b {
		return a
	}
	return b
}
