/* Program that when a string is copied all escape sequences are visible */

#include <stdio.h>
#define MAXLEN 100

/* function will replace all the escape sequences in string 't' when it is 
 * copied into  string 's'
 *
 * INPUTS: --> string s, will contain escape sequences
 *         --> string t, original string
 *
 * OUTPUT: --> change string s to contain escape sequences              
 *                                                                      */
void escape(char s[], char t[]);

int main() {
  char t[] = "Hello\t\bmy Name is \\Darsh\n";
  char string[100];
  
  escape(string, t);

  return 0;
}


void escape(char s[], char t[]) {
  int i;
  int index;

  for (i = index = 0; t[i] != '\0'; i++, index++) {
    switch(t[i]) {
      case('\n'):
        s[index++] = '\\';
        s[index] = 'n';
        break;
      case('\t'):
        s[index++] = '\\';
        s[index] = 't';
        break;
      case('\a'):
        s[index++] = '\\';
        s[index] = 'a';
        break;
      case('\b'):
        s[index++] = '\\';
        s[index] = 'b';
        break;
      case('\f'):
        s[index++] = '\\';
        s[index] = 'b';
        break;
      case('\r'):
        s[index++] = '\\';
        s[index] = 'r';
        break;
      case('\v'):
        s[index++] = '\\';
        s[index] = 'v';
        break;
      case('\''):
        s[index++] = '\\';
        s[index] = '\'';
        break;
      case('\"'):
        s[index++] = '\\';
        s[index] = '"';
        break;
      case('\?'):
        s[index++] = '\\';
        s[index] = '?';
        break;
      case('\\'):
        s[index++] = '\\';
        s[index] = '\\';
        break;
      default:
        s[index] = t[i];
        break;
    }
  }

  printf("Original string: %s\n", t);
  printf("New string: %s\n", s);
}
