#include <stdio.h>
#define B_SIZE 1024

int main(int argc, char **argv)
{
  printf("Part 1\n");
  //Basis for the input and output file names
  char inFileName[100] = "./";
  char outFileName[100] = "./";
  //Variable to hold the string found in the input file
  char string[100];
  //Variables to hold the strings that the user inputs for the files
  char inputString[100];
  char outputString[100];

  //Get the names that the user would like to use for the files
  printf("Please enter the file that you want to read from: ");
  gets(inputString);
  printf("Please enter the file that you want to write to: ");
  gets(outputString);
  
  //Create the files by using the directory, file name and .txt
  FILE *in;
  FILE *out;

  strcat(inFileName, inputString);
  strcat(inFileName, ".txt");

  strcat(outFileName, outputString);
  strcat(outFileName, ".txt");

  //Open the files as read and write files
  in = fopen(inFileName, "r");
  out = fopen(outFileName, "w");
  
  //Read the string from the file
  int character = 0;
  int i = 0;
  while((character = fgetc(in)) != EOF)
  {
     string[i] = character;
     i++; 
  }

  //Read each character from the string in turn, deciding whether it should be
  //converted or not and alter the variables to hold these values accordingly
  i = 0;
  int numberOfCharacters = 0;
  int convertedToUpper = 0;
  int convertedToLower = 0;

  for (i = 0; string[i] != '\0'; i++)
  {
     numberOfCharacters++; 
     if(string[i] >= 'a' && string[i] <= 'z')
     {
        string[i] = string[i] - 32;
        convertedToUpper++;
     }
     else if(string[i] >= 'A' && string[i] <= 'Z')
     {
        string[i] = string[i] + 32;
        convertedToLower++;
     }
  }

  //Print the converted word to the standard output and write it to the file
  printf("\nConverted word: ");
  puts(string);
  fprintf(out, string);

  //Ouput the information regarding the change string to the standard output
  printf("Read %d characters in total, %d converted to upper-case, "
          "%d to lower-case\n", numberOfCharacters, convertedToUpper, convertedToLower);
  
  //Close the files that we have used
  fclose(in);
  fclose(out);
  printf("\nEnd of Part 1\n");
}
