#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "arrays.h"

/* declare your struct for a person here */
//A struct to record a person
typedef struct person {
  char* name;
  int age;
} person;

//Insert the information about a person to the person pointed to in the
//next free place
static void insert(struct person *peoplePointers[], char *name, int age,
                   int *nextfreeplace)
{
  //Allocate a pointer at the next free place and check that it is done
  //correctly
  peoplePointers[*nextfreeplace] = malloc(sizeof(struct person));
  if(!(peoplePointers[*nextfreeplace]))
  {
     exit(1);
  }

  /* put name and age into the next free place in the array parameter here */
  //Puts name and age at this pointer
  peoplePointers[*nextfreeplace]->name = name;
  peoplePointers[*nextfreeplace]->age = age;

  /* modify nextfreeplace here */
  //Increment next free place
  (*nextfreeplace)++;
}

int main(int argc, char **argv)
{
  //Initialise nextfreeplace
  int nextfreeplace = 0;

  /* declare the people array here */
  //Declare an array of people
  struct person *people[HOW_MANY];

  //Insert the names and ages from arrays.h
  for(int i = 0; i < HOW_MANY; i++)
  {
    insert (people, names[i], ages[i], &nextfreeplace);
  }

  /* print the people array here*/
  //Print the people array
  for(int j = 0; j < HOW_MANY; j++)
  {
    //printf("Person %d:\n", j + 1);
    //printf("Name: %s\n", people[j]->name);
    //printf("Age: %d\n", people[j]->age);
    //printf("\n");
    printf("%d: %s is %d\n", j, people[j]->name, people[j]->age);
  }

  //Free the allocated memory
  for(int k = 0; k < HOW_MANY; k++)
  {
     free(people[k]);
  }

  return 0;
}
