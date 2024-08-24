#include "MIPS.h"

enum {ZERO, AS, V0, V1, A0, A1, A2, A3, T0, T1, T2, T3, T4, T5, T6, T7, S0, S1, S2, S3, S4, S5, S6, S7, T8, T9, K0, K1, GP, SP, S8, RA} registers;

static char *line = NULL;
static int line_size = 0;

bool advance(int *i) {
    if (*i >= line_size || line[*i] == '\n' || line[*i] == ';' || line[*i] == EOF)
        return false;
    else return true;
}

/*
** TOKENIZER(char *current_source_line, int line_size) -> Array of tokens
*/
Token_t *tokenizer(char *curline, int curline_size) {
    line = curline;
    line_size = curline_size;
    Token_t *tokens = malloc(sizeof(Token_t) * DEFAULT_TOKEN_COUNT);

    for (int i = 0, tmp = 0, token_num = 0; advance(&i) ; i++) {
        char c;

        switch((c = line[i])) {
            case('.'): {// Directive
                Token_t token;
                int directive_size;

                token.token_type = Tdirective;

                for(directive_size = 0; advance(&i) && line[i] != ','; i++, directive_size++)
                    ;

                token.token_str = malloc(sizeof(char) * directive_size);

                for (int j = 0; j < directive_size; j++)
                    token.token_str[j] = line[i - directive_size + j];

                tokens[token_num++] = token;
                break;
            }
            case('$'): {// Register
                Token_t token;

                token.token_type = Tregister;
                token.token_str = malloc(sizeof(char) * 5);

                for(int j = 0; advance(&i) && line[i] != ','; i++, j++)
                    token.token_str[j] = line[i];

                tokens[token_num++] = token;
                break;
            }
            case(' '): // Separator
            case(','): // Separator
            case('('): // Separator
            case(')'): // Separator
                break;

            default: {// mneumonic, label, symbol and immediate check
                Token_t token;
                int token_size;

                tmp = i;
                // get the token str
                for (token_size = 0; advance(&i) && line[i] != ',' && line[i] != ':'; i++, token_size++)
                    ;

                token.token_str = malloc(sizeof(char) * token_size);

                for (i = tmp; advance(&i) && line[i] != ','; i++)
                    token.token_str[i - tmp] = line[i];

                // mneumonic found
                for (int opnum = 0; opnum < OPCOUNT; opnum++)
                    if (!strcmp(token.token_str, optable[opnum].mneumonic))
                        token.token_type = Tmneumonic;

                // immediate value
                if (c >= '0' && c <= '9')
                    token.token_type = Timmediate;

                // label found
                if (line[i] == ':') {
                    // Do symbol table look up

                }

                tokens[token_num++] = token;
                break;
            }
        }
    }

    return tokens;
}
