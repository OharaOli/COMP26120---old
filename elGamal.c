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

void getPublicKey(unsigned long p, unsigned long g)
{
   // Private key (x) (selected by user)
   unsigned long x;
   printf("\n----------------------------------------");
   printf("\nPlease select your private key: ");
   scanf(" %lu", &x);

   // Check that the value given lies between 1 and p
   while (x < 1 || x >= p)
   {
      printf("\n----------------------------------------");
      printf("\nYour private key must be between 1 and p, try again: ");
      scanf(" %lu", &x);
   } //while

   // Public key (y) (calculated using fme)
   unsigned long y = fme(g, x, p);
   printf("\nYour public key is: %lu", y);
   return;
} //getPublicKey


void encrypt(unsigned long p, unsigned long g)
{
   // Message to encrypt (m) (selected by user)
   unsigned long m;
   printf("\n----------------------------------------");
   printf("\nPlease select number to send: ");
   scanf(" %lu", &m);

   // Check that the value given lies between 1 and p
   while (m < 1 || m >= p)
   {
      printf("\n----------------------------------------");
      printf("\nYour message must be between 1 and p, try again: ");
      scanf(" %lu", &m);
   } //while

   // Public key (y) (selected by user)
   unsigned long y;
   printf("\n----------------------------------------");
   printf("\nPlease type your public key: ");
   scanf(" %lu", &y);

   // Select random number (k) in the range 1 to p
   srand(time(0));
   int k = (rand()%(p-1) + 1);
   printf("\nk = %d", k);

   // Find the encrypted values a and b
   unsigned long a = fme(g, k, p);
   unsigned long b = m*fme(y, k, p);

   // Output the values for a and b
   printf("\nThe encrypted message is: (%lu,%lu)", a, b);
} //encrypt


void decrypt(unsigned long p, unsigned long g)
{
   // String to hold the initial input for the encrypted message
   char eMessage[100];
   char *token1;
   char *token2;
   // First part of the encrypted message (a) (entered by user)
   unsigned long a;
   // Second part of the encrypted message (b) (entered by user)
   unsigned long b;
   // Private key (x) (entered by user)
   unsigned long x;

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
   a = strtoul(token1, NULL, 10);
   b = strtoul(token2, NULL, 10);

   printf("\nPlease enter your private key: ");
   scanf(" %lu", &x);

   // Find the decrypted message
   unsigned long decryptedMessage = (b / fme(a, x, p));

   // Output the decrypted message
   printf("The decrypted message is: %lu", decryptedMessage);
} //decrypt

int main(int argc, char *argv[])
{
  // Prime modulus (p) (given)
  unsigned long p = 65537;
  // Primitive root (g) (given)
  unsigned long g = 3;
  // Print p and g
  printf("\n----------------------------------------");
  printf("\nPrime modulus is %lu", p);
  printf("\nPrimitive root wrt %lu is %lu", p, g);

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
