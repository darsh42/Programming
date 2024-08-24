#include "lexer.h"
#include "optable.h"

extern OP TABLE[];

struct {
    int cur;
    int line;
    Token *tokens;
    int token_count;
} lexer = {0, 0, NULL, 0};

static int isNumeric(char *str) {
    int i;
    for (i = 0; str[i] >= '0' && str[i] <= '9'; i++)
        ;
    return i;
}

static int isAlphabetic(char *str) {
    int i;
    for (i = 0; (str[i] >= 'A' && str[i] <= 'Z') ||
                (str[i] >= 'a' && str[i] <= 'z'); i++)
        ;
    return i;
}

static int nstrint(char *str, int n) {
    int total = 0;
    for (int i = 0; i < n; i++) {
        total *= 10;
        total += str[i] - '0';
    }
    return total;
}

static int nstrcmp(char *str, char *cmp, int n) {
    int total = 0;
    for (int i = 0; i < n; i++) {
        total += str[i] - cmp[i];
    }
    return total;
}

static void nstrcpy(char *src, char *dest, int n) {
    int i;
    for (i = 0; i < n; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

static char *tokenType(enum TYPE type) {
    switch(type) {
        case (SPACE):      return "SPACE  ";
        case (COLON):      return "COLON  ";
        case (NUMBER):     return "NUMBER ";
        case (MNEM):       return "MNEM   ";
        case (LABEL):      return "LABEL  ";
        case (END):        return "END    ";
        default:           return "ERR    ";
    }
}

static void addToken(enum TYPE type, char *lexeme, int lexeme_len, int literal, int line) {
    lexer.tokens = realloc(lexer.tokens, ++lexer.token_count * sizeof(Token));
    if (lexer.tokens == NULL) {
        fprintf(stderr, "[Error] cannot allocate space for token\n");
        return;
    }
    lexer.tokens[lexer.token_count - 1] = (Token) {type, lexeme, lexeme_len, literal, line};
}

/* LABEL SOURCE OBJECT */
void lex(char *source, int source_len) {
    char *c;
    while (lexer.cur < source_len) {
        // check for single character symbols
        c = &source[lexer.cur++];
        switch(*c) {
            case  ',': addToken(COMMA, c, 1, -1, lexer.cur); continue;
            case '\n': lexer.line++; continue;
            case  ' ': continue;
            case  ':': addToken(COLON, c, 1, -1, lexer.cur); continue;
        }
        int len;
        switch (len = isAlphabetic(c)) {
            case 1:
                // check if it is label
                addToken(LABEL, c, 1, -1, lexer.cur);
                continue;
            case 3: {
                // check each instruction
                int res;
                for (int i = 0; (res = nstrcmp(c, TABLE[i].mnem, len)) != 0 &&
                                i < N_OPCODES; i++)
                    ;
                if (res == 0) {
                    addToken(MNEM, c, 3, -1, lexer.cur);
                    lexer.cur += 3;
                    continue;
                }
                break;
            }
        }
        // check if it is a number
        if ((len = isNumeric(c)) != 0) {
            int n = nstrint(c, len);
            addToken(NUMBER, c, len, n, lexer.cur);
            lexer.cur += len;
        }
    }
}

void printTokens() {
    Token tok;
    char lexeme[5], *type;
    int line;
    for (int i = 0; i < lexer.token_count; i++) {
        tok = lexer.tokens[i];
        type = tokenType(tok.type);
        line = tok.line;
        nstrcpy(tok.lexeme, lexeme, tok.lexeme_len);
        printf("{%s %s %d}\n", type, lexeme, line);
    }
}
