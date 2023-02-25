/* Code for the stack data structure
 *
 * PARTS:
 *  - print
 *  - push
 *  - pop
 *
 *                                  */
#include <stdio.h>
#define STACKSIZE 1000
#define STACKINIT -1

/* Will push data onto the stack 
 * INPUTS: --> Stack 
 *         --> stackTop
 *         --> data             
 *
 * OUTPUT: --> adds data to stack  
 *         --> int RETURN status  
 *            |--> 0 normal return
 *            |--> 1 error return*/
int stackPush(int stack[STACKSIZE], int data);


/* Will pop(return top value) from stack
 * INPUTS: --> STACK 
 *         --> stackTop
 *
 * OUTPUT: --> top value in stack   */
int stackPop(int stack[STACKSIZE]);


/* Will display contents in the stack
 * INPUTS: --> STACK
 *         --> stcakTop
 *
 * OUTPUT: --> print stack          */
void stackView(int stack[STACKSIZE]);


int stackTop;


int main(){
  int i;
  int stack[STACKSIZE];
  int data[] = {1, 2, 3, 4 ,5};


  stackTop = STACKINIT; 
  // check if stackTop is out of bounds
  if ((stackTop >= STACKSIZE) ^ (stackTop < -1)) {
    printf("Error: stackTop not in range!");
    return 1;
  }
  
  for (i = 0; i < 5; ++i)
    stackTop = stackPush(stack, data[i]);

  printf("original:\n");
  stackView(stack);
  printf("\n");

  for (i = 0; i < 2; ++i)
    stackPop(stack);
  
  printf("Popped stack:\n");
  stackView(stack);

  return 0;
}

/*=================FUNCTIONS====================*/


int stackPush(int stack[STACKSIZE], int data) {
  // under normal conditions
  ++stackTop;
  stack[stackTop] = data;
  return stackTop;
}


int stackPop(int stack[STACKSIZE]) {
  int out;

  out = stack[stackTop];
  stack[stackTop] = 0;
  --stackTop;
  return out;
}


void stackView(int stack[STACKSIZE]) {
  int i;

  for (i = 0; i < stackTop + 1; ++i) {
    if (i == stackTop) {
      printf("%d <-- stackTop\n", stack[i]);
      continue;
    }
    printf("%d\n", stack[i]);
  }
}
