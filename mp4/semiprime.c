#include <stdlib.h>
#include <stdio.h>


/*
 * is_prime: determines whether the provided number is prime or not
 * Input    : a number
 * Return   : 0 if the number is not prime, else 1

 The first error was the is_prime had return 0 at the end of the function which gave the program a blank output.
 The second error was that there was that 1 was being returned when the number was not prime

*/
int is_prime(int number)
{
    int i;
    if (number == 1) {return 0;}
    for (i = 2; i < number; i++) { //for each number smaller than it
        if (number % i == 0) { //check if the remainder is 0
            return 0;
        }
    }
    return 1;
}

/*
 * print_semiprimes: prints all semiprimes in [a,b] (including a, b).
 * Input   : a, b (a should be smaller than or equal to b)
 * Return  : 0 if there is no semiprime in [a,b], else 1
 
 The program was taking the remainder of i/j and seeing if that was prime. since if (i%j == 0) was already in 
 the program before k = i%j k would always be 0 since is_prime(j) doesnt change the value of j 
 this issue was fixed by taking is_prime(i/j) instead of is_prime(k)
 Once that error was fixed the next error was that when a number was semiprime it would print
 to the screen multiple times. This issue was fixed by using the no longer in use variable k.
 for each new i value k is set to 0 and once an i value is found to be semiprime the program sets 
 k to 1 to indicate that the particular i value has already been printed. then k is set to 0 
 once i moves to the next value
*/
int print_semiprimes(int a, int b)
{
    int i, j, k;
    int ret = 0;
    for (i = a; i <=b; i++) { //for each item in interval
        //check if semiprime
        k = 0;
        for (j = 2; j <= i; j++) {
            if (i%j == 0) {
                if (is_prime(j)) {
                    if (is_prime(i/j)) {
                        if (k == 0) {
                            printf("%d ", i);
                            k = 1;
                        }
                    }
                }
            }
        }

    }
    printf("\n");
    return ret;
}
