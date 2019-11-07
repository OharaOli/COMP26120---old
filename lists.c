#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "data.h"

//A struct to record a person
typedef struct person {
  char* name;
  int age;
  struct person* next;
} person;

//Insert the information about a person to the person pointed to in the
//next free place
static person* insert(struct person** start, char *name, int age)
{
  //Allocate a pointer at the next free place and check that it is done
  //correctly
  struct person* new_person;
  new_person = malloc(sizeof(person));
  if(new_person == NULL)
  {
    printf("Memory allocation failed");
    return;
  }

  //Puts name and age at this pointer
  new_person->name = name;
  new_person->age = age;
  new_person->next = start;

  start = new_person;
  return start;
}

int main(int argc, char **argv)
{
  //Declare an array of people
  person* start = NULL;
  start = malloc(sizeof(person));
  if(start == NULL)
  {
    printf("Memory allocation failed");
    return;
  }

  //Insert the names and ages from arrays.h
  int i;
  for(i = 0; i < HOW_MANY; i++)
  {
    start = (insert (start, names[i], ages[i]));
  }

  //Print the people array and free the memory
  int j;
  struct person* temp_start;
  for(j = 0; j < HOW_MANY; j++)
  {
    printf("%d: %s is %d\n", j + 1, start->name, start->age);
    temp_start = start;
    start = start->next;
    free(temp_start);
  }
 
  //Free the initially allocated memory
  free(start);

  return 0;
}
