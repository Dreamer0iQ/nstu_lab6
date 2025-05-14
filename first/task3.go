package main

import (
	"fmt"
	"math/rand"
	"os"
	"os/exec"
	"time"
)

const (
	WIDTH  = 100
	HEIGHT = 10
	ALIVE  = '@'
	DEAD   = ' '
)

type Grid [][]bool

func printGrid(grid Grid) {
	cmd := exec.Command("clear")
	cmd.Stdout = os.Stdout
	cmd.Run()
	for y := 0; y < HEIGHT; y++ {
		for x := 0; x < WIDTH; x++ {
			if grid[y][x] {
				fmt.Printf("%c", ALIVE)
			} else {
				fmt.Printf("%c", DEAD)
			}
		}
		fmt.Println()
	}
}

func countNeighbors(grid Grid, x, y int) int {
	count := 0
	for dy := -1; dy <= 1; dy++ {
		for dx := -1; dx <= 1; dx++ {
			if dx == 0 && dy == 0 {
				continue
			}
			nx, ny := x+dx, y+dy
			if nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT {
				if grid[ny][nx] {
					count++
				}
			}
		}
	}
	return count
}

func nextGeneration(current Grid) Grid {
	next := make(Grid, HEIGHT)
	for y := range next {
		next[y] = make([]bool, WIDTH)
		copy(next[y], current[y])
	}

	for y := 0; y < HEIGHT; y++ {
		for x := 0; x < WIDTH; x++ {
			neighbors := countNeighbors(current, x, y)
			if current[y][x] {
				next[y][x] = neighbors == 2 || neighbors == 3
			} else {
				next[y][x] = neighbors == 3
			}
		}
	}
	return next
}

func randomInit(grid Grid, density int) {
	for y := 0; y < HEIGHT; y++ {
		for x := 0; x < WIDTH; x++ {
			grid[y][x] = rand.Intn(100) < density
		}
	}
}

func main() {
	rand.Seed(time.Now().UnixNano())
	grid := make(Grid, HEIGHT)
	for y := range grid {
		grid[y] = make([]bool, WIDTH)
	}
	randomInit(grid, 60)

	for {
		printGrid(grid)
		grid = nextGeneration(grid)
		time.Sleep(100 * time.Millisecond)
	}
}
