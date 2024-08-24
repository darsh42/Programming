#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXTOKEN 50
#define NUMMATCHES 5
#define MAXTOKENLEN 50

#define ISNUMER(c) (c >= '0' && c <= '9')
#define ISALPHA(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
#define ISALPHANUMER(c) (ISNUMER(c) || ISALPHA(c))

#define TOKENPRINT(t) printf("TOK: %s, %d\n", t->string, t->type)
#define STACKPRINT    for (struct TOKEN *t = stack; t < top; t++) TOKENPRINT(t)

#define PUSH (*top) = this; top++
#define POP   top--

enum type {
   OPBR = 0,
   OPSQ = 1,
   STAR = 2,
   TYPE = 3,
   IDEN = 4,
   QUAL = 5,
   CLBR = 6,
   CLSQ = 7,
};

struct TOKEN {
    char string[MAXTOKEN];
    enum type type;
};

const char *s;
const char *e;
struct TOKEN this;
struct TOKEN stack[MAXTOKEN];
struct TOKEN *top = stack;

static void classtok() {
    int len = strlen(this.string);
    if (len == 5 && !strncmp(this.string, "const", 5))    {this.type = QUAL; return;}
    if (len == 8 && !strncmp(this.string, "volatile", 8)) {this.type = QUAL; return;}
    if (len == 1 && !strncmp(this.string, "*", 1))        {this.type = STAR; return;}
    if (len == 1 && !strncmp(this.string, "(", 1))        {this.type = OPBR; return;}
    if (len == 1 && !strncmp(this.string, "[", 1))        {this.type = OPSQ; return;}
    if (len == 1 && !strncmp(this.string, "]", 1))        {this.type = CLSQ; return;}
    if (len == 1 && !strncmp(this.string, ")", 1))        {this.type = CLBR; return;}
    if (len == 4 && !strncmp(this.string, "char", 4))     {this.type = TYPE; return;}
    if (len == 4 && !strncmp(this.string, "void", 4))     {this.type = TYPE; return;}
    if (len == 6 && !strncmp(this.string, "signed", 6))   {this.type = TYPE; return;}
    if (len == 8 && !strncmp(this.string, "unsigned", 8)) {this.type = TYPE; return;}
    if (len == 5 && !strncmp(this.string, "short", 5))    {this.type = TYPE; return;}
    if (len == 3 && !strncmp(this.string, "int", 3))      {this.type = TYPE; return;}
    if (len == 4 && !strncmp(this.string, "long", 4))     {this.type = TYPE; return;}
    if (len == 5 && !strncmp(this.string, "float", 5))    {this.type = TYPE; return;}
    if (len == 6 && !strncmp(this.string, "double", 6))   {this.type = TYPE; return;}
    if (len == 6 && !strncmp(this.string, "struct", 6))   {this.type = TYPE; return;}
    if (len == 5 && !strncmp(this.string, "union", 5))    {this.type = TYPE; return;}
    if (len == 4 && !strncmp(this.string, "enum", 4))     {this.type = TYPE; return;}
    this.type = IDEN;
}

static void gettok(void) {
    char *tok = this.string;
    while (*s != ' ' && s < e) {
        if (ISALPHANUMER(*s)) {
            while(ISALPHANUMER(*s)) {
                *tok++ = *s++;
            }
            *tok = '\0';
            goto complete;
        }
        switch(*s) {
            case '*':
			case '[': case ']':
            case '(': case ')':
                *tok++ = *s++;
                *tok = '\0';
                goto complete;
        }
        s++;
    }
complete:
    if (*s == ' ') {
        s++;
    }
    classtok();
}

void func(void) {
    printf("a function returning ");
    gettok();
    while(this.type != CLBR && s < e) {
        printf("%s ", this.string);
        gettok();
    }
}

void array(void) {
    printf("an array of size 0..");
    while(this.type != CLSQ && s < e) {
        gettok();
        printf("%s", this.string);
    }
    printf(" ");
}

void pointer(void) {
    do {
        printf("pointer to ");
        gettok();
    } while(this.type == STAR && s < e);
}

void type(void) {
    printf("type %s ", top->string);
}

void qualifier(void) {
    printf("%s ", top->string);
}

void unused(void) {
}

void (*routines[8])() = {func, array, pointer, type, unused, qualifier, unused, unused};

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "[Usage]: ./FSM <expression>\n");
        return -1;
	}

    s = *++argv;
	e = s + strlen(s);

    while (this.type != IDEN && s < e) {
		gettok();
        PUSH;
	}
    
    printf("%s is a ", this.string);

    if (s <= e) {
        gettok();
        routines[this.type]();
        while(top != stack) {
            POP;
            routines[top->type]();
        }
    }

    printf("\n");
	return 0;
}
