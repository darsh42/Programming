#include <stdio.h>

int main() {
  float celsius, fahrenheit;
  int upper, lower, step;

  lower = 0;
  upper = 200;
  step = 20;

  celsius = lower;
  printf("Celsius --> Fahrenheit\n");
  while (celsius < upper) {
    fahrenheit = (9.0 / 5.0) * celsius + 32;
    printf("%7.0f %14.1f\n", celsius, fahrenheit);
    celsius += step;
  }
}
