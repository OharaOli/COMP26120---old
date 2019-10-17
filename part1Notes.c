#include <stdio.h>
#define B_SIZE 1024

int main(int argc, char **argv)
{
  printf("Part 1\n");
  char inFileName[100] = "./";
  char outFileName[100] = "./";
  char string[100];
  char inputString[100];
  char outputString[100];

  printf("Please enter the file that you want to read from: ");
  gets(inputString);

  printf("Please enter the file that you want to write to: ");
  gets(outputString);
  //printf(inputString);

  FILE *in;
  FILE *out;
  //sprintf(fileName, "./%s.txt", fileName);
  strcat(inFileName, inputString);
  strcat(inFileName, ".txt");

  strcat(outFileName, outputString);
  strcat(outFileName, ".txt");

  //printf("\nStage 0\n");
  //printf(outFileName);
  //in = fopen("./input.txt", "r");
  in = fopen(inFileName, "r");
  out = fopen(outFileName, "w");
  //fprintf(out, "Some text: %s\n", outFileName);
  //if(access(in, R_OK) != -1) 
  //{
  //  printf("\nFile exists\n");
  //} 
  //else 
  //{
  //  printf("\nRIP file\n");
  //}
  //fclose(in);
  //printf("\nStage 1\n");
  //fgets(string, B_SIZE, in);
  //fscanf(in,"%[^\n]", string);
  
  int character = 0;
  int i = 0;
  while((character = fgetc(in)) != EOF)
  {
     string[i] = character;
     i++; 
  }

  //printf("\nStage 2\n");

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

  printf("\nConverted word: ");
  puts(string);
  fprintf(out, string);

  printf("Read %d characters in total, %d converted to upper-case, "
          "%d to lower-case\n", numberOfCharacters, convertedToUpper, convertedToLower);

  //fprintf(out, "Read %d characters in total, %d converted to upper-case, "
  //        "%d to lower-case\n", numberOfCharacters, convertedToUpper, convertedToLower);

  printf("\nEnd of Part 1\n");
}
