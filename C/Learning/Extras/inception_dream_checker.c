#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 100
#define MAX_CHAR_NUMBER 6

typedef struct Character {
    char* name;                  // name of char
    int level;                   // level number
    int limbo;                   // 0 if false, else true
    int owner;                   // level number
}Char_t;

typedef struct node {
    Char_t *person;
    struct node *next;
}Link_t;

void create_char(char name[]);
void command_person(char command_person[2][BUFSIZE], char* line, int line_len);
Link_t* get_char(char* person);
void wake(Char_t* person);
void dream(Char_t* person);

Link_t *first_char;
int char_number = 0;

int main( int argc, char** argv ) {

    if (argc == 0) {
        printf("main: no input file specified\n");
        exit(1);
    }

    // file handle
    FILE *fp;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    char ins_person[2][BUFSIZE];
    Link_t* person_addr;

    if((fp = fopen(argv[1], "r")) == NULL) {
        printf("fopen: no file %s\n", argv[1]);
        exit(1);
    }

    while((read = getline(&line, &len, fp)) != -1) {
        command_person(ins_person, line, len);

        if (char_number != 6) {
            int exist = 0; // false
            for (person_addr = first_char; person_addr != NULL; person_addr = person_addr->next) {
                if (strcmp(person_addr->person->name, ins_person[1]) == 0)
                    exist = 1;
            }

            if (exist == 0) {
                create_char(ins_person[1]);
            }
        }

        person_addr = get_char(ins_person[1]);

        if (person_addr->person->limbo != 0)
            continue;

        switch(ins_person[0][0]) {
            case('W'):
                wake(person_addr->person);
                break;
            case('D'):
                dream(person_addr->person);
                break;
            default:
                printf("error: Incorrect command found %s\n", ins_person[0]);
                break;
        }
    }

    Link_t* p;
    for (p = first_char; p != NULL; p = p->next) {
        //if (p->person->limbo != 0) {
            //printf("Name: %s\n", p->person->name);
            //printf("Level: %d\n", p->person->level);
            //printf("Limbo: true\n");
            //printf("Owner: %d\n", p->person->owner);
            //printf("\n");
        //}

        printf("Name: %s\n", p->person->name);
        printf("Level: %d\n", p->person->level);
        printf("Limbo: %d\n", p->person->limbo);
        printf("Owner: %d\n", p->person->owner);
        printf("\n");
    }


    return 0;
}


// Creating and manipulating linked list of persons
Link_t *next_char;

Link_t * link_list_init(void) {
    Link_t *link_init = (Link_t *) malloc(sizeof(Link_t));
    return link_init;
}

void create_char(char* name_char) {
    void *tmp;
    Link_t * p;
    next_char = (char_number <= 6) ? link_list_init(): NULL;


    if (char_number == 0) {
        first_char = next_char;
    }
    else {
        for (p = first_char; p->next != NULL; p = p->next)
            ;
        p-> next = next_char;
    }

    if (char_number > MAX_CHAR_NUMBER && next_char == NULL) {
        printf("create_char: error max character number reached\n");
        return;
    }


    if ((tmp = malloc(sizeof(Char_t))) == NULL) {
        printf("create_char: no memory for new person\n");
        return;
    }

    next_char->person = (Char_t *) tmp;
    int i;
    for (i = 0; name_char[i] != '\0'; i++)
        ;
    next_char->person->name = (char *) malloc(i);
    strncpy((next_char->person->name), name_char, i);
    next_char->person->level = 0;
    next_char->person->limbo = 0;
    next_char->person->owner = -1;

    next_char->next = NULL;
    char_number++;
}


void command_person(char command_person[2][BUFSIZE], char* line, int line_len) {
    int i;
    int j;

    for ( i = 0; i < line_len && line[i] >= 'A' && line[i] <= 'Z'; i++ )
        command_person[0][i] = line[i];
    command_person[0][i] = '\0';

    i = (line[i] == ' ') ? i + 1: i;

    for (j = 0; j < BUFSIZE && i < line_len && line[i] != '\n'; i++, j++ )
        command_person[1][j] = line[i];
    command_person[1][j] = '\0';
}

Link_t* get_char(char* person) {
    Link_t* p;
    for (p = first_char; p != NULL; p = p->next) {
        if (strcmp(p->person->name, person) == 0)
            return p;
    }

    return NULL;
}

// main Wake and Dream functions

Char_t* find(char* field, int value);
void set_limbo(int level);

void wake(Char_t* person) {
    Char_t* tmp;

    if (person->level == 0) {
        printf("wake: error, character %s cannot wake from being awake\n", person->name);
        return;
    }

    if (person->owner != -1) {
        //if ((tmp = find("level", person->owner)) == NULL)
            set_limbo(person->owner);

        person->owner = -1;
    }

    person->level--;
    return;
}

void dream(Char_t* person) {
    int set_owner = 1;

    if (person->owner == person->level) {
        printf("dream: Owner cannot enter deeper dream\n");
        return;
    }

    // check if there is already a person in this level
    for (Link_t* p = first_char; p != NULL; p = p->next)
        if (p->person->level == person->level + 1)
            set_owner = 0;


    person->level++;

    if (set_owner == 1)
        person->owner = person->level;

    return;
}

Char_t* find(char* field, int value) {
    Link_t* p;
    Char_t* cha;

    for (p = first_char; p != NULL; p = p->next) {
        switch(field[0]) {
            case('l'):
                if (p->person->level == value) {
                    cha = p->person;
                    return cha;
                }
                break;
            case('o'):
                if (p->person->owner == value) {
                    cha = p->person;
                    return cha;
                }
                break;
            default:
                printf("find: error incorrect field specified\n");
                exit(1);
        }
    }
    return cha = NULL;
}

void set_limbo(int level) {
    Link_t* p;
    int new_owner = 0; // Looking for a new owner

    for(p = first_char; p != NULL; p = p->next) {
        if (p->person->level > level)
            p->person->limbo = 1;
        else if (p->person->owner == level)
            new_owner = 1;
        else if (p->person->level == level && new_owner == 0)
            p->person->owner = p->person->level;
    }
}
