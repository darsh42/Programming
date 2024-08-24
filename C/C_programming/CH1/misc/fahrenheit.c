#include <stdio.h>

/* Print Fahrenheit-to-celsius table
 * for Fah = 0, 20, ..., 300 */

#define LOWER 0 /*Lower limit*/
#define UPPER 300 /*upper limit*/
#define STEP 20 /*step size*/


int main() {
  int fahr;
  for (fahr = LOWER; fahr < UPPER; fahr += STEP)
      printf("%3d %6.1f\n", fahr, (5.0/9.0) * (fahr - 32));
}






/*
int main() {
  float fahr, cels;
  int lower, upper, step;

  lower = 0;        lower limit of the temp table
  upper = 300;      upper limit of the temp table
  step = 20;        step between each iteration

  fahr = lower;
  printf("Farenheit --> Celcius\n");
  while (fahr < upper) {
    cels = 5 * (fahr - 32) / 9;
    printf("%9.0f %11.1f\n", fahr, cels);
    fahr += step;
  }
}
*/
