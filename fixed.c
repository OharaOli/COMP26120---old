#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// A struct to record pairs of strings
typedef struct pair {
  char* first;
  char* second;
} pair;


// processes a work pair by first storing in
// temp a version of work.first that is all
// lowercase and then storing in work.second
// a version where the first character is
// made uppercase
void process(pair* work)
{
  //Allocate a new space in memory to store a lower case version of the
  //input string and copy the string into it
  char* temp = (char*)(malloc(32 * sizeof(char)));
  strcpy(temp,work->first);

  for(int i=0;i<=strlen(work->first)+1;i++){
    char c = work->first[i];
    if(islower(c)){
      temp[i] = c;
    }
    else if(isupper(c)){
      temp[i] = tolower(c);
    }
    else
    {
      temp[i] = c;
    }
  }

  //Allocate a new space in memory to store the altered word and start by
  //copying the lowercase version of the input string
  work->second = (char*)(malloc(32 * sizeof(char)));
  strcpy(work->second,temp);
  char* ptr = work->second;

  //Identify the space character
  int last_space = 0;

  //Convert the first letter to upper case
  *ptr = toupper(*ptr);

  //For every space in the string, convert the next character to uppercase
  while(*ptr++){
    if(last_space){
      *ptr = toupper(*ptr);
    }
    last_space = *ptr == ((char) ' ');
  }

  //Free the allocated memory
  free(temp);
}

// Takes a single string and writes the result to stdout
int main(int argc, char **argv)
{
  //Create a pair and assign the pointer of first to the string
  pair work;
  work.first = argv[1];

  //Pass the value of work by reference
  process(&work);

  //Print the result
  printf("%s becomes %s\n",work.first,work.second);

  //Free the allocated memory
  free(work.second);
}
