/* Binarey search improvement and comparison
 *
 * will find index of desired element from
 * orderd list, by searching in an efficient
 * way
 *
 *                                          */
#include <stdio.h>
#include <time.h>

/* BinSearch function with more comparisons
 * INPUTS: --> array of ORDERD data
 *         --> number of elements 'n'
 *         --> element to locate
 *
 * OUTPUT: --> index of element in array
 *                                          */
int binSearchOld(int data[], int n, int element);


/* BinSearch function with less comparisons
 * INPUTS: --> array of ORDERD data
 *         --> number of elements 'n'
 *         --> element to locate
 *
 * OUTPUT: --> index of element in array
 *                                          */
int binSearchNew(int data[], int n, int element);


/* Timer function, will measure execution
 * time taken for funtion passed when run
 * INPUTS: --> function 
 *         --> parameters 
 *
 *  OUTPUT: --> total time taken
 *                                          */
double timer(int (*function)(int *, int, int), int data[], int n, int element);

int main() {
  int element = 4;
  int data[] = {1,2,3,4,5,6,7,8,9,10};
  int n = 10;

  double oldBinExecTime = timer(binSearchOld, data, n, element);
  double newBinExecTime = timer(binSearchNew, data, n, element);

  printf("Original execution time for binary search: %f\n", oldBinExecTime);
  printf("New execution time for binary search: %f\n", newBinExecTime);
  printf("Performance percentage increase of new binary search: %f%%\n", 100*((oldBinExecTime - newBinExecTime)/newBinExecTime));

  return 0;
}


double timer(int (*function)(int *, int, int), int data[], int n, int element) {
  clock_t start, end;
  int index;

  start = clock();

  index = function(data, n, element);

  printf("Data:[");
  for (;n > 0; --n)
    printf("%d,",data[10 - n]);
  printf("\b]\n");
  printf("element:%d\nindex:%d\n", element, index);

  end = clock();

  return ((double)( end - start))/CLOCKS_PER_SEC;
}


int binSearchNew(int data[], int n, int element) {
  int low, mid, high;

  low = 0;
  high = n-1;
  mid = (high + low)/2;

  while (data[mid] != element && low <= high ) {
    mid = (high + low)/2;
    
    if (element < data[mid]) 
      high = mid - 1;
    else 
      low = mid + 1;
    
  }

  if (data[mid] != element)
    return -1;

  return mid;
}




int binSearchOld(int data[], int n, int element) {
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
