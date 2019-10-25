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

static void insert(struct person people[], char *name, int age)
{
  static int nextfreeplace = 0;
  /* put name and age into the next free place in the array parameter here */
  people[nextfreeplace].name = name;
  people[nextfreeplace].age = age;
  /* modify nextfreeplace here */
  nextfreeplace++;
}

int main(int argc, char **argv)
{

  /* declare the people array here */
  //Declare an array of people
  struct person people[HOW_MANY];

  for (int i = 0; i < HOW_MANY; i++)
  {
    insert (people, names[i], ages[i]);
  }

  /* print the people array here*/
  for (int j = 0; j < HOW_MANY; j++)
  {
    printf("Person %d:\n", j + 1);
    printf("Name: %s\n", people[j].name);
    printf("Age: %d\n", people[j].age);
    printf("\n");
  }

  return 0;
}
