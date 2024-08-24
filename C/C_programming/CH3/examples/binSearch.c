/* Binarey search implemented in the book
 *
 * will find index of desired element from
 * orderd list, by searching in an efficient
 * way
 *
 *                                          */
#include <stdio.h>

/* BinSearch function
 * INPUTS: --> array of ORDERD data
 *         --> number of elements 'n'
 *         --> element to locate
 *
 * OUTPUT: --> index of element in array
 *                                          */
int binSearch(int data[], int n, int element);

int main() {
  int element = 4;
  int data[] = {1,2,3,4,5,6,7,8,9,10};
  int n = 10;
  
  int index = binSearch(data, n, element);

  printf("Data:[");
  for (;n > 0; --n)
    printf("%d,",data[10 - n]);
  printf("\b]\n");
  printf("element:%d\nindex:%d\n", element, index);

  return 0;
}


int binSearch(int data[], int n, int element) {
  int low, mid, high;

  low = 0;
  high = n-1;

  while (low <= high) {
    mid = (low + high)/2;
    if (element > data[mid])
      low = mid+1;
    else if (element < data[mid])
      high = mid-1;
    else
      return mid;
  }

  return -1;
}
