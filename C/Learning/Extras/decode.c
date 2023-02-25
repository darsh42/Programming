/*
** -- Match PHRASE in CODED and return start index of sub-string
** -- Compare PHRASE with CODED sub-string and generate a key
** -- use key to decode the full message
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* coded = "ETEVHTWGSAHGWYVPNKQOEGWYVPNKPDEPHWAOVWPFWNHANEVWXAVOAEAJEUXTAOWBTEVHTWGSAHGWYVPNQAOQVGTYHAVAXETOANFQEOIQPLANTEVHFYNSQVEBEOWSKNCKLOPEVTYJAUFWYNCOTWZESQEPERQSQOPEVYCEVHEGDEHEVHEYOPNQEEHWYFTKTEVHTWGSAHGWYVPNKQOWVAPDEPWVTKFWNHANOTEVHTWGSAHGWYVPNQAOPDANAENAWVTKPIWHWYFTKTEVHTWGSAHGWYVPNQAOQVPDAIWNTHWVAWBPDAUQOYLFASQOPEVIDEPQOPDAWPDANWVA";
char* phrase = "NEPALSERBIASWITZERLANDBURKINAFASOKYRGYZSTANLUXEMBOURGSLOVAKIATAJIKISTANUGANDACHADANDAUSTRIA";

/*
** Function to find start index of sub-String
** RETURN:
** --SUCCESS: index
** --FAILURE: -1
*/
int findIndex(int* indexes, int coded_len, int phrase_len);

/*
** Will generate the key from subStringIndex
** RETURN:
** --SUCCESS: Char* key
** --FAILURE: Char* null
*/
void keyGen(char* trueKeys, int* subStrIndex, int top, int phrase_len);

int main( int argc, char **argv ) {
    int coded_len = strlen(coded);
    int phrase_len = strlen(phrase);
    int startIndex[100];
    int top = findIndex(startIndex, coded_len, phrase_len);
    char trueKeys[27];

    for (int i = 0; i < top; i++)
        printf("%d\n", startIndex[i]);

    keyGen(trueKeys, startIndex, top, phrase_len);
    printf("%s\n", trueKeys);
    for (int i = 0; i < coded_len; i++)
        printf("%c", trueKeys[coded[i] - 'A']);

    printf("\n");

    return 0;
}

int findIndex(int* indexes, int coded_len, int phrase_len) {
    int i, j;
    int top = 0;

    for (j = 0; j < (int) (coded_len - phrase_len); j++) {              // Changes start index in coded message
        for (i = 0; i < phrase_len && (coded[j + i] != phrase[i]); i++) // loops til end or til similair char found
            ;

        if (coded[j + i] != phrase[i])                                  // when loop ends checks if char not simiair
            indexes[top++] = j;                                         // if true returns start index
    }

    return top;
}

void keyGen(char* trueKeys, int* subStrIndex, int top, int phrase_len) {
    int min_spaces = 100;
    int keyInx;
    char keys[top][27];
    memset(keys, ' ', sizeof(keys));

    for(int i = 0; i < top; i++) {

        for (int j = subStrIndex[i]; j < (phrase_len + subStrIndex[i]); j++) {

            if (keys[i][coded[j] - 'A'] == ' ' )
                keys[i][coded[j] - 'A'] = phrase[j - subStrIndex[i]];

            if (keys[i][coded[j] - 'A'] != ' ' && keys[i][coded[j] - 'A'] != phrase[j - subStrIndex[i]]) {
                break;
            }

        }

        keys[i][26] = '\0';
    }

    for (int i = 0; i < top; i++) {
        int spaces = 0;

        for (int j = 0; j < 27; j++)
            spaces += (keys[i][j] == ' ') ? 1: 0;

        if (spaces < min_spaces) {
            min_spaces = spaces;
            keyInx = i;
        }
    }
    strcpy(trueKeys, keys[keyInx]);
}
