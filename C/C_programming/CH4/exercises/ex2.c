#include <stdio.h>
#include <ctype.h>

/* Will convert a string with decimals into a double
   It will also convert scientific notation into doubles
   INPUT: --> string[]

   OUTPUT: --> double coverted string
                                                           */
double atof(char string[]);

int main( int argc, char **argv ) {
  printf("Input: %s\nOutput: %f\n", argv[1], atof(argv[1]));
  return 0;
}

double atof( char string[] ) {
  double val;
  int i, sign, power, exp_sign, exp_num, exp;

  for ( i = 0; isspace(string[i]); i++ )
    ;

 sign = (string[i] < 0) ? -1: 1;

 if (string[i] == '+' || string[i] == '-')
   i++;

 for ( val = 0.0; string[i] != '.' && string[i] != 'e' && string[i] != 'E' && string[i] != '\0'; i++ )
   val = 10.0 * val + (string[i] - '0');

 if ( string[i] == '.' ) {
   i++;
   for ( power = 0; string[i] != 'e' && string[i] != 'E' && string[i] != '\0'; i++ ) {
     val = 10 * val + (string[i] - '0');
     power *= 10;
   }
 }

 if (string[i] == 'e' || string[i] == 'E') {
   i++;
   exp_sign = (string[i] == '-') ? -1: 1;

   if (string[i] == '-' || string[i] == '+')
     i++;

   for (exp_num = 0; string[i] != '\0'; i++)
     exp_num = 10 * exp_num;

   for (int j = 0; j < exp_num; j++)
     exp *= 10;
 }

 return (exp_sign < 0) ? sign * val/(power * exp): sign * val/(power / exp);

}
