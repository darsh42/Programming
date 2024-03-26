/* ======================================================== *
 *                 Unscramble C declarations                *
 * ======================================================== *
 *  RULES:                                                  *
 *       read left most IDENTIFIER; -> "IDENTIFIER is"      *
 *       read right of IDENTIFIER;                          *
 *            if '[': then -> "array of"                    *
 *            if '(': then -> "function returning"          *
 *            else continue;                                *
 *       read left of INDENTIFIER;                          *
 *            if '('       : then -> read left and step 2   *
 *            if 'const'   : then -> "read-only"            *
 *            if 'volatile': then -> "volatile"             *
 *            if '*'       : then -> "pointer to"           *
 *            else continue;                                *
 *       read remaining tokens;                             *
 *  STATUS:                                                 *
 *       The program works?                                 *
 *       the program will handle most expressions but       *
 *       not deal with unique structs, enums and typedef    *
 *       additionally the program will not specify the      *
 *       arguments of a function.                           *
 * ======================================================== */
#include <stdio.h>
#include <string.h>
#define MAXTOKENLEN 50
#define MAXTOKENCOUNT 200

struct TOKEN {
    char type;
    char token[MAXTOKENLEN];
};

static int isalphanum(char c) {
    return ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z'));
}

struct TOKEN this;
struct TOKEN stack[MAXTOKENCOUNT];
struct TOKEN *top = stack;

static void STACKpush() {
    (*top) = this;
    top++;
}

static void STACKpop() {
    top--;
}

static const char *TOKENget(const char *s) {
    char *tok = this.token;
    while (*s != ' ' && *s != '\0') {
        if (isalphanum(*s)) {
            while (isalphanum(*s)) {
                *tok++ = *s++;
            }
            *tok = '\0';
            return (*s == ' ') ? ++s: s;
        }
        switch(*s) {
            case '*':
			case '[': case ']':
            case '(': case ')':
                *tok++ = *s++;
                *tok = '\0';
                return (*s == ' ') ? ++s: s;
        }
        s++;
    }
    return ++s;
}

/* Look at current token, this.token */
static void TOKENclassify() {
    int len = strlen(this.token);
    if (len == 5 && !strncmp(this.token, "const", 5))    {this.type = 'q'; return;}
    if (len == 8 && !strncmp(this.token, "volatile", 8)) {this.type = 'q'; return;}
    if (len == 1 && !strncmp(this.token, "*", 1))        {this.type = '*'; return;}
    if (len == 1 && !strncmp(this.token, "(", 1))        {this.type = '('; return;}
    if (len == 1 && !strncmp(this.token, "[", 1))        {this.type = '['; return;}
    if (len == 1 && !strncmp(this.token, "]", 1))        {this.type = ']'; return;}
    if (len == 1 && !strncmp(this.token, ")", 1))        {this.type = ')'; return;}
    if (len == 4 && !strncmp(this.token, "char", 4))     {this.type = 't'; return;}
    if (len == 4 && !strncmp(this.token, "void", 4))     {this.type = 't'; return;}
    if (len == 6 && !strncmp(this.token, "signed", 6))   {this.type = 't'; return;}
    if (len == 8 && !strncmp(this.token, "unsigned", 8)) {this.type = 't'; return;}
    if (len == 5 && !strncmp(this.token, "short", 5))    {this.type = 't'; return;}
    if (len == 3 && !strncmp(this.token, "int", 3))      {this.type = 't'; return;}
    if (len == 4 && !strncmp(this.token, "long", 4))     {this.type = 't'; return;}
    if (len == 5 && !strncmp(this.token, "float", 5))    {this.type = 't'; return;}
    if (len == 6 && !strncmp(this.token, "double", 6))   {this.type = 't'; return;}
    if (len == 6 && !strncmp(this.token, "struct", 6))   {this.type = 't'; return;}
    if (len == 5 && !strncmp(this.token, "union", 5))    {this.type = 't'; return;}
    if (len == 4 && !strncmp(this.token, "enum", 4))     {this.type = 't'; return;}
    this.type = 'i';
}

static const char *IDENTIFIERfind(const char *s, const char *e) {
    for (;s < e;) {
        s = TOKENget(s);
        TOKENclassify();
        if (this.type == 'i') {
            break;
        }
        STACKpush();
    }

    printf("%s is ", this.token);
    return s;
}

static const char *HANDLEfunction_args(const char *s, const char *e) {
    printf("a function returning");
    while (*s != ')' && s++ < e);
    printf(" ");
    return s;
}

static const char *HANDLEarrays(const char *s, const char *e) {
    printf("an array of size 0..");
    while (*s != ']' && s < e) {
        printf("%c", *s++);
    }
    printf(" ");
    return s;
}

static const char *HANDLEpointers(const char *s, const char *e) {
    while (*s == '*' && s++ < e) {
        printf("pointer to ");
    }
    printf(" ");
    return s;
}

static const char *HANDLEdeclarators(const char *s, const char *e) {
    while (s < e) {
        s = TOKENget(s);
        TOKENclassify();
        switch(this.type) {
            case '[': s = HANDLEarrays(s, e); break;
            case '(': s = HANDLEfunction_args(s, e); break;
            case '*': s = HANDLEpointers(s, e); break;
            default:
                if (top != stack) {
                    STACKpop();
                    switch(top->type) {
                        case '*': printf("pointer to "); break;
                        case '(': break;
                        case '[': break;
                        default:
                            printf("%s ", top->token);
                    }
                }
        }
    }
    while (top != stack) {
        STACKpop();
        switch(top->type) {
            case '*': printf("pointer to "); break;
            case '(': break;
            case '[': break;
            default:
                printf("%s ", top->token);
        }
    }
    printf("\n");
    return s;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        fprintf(stderr, "[Usage]: ./unscramble <expression>\n");
        return -1;
    }

    const char *source = *(++argv);
    const char *end    = source + strlen(source);
    source = IDENTIFIERfind(source, end);
    HANDLEdeclarators(source, end);
}
