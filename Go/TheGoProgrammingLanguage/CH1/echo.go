package main

import (
	"fmt"
	"os"
)

func main() {
	echo1()
	echo2()
}

func echo1() {
	var s, sep string
	for i := 1; i < len(os.Args); i++ {
		s += sep + os.Args[i]
		sep = " "
	}
	fmt.Println(s)
}

func echo2() {
	var s, sep string 
	for _, arg := range os.Args[1:] {
		s = sep + arg
		sep = " "
	}
	fmt.Println(s)
}

