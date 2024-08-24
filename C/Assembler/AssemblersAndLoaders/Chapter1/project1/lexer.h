#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

enum TYPE {
  // single char tokens
  COMMA,
  NEWLINE,
  SPACE,
  COLON,
  // Literals
  NUMBER,
  STRING,
  IDENTIFIER,
  // KEYWORDS
  MNEM,
  ADDRESS,
  LABEL,
  DIRECTIVE,

  END
};

typedef struct Token {
    enum TYPE type;
    char     *lexeme;
    int       lexeme_len;
    int       literal;
    int       line;
} Token;


#endif // LEXER_H_INCLUDED
