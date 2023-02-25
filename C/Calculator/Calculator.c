#include <stdio.h>

int main(int argc, char *argv[])
{
  float valueOne;
  float valueTwo;
  float answer;
  char operator;

  printf("Enter Calculation:\n\n");
  scanf("%f %c %f", &valueOne, &operator, &valueTwo);

  switch(operator)
   {
    case '+': answer = valueOne + valueTwo;
      break;
    case '-': answer = valueOne - valueTwo;
      break;
    case '*': answer = valueOne * valueTwo;
      break;
    case '/': answer = valueOne / valueTwo;
      break;
    default: goto fail;
     }
   printf("%f %c %f = %f\n", valueOne, operator, valueTwo, answer);
   goto exit;
  fail:
   printf("FAIL\n");
  exit:
   return 0;
}
