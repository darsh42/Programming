package main

import (
	"fmt"
	"bufio"
	"os"
)

func main() {
	count := make(map[string]int)
	input := bufio.NewScanner(os.Stdin)
	for input.Scan() {
		count[input.Text()]++
	}
	for line, n := range count {
		if n > 1 {
			fmt.Printf("%d %s\n", n, line)
		}
	}
}

