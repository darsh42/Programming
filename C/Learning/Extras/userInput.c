#include <stdio.h>

int main(void) {
  // Create int and char vars
  int myNum;
  char myChar[55];

  // Ask user to type num and char
  printf("Type number and character and press enter: \n");

  // Get and save number and charcter
  scanf("%d %s", &myNum, &*myChar);

  // Print the number
  printf("Your number is: %d\n", myNum);

  // Print the character
  printf("Your character is: %s\n", myChar);
}
