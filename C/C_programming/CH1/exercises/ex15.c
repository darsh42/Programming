#include <stdio.h>

float fahrenheit_to_celsius(int fahrenheit_temp);
float celsius_to_fahrenheit(int celsius_temp);

float fahrenheit_to_celsius(int faren) {
  return (5.0 / 9.0) * (faren - 32);
}


float celsius_to_fahrenheit(int cels) {
  return (9.0 / 5.0) * cels + 32;
}

int main() {
  int i;

  printf("Fahrenheit --> Celsius\n");
  for (i = 0; i < 300; i += 20)
    printf("%10.0d %11.2f\n", i, fahrenheit_to_celsius(i));

  printf("Celsius --> Fahrenheit\n");
  for (i = 0; i < 300; i += 20)
    printf("%7.0d %14.2f\n", i, celsius_to_fahrenheit(i));

}
