// Task 1, 2, 4 and 5
// Oliver O'Hara

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

// Task 1:
// Highest common factor
// Compute the highest common factor of a and b
unsigned long long hcf(unsigned long long a, unsigned long long b)
{
   unsigned long long r;
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
unsigned long long fme(unsigned long long g, unsigned long long x, unsigned long long p)
{
   unsigned long long answer = 1;

   g = g % p;
   if (g == 0)
   {
      return 0;
   }  

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
unsigned long long imp(unsigned long long y, unsigned long long p)
{
   // Check if an inverse exists
   unsigned long long check = hcf(y, p);
   if (check != 1)
   {

      printf("\nInverse not possible for that input\n");
      return -1;
   } //if
   else
   {
      // y and p must be relatively prime and so we can use modular exponentiation
      return fme(y, p-2, p);
   }
} //imp

void getPublicKey(unsigned long long p, unsigned long long g)
{
   // Private key (x) (selected by user)
   unsigned long long x;
   printf("\n----------------------------------------");
   printf("\nPlease select your private key: ");
   scanf(" %llu", &x);

   // Check that the value given lies between 1 and p
   while (x < 1 || x >= p)
   {
      printf("\n----------------------------------------");
      printf("\nYour private key must be between 1 and p, try again: ");
      scanf(" %llu", &x);
   } //while

   // Public key (y) (calculated using fme)
   unsigned long long y = fme(g, x, p);
   printf("\nYour public key is: %llu", y);
   return;
} //getPublicKey


void encrypt(unsigned long long p, unsigned long long g)
{
   // Message to encrypt (m) (selected by user)
   unsigned long long m;
   printf("\n----------------------------------------");
   printf("\nPlease select number to send: ");
   scanf(" %llu", &m);

   // Check that the value given lies between 1 and p
   while (m < 1 || m >= p)
   {
      printf("\n----------------------------------------");
      printf("\nYour message must be between 1 and p, try again: ");
      scanf(" %llu", &m);
   } //while

   // Public key (y) (selected by user)
   unsigned long long y;
   printf("\n----------------------------------------");
   printf("\nPlease type your public key: ");
   scanf(" %llu", &y);

   // Select random number (k) in the range 1 to p
   srand(time(0));
   int k = (rand()%(p-1) + 1);
   printf("\nk = %d", k);

   // Find the encrypted values a and b
   unsigned long long a = fme(g, k, p);
   unsigned long long b = m*fme(y, k, p);

   // Output the values for a and b
   printf("\nThe encrypted message is: (%llu,%llu)", a, b);
} //encrypt


void decrypt(unsigned long long p, unsigned long long g)
{
   // String to hold the initial input for the encrypted message
   char eMessage[1000];
   char *token1;
   char *token2;
   // First part of the encrypted message (a) (entered by user)
   unsigned long long a;
   // Second part of the encrypted message (b) (entered by user)
   unsigned long long b;
   // Private key (x) (entered by user)
   unsigned long long x;

   printf("\n----------------------------------------");
   //printf("\nPlease enter the first part of the message: ");
   //scanf(" %lu", &a);
   //printf("\nPlease enter the second part of the message: ");
   //scanf(" %lu", &b);
   printf("\nPlease enter the secret message: ");

   scanf(" %s", &eMessage);
   printf("\nEntered: %s", eMessage);
   memmove(eMessage, eMessage+1, strlen(eMessage));
   eMessage[strlen(eMessage)-1] = 0;
   token1 = strtok(eMessage, ",");
   token2 = strtok(NULL, ",");
   a = strtoull(token1, NULL, 10);
   b = strtoull(token2, NULL, 10);

   printf("\nPlease enter your private key: ");
   scanf(" %llu", &x);

   // Find the decrypted message
   unsigned long long decryptedMessage = (b / fme(a, x, p));

   // Output the decrypted message
   printf("The decrypted message is: %llu", decryptedMessage);
} //decrypt

int main(int argc, char *argv[])
{
  // Prime modulus (p) (given)
  unsigned long long p = 65537;
  // Primitive root (g) (given)
  unsigned long long g = 3;
  // Print p and g
  printf("\n----------------------------------------");
  printf("\nPrime modulus is %llu", p);
  printf("\nPrimitive root wrt %llu is %llu", p, g);

  //Give the user a choice of functions and read input
  char choice;
  do{
  printf("\n----------------------------------------");
  printf("\nPlease select one of the following:");
  printf("\ne: Encrypt");
  printf("\nd: Decrypt");
  printf("\nk: Get public key");
  printf("\nx: Exit");
  printf("\n----------------------------------------\n");
  //scanf("%d", &choice);
  scanf(" %c", &choice);

  // Branch to the correct function based on the input
  switch(choice)
  {
     case 'e':
       encrypt(p, g);
       break;
     case 'd':
       decrypt(p, g);
       break;
     case 'k':
       getPublicKey(p, g);
       break;
     case 'x':
       break;
     default:
       printf("\nInvalid input");
       break;
  } //switch
  }while(choice != 0);

  return(0);
}
