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

char **g_argv;
int g_argc;

//Insert the information about a person to the start of the list
static person* insert_start(struct person** start, char *name, int age)
{
  //Allocate a pointer for the new person and check it works correctly
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

  //Puts this pointer at the start of the list
  start = new_person;
  return start;
}



//Insert the information about a person at the end of the list
static person* insert_end(struct person** start, char *name, int age)
{
  //Allocate a pointer for the new person and check it works correctly
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
  new_person->next = NULL;

  //Create another point to the person at the end of the list
  struct person* current_person = start;

  //Check for an empty list and then for the next null pointer
  if(current_person == NULL)
  {
    start = new_person;
    return start;
  }
  else
  {
    while(current_person->next != NULL)
    {
      current_person = current_person->next;
    }
    current_person->next = new_person;
    return start;
  }
}


//Insert the information about a person in the order of the list
static person* insert_sorted(struct person** start, char *name, int age)
{
  //Allocate a pointer for the new person and check it works correctly
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

  //Creates a pointer that will point to the comparable person
  struct person* current_person = start;

  //Order the list by either name or age based on the arguments given
  if(g_argc == 2 || (strcmp (g_argv[2],"name") == 0))
  {
    if(current_person == NULL || (strcmp(name, current_person->name)) <= 0)
    {
      new_person->next = start;
      start = new_person;
      return start;
    }
    else
    {                      
      while(current_person->next != NULL && (strcmp(name, current_person->next->name)) >= 0)
      {
        current_person = current_person->next; 
      }
  
    new_person->next = current_person->next;
    current_person->next = new_person;
    return start;
    }
  }
  else
  {
    if(current_person == NULL || age < current_person->age)
    {
      new_person->next = start;
      start = new_person;
      return start;
    }
    else
    {                      
      while(current_person->next != NULL && age > current_person->next->age)
      {
        current_person = current_person->next; 
      }
  
    new_person->next = current_person->next;
    current_person->next = new_person;
    return start;
    }
  }
}


//main
int main(int argc, char **argv)
{
  g_argv = argv;
  g_argc = argc;

  //Declare an array of people
  person* start = NULL;

  //Insert the names and ages based on arguments
  if(argc == 1 || strcmp (argv[1],"insert_start") == 0)
  {
    int i;
    for(i = 0; i < HOW_MANY; i++)
    {
      start = (insert_start (start, names[i], ages[i]));
    }
  }
  else if(argc == 2 && strcmp (argv[1],"insert_end") == 0)
  {
    int i;
    for(i = 0; i < HOW_MANY; i++)
    {
      start = (insert_end (start, names[i], ages[i]));
    }
  }
  else if(argc == 2 && strcmp (argv[1],"insert_sorted") == 0)
  {
    int i;
    for(i = 0; i < HOW_MANY; i++)
    {
      start = (insert_sorted (start, names[i], ages[i]));
    }
  }
  else if(argc == 3 && strcmp (argv[1],"insert_sorted") == 0 
          && (strcmp (argv[2],"name") == 0 || strcmp (argv[2],"age") == 0))
  {
    int i;
    for(i = 0; i < HOW_MANY; i++)
    {
      start = (insert_sorted (start, names[i], ages[i]));
    }
  }
  else
  {
    printf("Error: Unexpected arguments\n");
    return 2;
  }

  //Print the people array and free the memory
  int j;
  struct person* temp_start;
  for(j = 0; j < HOW_MANY; j++)
  {
    printf("Name: %s, age: %d\n", start->name, start->age);
    temp_start = start;
    start = start->next;
    free(temp_start);
  }
 
  //Free the initially allocated memory
  free(start);

  return 0;
}
