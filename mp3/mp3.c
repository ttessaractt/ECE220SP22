#include <stdio.h>
#include <stdlib.h>

/*
this program uses k loop to run the program from i=1 to k and then 
calculate the coefficient.   
*/
int main()
{
  int row;

  printf("Enter a row index: ");
  scanf("%d",&row);

  // Write your code here
  // k loop
  for (int k = 0; k < row+1; k++) {
    // initialize number to be printed
    unsigned long x = 1;   
    // loop to calculate coefficient
    for (int i = 1; i <= k; i++) {
      // caclulate coefficient using equation
      x = x * (row + 1 - i)/i;
    }
    // print coefficient
    printf("%lu ",x);
  }
  return 0;
}
