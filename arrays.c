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

static void insert(struct person *peoplePointers[], char *name, int age,
                   int *nextfreeplace)
{
  peoplePointers[*nextfreeplace] = malloc(sizeof(struct person));

  if(!(peoplePointers[*nextfreeplace]))
  {
     exit(1);
  }

  /* put name and age into the next free place in the array parameter here */
  peoplePointers[*nextfreeplace]->name = name;
  peoplePointers[*nextfreeplace]->age = age;
  /* modify nextfreeplace here */
  (*nextfreeplace)++;
}

int main(int argc, char **argv)
{
  int nextfreeplace = 0;
  /* declare the people array here */
  //Declare an array of people
  struct person *people[HOW_MANY];

  for(int i = 0; i < HOW_MANY; i++)
  {
    insert (people, names[i], ages[i], &nextfreeplace);
  }

  /* print the people array here*/
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
