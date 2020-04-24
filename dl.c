// Task 3
// Oliver O'Hara

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

// Helper function from task 2
// Compute the function g^x mod p
unsigned long fme(unsigned long g, unsigned long x, unsigned long p)
{
   unsigned long answer = 1;

   while(x > 0)
   {
      // If x is odd then we can multiply with result
      if (x & 1)
      {
         answer = (answer * g) % p;
      } //if

      // At this point x is even and so we can use a right shift to divide by 2
      x = x>>1;
      g = (g*g) % p;
   } //while
   return answer;
} //fme


// Task 3:
// Discrete logarithm
// Compute x in the equation y = g^x mod p
// Does this by instead computing y^(i*n) = y^j * b (mod p)
// This algorithm runs with the time complexity O(sqrt(p) * log(y))
// Modular exponentiation is seen as a one-way function because it is the
// inverse of discrecte logarithms. Exponentiation can be computed quickly but
// there's no quick alogrithm to compute discrete logarithms and so it is a one
// way function as it is easy to compute but hard to invert. 
unsigned long dl(unsigned long y, unsigned long g, unsigned long p)
{
  // Number of bits in p
  double doublep = (double)p;
  int n = (int)(sqrt(doublep) + 1);

  // Array to store modular exponentiation for each bit (not ordered)
  unsigned long fmeValues[p];

  // Calculate and store the values y^(n*i) (LHS of equatioln)
  int i;
  for (i = n; i >= 1; i--)
  {
     fmeValues[fme(y, n * i, p)] = i;
  } //for

  int j;
  for (j = 0; j < n; j++)
  {
     // Find modular exponentiation a^i mod p and check if LHS = RHS
     unsigned long lhs = (fme(y, j, p) * g) % p;

     // If LHS = RHS
     if (fmeValues[lhs] > 0)
     {
        unsigned long answer = fmeValues[lhs] * n - j;
        // If answer is less than p then we can output
        if (answer < p)
        {
           return answer;
        } //if
     } //if
  } //for
  return 0;
} //dl

int main(int argc, char *argv[])
{
  // Test for task 1
  unsigned long y = strtoul(argv[1], NULL, 10);
  unsigned long g = strtoul(argv[2], NULL, 10);
  unsigned long p = strtoul(argv[3], NULL, 10);
  unsigned long result = dl(y, g, p);
  printf("\nx in the equation %lu = %lu^x mod %lu is %lu\n", y, g, p, result);
  return(0);
}
