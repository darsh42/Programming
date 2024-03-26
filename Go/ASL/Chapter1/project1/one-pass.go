package main

import (
       "os"
       "io/ioutil"
       "fmt"
       "strings"
       "unicode"
)

const (
       LABEL int = 0
       MNEM  int = 1
       NUMB  int = 2
)

type Token struct {
       toktype int
       lexeme  string
       pos     int
       line    int
}

type OP struct {
       mneumonic string
       opcode    int
       operand   bool
}

type SYMBOL struct {
       status int
       name   string
}

var O_TABLE = []OP {
     {"LOD",  1, true},
     {"STO",  2, true},
     {"ADD",  3, true},
     {"BZE",  4, false},
     {"BNE",  5, false},
     {"BRA",  6, false},
     {"INP",  7, true},
     {"OUT",  8, false},
     {"CLA",  9, false},
     {"HLT", 10, false},
}

var S_TABLE = []SYMBOL{}

var LC int = 0

func main() {
       if len(os.Args) < 2 {
              fmt.Fprintf(os.Stderr, "[Usage]: ./one-pass <source.asm>\n")
              return
       }
       filename := os.Args[1]
       bytes, err := ioutil.ReadFile(filename);
       if err != nil {
              panic(err)
       }
       source := string(bytes[:])
       lines := strings.Split(source, "\n")
       fmt.Println("Tokens: {TYPE, LEXEME, POS, LINE}")
       for LC = 0; len(lines[LC]) != 0; LC++ {
              fmt.Println(lines[LC])
              tokens := parseLine(lines[LC])
              printTokens(tokens)
       }
}

func tokType(toktype int) string {
       switch toktype {
       case LABEL: return "LABEL"
       case MNEM:  return "MNEM"
       case NUMB:  return "NUMB"
       }
       return "ERR"
}

func parseLine(line string) []Token {
       tokens := []Token{}
       line    = strings.TrimSpace(line)
       split  := strings.Split(line, " ")
       for i, s := range split {
              switch s {
              case "LOD": tokens = append(tokens, Token{MNEM, s, i, LC})
              case "STO": tokens = append(tokens, Token{MNEM, s, i, LC})
              case "ADD": tokens = append(tokens, Token{MNEM, s, i, LC})
              case "BZE": tokens = append(tokens, Token{MNEM, s, i, LC})
              case "BNE": tokens = append(tokens, Token{MNEM, s, i, LC})
              case "BRA": tokens = append(tokens, Token{MNEM, s, i, LC})
              case "INP": tokens = append(tokens, Token{MNEM, s, i, LC})
              case "OUT": tokens = append(tokens, Token{MNEM, s, i, LC})
              case "CLA": tokens = append(tokens, Token{MNEM, s, i, LC})
              case "HLT": tokens = append(tokens, Token{MNEM, s, i, LC})
              default:
                     if (unicode.IsDigit(rune(s[0]))) {
                            tokens = append(tokens, Token{NUMB, s, i, LC})
                     } else if (unicode.IsLetter(rune(s[0]))) {
                            tokens = append(tokens, Token{LABEL, s, i, LC})
                     }
              }
       }
       return tokens
}

func printTokens(tokens []Token) {
       for _, t := range tokens {
              fmt.Printf("{%s, %s, %d, %d}\n",
                          tokType(t.toktype),
                          t.lexeme,
                          t.pos,
                          t.line)
       }
}

func assemble(tokens []Token) {

}
