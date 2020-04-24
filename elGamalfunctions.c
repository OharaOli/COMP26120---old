// Task 1, 2, 4 and 5
// Oliver O'Hara

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

// Task 1:
// Highest common factor
// Compute the highest common factor of a and b
unsigned long hcf(unsigned long a, unsigned long b)
{
   unsigned long r;
   if (b == 0)
   {
      return a;
   } //if
   else
   {
      r = a % b;
      return hcf(b, r);
   } //else
} //hcf


// Task 2:
// Fast modular exponentiation
// Compute the function g^x mod p
// The time complexity of this function is O(log x) because each time that x is
// even we can divide it by two and this is the condition of the while loop
unsigned long fme(unsigned long g, unsigned long x, unsigned long p)
{
   unsigned long answer = 1;

   while(x > 0)
   {
      // If x is odd then we can multiply with result (bitwise AND on last bit)
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


// Task 4:
// Inverse modulo prime
// For a given y and p (prime) returns an x such that x*y = 1 mod p
//
unsigned long imp(unsigned long y, unsigned long p)
{
   // Check if an inverse exists
   unsigned long check = hcf(y, p);
   if (check != 1)
   {
      printf("\nInverse not possible for that input\n");
   } //if
   else
   {
      // y and p must be relatively prime and so we can use modular exponentiation
      return fme(y, p-2, p);
   }
} //imp


int main(int argc, char *argv[])
{
  // Test for task 1
  unsigned long a = strtoul(argv[1], NULL, 10);
  unsigned long b = strtoul(argv[2], NULL, 10);
  unsigned long result1 = hcf(a, b);
  printf("\nThe highest common factor of %lu and %lu is %lu\n", a, b, result1);

  // Test for task 2
  unsigned long g = strtoul(argv[3], NULL, 10);
  unsigned long x = strtoul(argv[4], NULL, 10);
  unsigned long p = strtoul(argv[5], NULL, 10);
  unsigned long result2 = fme(g, x, p);
  printf("\nThe modular exponentiation of %lu, %lu and %lu is %lu\n", g, x, p, result2);

  // Test for task 4
  unsigned long c = strtoul(argv[6], NULL, 10);
  unsigned long d = strtoul(argv[7], NULL, 10);
  unsigned long result3 = imp(c, d);
  printf("\nThe mod multaplicative inverse is %lu\n", result3);
  return(0);
}
