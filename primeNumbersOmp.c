#include <stdio.h>
#include <math.h>
int main ()
{

int lower, upper,total,i,j;
int isprime;

total = 0;
lower = 2;
upper = 131072;

/*each thread has its own private copis of i, j, and isprime
 Modification made on them are not visible to other threads
So, each thread sees only the part of iterations i.e.
the numbers  assigned to it to test the primality which will 
modifies its own copy of isprime  
*/

#pragma omp parallel shared (upper) private (i,j,isprime)

/*
Here, each thread calculate their own private copy of the total
The partial totals from each thread are combined (summation) on exit
*/

#pragma omp for reduction (+ : total)

for (i = lower; i<= upper; i++)
{
  isprime = 1;

     for ( j=2; j < i; j++)
       {
        if (i % j == 0)
         {
           isprime = 0;
           break;
         }
       }       
       
    total+=isprime;     
}
   printf("Number of Prime Numbers from %d to %d=%d\n",lower,upper,total);

}
