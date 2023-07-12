#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STACKNUM 9
#define LINEBUF 50

char *strrev(char *str);

int main(int argc, char* argv[]) {

    if (argc == 1) {printf("specify input\n"); return -1;}

    char stacks[STACKNUM][LINEBUF];
    for (int i = 0; i < STACKNUM; i++)
        strncpy(stacks[i], "", 1);

    char line[LINEBUF];
    FILE *fp = fopen(argv[1], "r");

    while (fgets(line, LINEBUF, fp) != NULL) {
        size_t len = strlen(line);
        for (size_t i = 0, stack = 0; i < len; i += 4, stack++)
            if (line[i] == '[')
                strncat(stacks[stack], &line[i+1], 1);
            else if (line[0] != '[')
                goto make_stacks;

    }

    make_stacks:
    for (int i = 0; i < 9; i++)
        strncpy(stacks[i], strrev(stacks[i]), sizeof(stacks[i]));

    while (fgets(line, LINEBUF, fp) != NULL) {
        if (line[0] != 'm')
            continue;

        size_t len = strlen(line);

        char tmp[LINEBUF];

        // TODO: move the chars one by one to other stacks
        // copy, reverse, concatonate

        // {element number, source, target}
        int cmd[3];
        for (size_t i = 0, j = 0; i < len; i++) {
            if (line[i] >= '1' && line[i] <= '9') {
                cmd[j++] = atoi(&line[i]);
            }
        }
        //printf("CMD: num -> %d, from -> %d, to -> %d", cmd[0], cmd[1], cmd[2] );

        strncpy(tmp, &stacks[cmd[1] - 1][strlen(stacks[cmd[1]-1]) - cmd[0] - 1], cmd[0] + 1);
        stacks[cmd[1]-1][strlen(stacks[cmd[1]-1]) - cmd[0] - 1] = '\0';
        strncat(stacks[cmd[2]-1], strrev(tmp), strlen(tmp));

    }

    for (int i = 0; i < STACKNUM; i++) {
        printf("%s\n", stacks[i]);
    }

    return 0;
}

char *strrev(char *str) {
    if (!str || ! *str)
        return str;

    int i = strlen(str) - 1, j = 0;

    char ch;
    while (i > j)
    {
        ch = str[i];
        str[i] = str[j];
        str[j] = ch;
        i--;
        j++;
    }
    return str;
}
