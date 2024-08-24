package main

import (
	"fmt"
	"io"
	"net/http"
	"os"
	"strings"
	"time"
)

func main() {
	start := time.Now()
	ch    := make(chan string)
	for _, url := range os.Args[1:] {
		go fetch(url, ch)
	}
	for range os.Args[1:] {
		fmt.Println(<-ch)
	}
	fmt.Printf("%.2fs elapsed\n", time.Since(start).Seconds())
}

func fetch(url string, ch chan<-string) {
	start := time.Now()
	if !strings.HasPrefix(url, "https://") {
		url = "https://" + url
	}
	resp, err := http.Get(url)
	if err != nil {
		fmt.Fprintf(os.Stderr, "fetchall: %v\n", err)
		os.Exit(1)
	}
	nBytes, err := io.Copy(io.Discard, resp.Body)
	resp.Body.Close()
	secs := time.Since(start).Seconds()
	ch<-fmt.Sprintf("%.2fs %7d %s\n", secs, nBytes, url)
}
