#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define __USE_BSD
#include <string.h>
#include <math.h>

#include "global.h"
#include "hashset.h"

// Can be redefined if Value_Type changes
int compare(Value_Type a, Value_Type b){
  return strcmp(a,b);
}

// Helper functions for finding prime numbers
bool isPrime (int n)
{
  for (int i = 2; i*i <= n; i++)
    if (n % i == 0)
      return false;
  return true;
}

int nextPrime(int n)
{
  for (; !isPrime(n); n++);
  return n;
}

// Linked list to deal with overflows
cell* insert_end(Value_Type value, cell* c)
{
  /* put value into the next free place in the array parameter here */

  //Attempt to add the cell to an empty list
  cell *newCell = (cell*) malloc(sizeof(cell));
  //Check if memory is allocated correctly
  check(newCell);
  newCell->element = strdup(value);
  newCell->state = in_use;
  //If the list is empty
  if(c == NULL)
  {
    newCell->next = c;
    return newCell;
  }

  //If the list is not empty loop through until the end
  cell *temp = c;
  newCell->next = NULL;
  while(1)
  {
    if(c->next == NULL)
    {
      c->next = newCell;
      break;
    }
    c = c->next;
  }
  //Return the new cell
  return temp;
}


// Helper methods
//Print contents of a linked list
void print_linked(cell* c) {
  while (c) {
    printf("\t%s\n", c->element);
    c = c->next;
  }
}

//Method to search a linked list
bool linked_search(Value_Type value, cell* c) {
  while (c) {
    if (compare(value, c->element) == 0)
      return true;
    c = c->next;
  }
  return false;
}

//Free memory used in the linked list
void tidy_linked(cell* c) {
  cell *temp = NULL;
  while (c) {
    temp = c->next;
    free(c->element);
    free(c);
    //Allocate c back to NULL
    c = temp;
  }
}

struct hashset* initialize_set (int size)
{
// TODO create initial hash table
  //Ensure that the size of the hash table is prime to prevent clustering
  if (!isPrime(size))
    size = nextPrime(size);

  //Allocate memory for the hashset and the cells
  struct hashset *newHashSet = malloc(sizeof(struct hashset));
  newHashSet->cells = (cell*) (malloc(sizeof(cell)*size));
  //Check allocation has worked correctly
  check(newHashSet);
  check(newHashSet->cells);

  //Set the attributes for the cells
  newHashSet->size = size;
  for(int i = 0; i < size; i++)
  {
    (newHashSet->cells[i]).element = NULL;
    (newHashSet->cells[i]).next = NULL;
    (newHashSet->cells[i]).state = empty;
  }
  newHashSet->num_entries = 0;
  newHashSet->resizes = 0;
  newHashSet->secondDimensionSize = 0;
  newHashSet->loadFactor = 0;
  newHashSet->loadThreshold = 0.75;
  //Return the new hashset
  return newHashSet;
}

//Free allocated memory
void tidy(struct hashset* set)
{
  if(set)
  {
    //Free memory
    for(unsigned i=0; i<set->size;i++)
    {
      free((set->cells[i]).element); tidy_linked((set->cells[i]).next);
    }
    //Struct and array
    free(set->cells);
    free(set);
  }
}

//Return the number of entries in the table
int size(struct hashset* set)
{
  return set->num_entries;
}

//Insert a value into the hash table
struct hashset* insert (Value_Type value, struct hashset* set)
{
//If the load threshold is above 0.75 rehash the table
  if(set->loadFactor > set->loadThreshold)
  {
     //Double the size of the hashset
    struct hashset* new_set = initialize_set(set->size*2);
    //Rehash any non-empty cells into the new hash table
    for(unsigned int i=0; i<set->size; i++)
    {
      if((set->cells[i]).state == empty)
	      continue;
      //Rehash non-empty cells and free the memory it was allocated
      new_set = insert((set->cells[i]).element, new_set);
      free((set->cells[i]).element);
    }
    //Update the attributes
    new_set->num_entries = set->num_entries;
    new_set->resizes = set->resizes + 1;
    //Free the memory allocated to old cells
    free(set->cells);
    free(set);
    set = new_set;
  }

  //Check that the value isnt already present before adding it
  if (!find(value, set))
  {
    int hashedIndex = hash(value, set, mode);
    //Check if it's possible to add the value
    switch(hashedIndex)
    {
      //If it isn't possible cause a rehashing to occur
      case -1:
      {
        set->loadFactor = set->loadThreshold+0.01;
        set = insert(value, set);
      } break;
      //No errors - place the value in the table
      default:
      {
        //If in use insert into linked list else into hash table
        if((mode == HASH_1_SEPARATE_CHAINING || mode == HASH_2_SEPARATE_CHAINING) && (set->cells[hashedIndex]).state == in_use)
        {
          (set->cells[hashedIndex]) = *(insert_end(value, &(set->cells[hashedIndex])));
          set->secondDimensionSize++;
          set->num_entries++;
        }
        else
        {
          (set->cells[hashedIndex]).state = in_use;
          (set->cells[hashedIndex]).element = strdup(value);
          set->num_entries++;
          set->loadFactor = set->num_entries/set->size;
        }
      } break;
    }
  }
  return set;
}

//Basic hash function using the ASCII character of each character
int hash1(Value_Type value, struct hashset* set)
{
  int hashedIndex = 0;
  for(int i = 0; i < strlen(value); i++)
  {
    hashedIndex = hashedIndex + (int)value[i];
  }

  //Modulus to ensure it is within the hash table
  hashedIndex = hashedIndex % set->size;
  return hashedIndex;
}

//More advanced hash function that multiplies the ASCII value by a prime number
int hash2(Value_Type value, struct hashset* set)
{
  int hashedIndex = 0;
  int prime = 2;

  for (int i = 0; i < strlen(value); i++)
  {
    hashedIndex = hashedIndex + (((int) value[i]) * prime);
    prime = nextPrime(prime);
  }

  //Modulus to ensure it is within the hash table
  hashedIndex = hashedIndex % set->size;
  return hashedIndex;
}

int hash (Value_Type value, struct hashset* set, int mode)
{
  switch(mode)
  {
    case HASH_1_LINEAR_PROBING:
    {
        int possibleHashedIndex;
        int hashedIndex = hash1(value, set);
        for (int i=0; i<set->size; i++)
        {
	        possibleHashedIndex = (hashedIndex+i) % set->size;
           if((set->cells[possibleHashedIndex]).state == in_use && i == set->size-1)
           {
             return -1;
           }
          else if((set->cells[possibleHashedIndex]).state == in_use)
          {
             continue;
          }
          hashedIndex = possibleHashedIndex;
          break;
        }
        return hashedIndex;
    } break;
    case HASH_1_QUADRATIC_PROBING:
    {
        int possibleHashedIndex;
        int hashedIndex = hash1(value, set);
        for (int i=0; i<set->size; i++)
        {
	        possibleHashedIndex = (hashedIndex+(i*i)) % set->size;
           if((set->cells[possibleHashedIndex]).state == in_use && i == set->size-1)
           {
             return -1;
           }
          else if((set->cells[possibleHashedIndex]).state == in_use)
          {
             continue;
          }
          hashedIndex = possibleHashedIndex;
          break;
        }
        return hashedIndex;
    } break;
    case HASH_1_DOUBLE_HASHING:
    {
        int possibleHashedIndex;
        int hashedIndex = hash1(value, set);
        for (int i=0; i<set->size; i++)
        {
	        possibleHashedIndex = (hashedIndex+(i*hash2(value, set))) % set->size;
           if((set->cells[possibleHashedIndex]).state == in_use && i == set->size-1)
           {
             return -1;
           }
          else if((set->cells[possibleHashedIndex]).state == in_use)
          {
             continue;
          }
          hashedIndex = possibleHashedIndex;
          break;
        }
        return hashedIndex;
    } break;
    case HASH_1_SEPARATE_CHAINING:
    {
        int hashedIndex = hash1(value, set);
        return hashedIndex;
    } break;
    case HASH_2_LINEAR_PROBING:
    {
        int possibleHashedIndex;
        int hashedIndex = hash2(value, set);
        for (int i=0; i<set->size; i++)
        {
	        possibleHashedIndex = (hashedIndex+i) % set->size;
           if((set->cells[possibleHashedIndex]).state == in_use && i == set->size-1)
           {
             return -1;
           }
           else if((set->cells[possibleHashedIndex]).state == in_use)
           {
             continue;
           }
           hashedIndex = possibleHashedIndex;
           break;
        }
        return hashedIndex;
    } break;
    case HASH_2_QUADRATIC_PROBING:
    {
        int possibleHashedIndex;
        int hashedIndex = hash2(value, set);
        for (int i=0; i<set->size; i++)
        {
	        possibleHashedIndex = (hashedIndex+(i*i)) % set->size;
           if((set->cells[possibleHashedIndex]).state == in_use && i == set->size-1)
           {
             return -1;
           }
          else if((set->cells[possibleHashedIndex]).state == in_use)
          {
             continue;
          }
          hashedIndex = possibleHashedIndex;
          break;
        }
        return hashedIndex;
    } break;
    case HASH_2_DOUBLE_HASHING:
    {
        int possibleHashedIndex;
        int hashedIndex = hash2(value, set);
        for (int i=0; i<set->size; i++)
        {
	        possibleHashedIndex = (hashedIndex+(i*hash1(value, set))) % set->size;
           if((set->cells[possibleHashedIndex]).state == in_use && i == set->size-1)
           {
             return -1;
           }
           else if((set->cells[possibleHashedIndex]).state == in_use)
           {
             continue;
           }
          hashedIndex = possibleHashedIndex;
          break;
        }
        return hashedIndex;
    } break;
    case HASH_2_SEPARATE_CHAINING:
    {
        int hashedIndex = hash2(value, set);
        return hashedIndex;
    } break;
    default:
      fprintf(stderr,
              "The value %d is not supported in hashset.c\n",
              mode);
      exit(23);
    }
}

bool find (Value_Type value, struct hashset* set)
{
  switch(mode)
  {
    case HASH_1_LINEAR_PROBING:
    {
        int possibleHashedIndex;
        int hashedIndex = hash1(value, set);
        for (int i=0; i<set->size; i++)
        {
  	       possibleHashedIndex = (hashedIndex+i) % set->size;
          if((set->cells[possibleHashedIndex]).state == empty)
          {
             break;
          }
          else if(compare((set->cells[possibleHashedIndex]).element, value) == 0)
          {
             return true;
          }
        }
        return false;
    } break;
    case HASH_1_QUADRATIC_PROBING:
    {
        int possibleHashedIndex;
        int hashedIndex = hash1(value, set);
        for (int i=0; i<set->size; i++)
        {
  	       possibleHashedIndex = (hashedIndex+(i*i)) % set->size;
          if((set->cells[possibleHashedIndex]).state == empty)
          {
             break;
          }
          else if(compare((set->cells[possibleHashedIndex]).element, value) == 0)
          {
             return true;
          }
        }
        return false;
    } break;
    case HASH_1_DOUBLE_HASHING:
    {
        int possibleHashedIndex;
        int hashedIndex = hash1(value, set);
        for (int i=0; i<set->size; i++)
        {
  	       possibleHashedIndex = (hashedIndex+(i*hash2(value, set))) % set->size;
          if((set->cells[possibleHashedIndex]).state == empty)
          {
             break;
          }
          else if(compare((set->cells[possibleHashedIndex]).element, value) == 0)
          {
             return true;
          }
        }
        return false;
    } break;
    case HASH_1_SEPARATE_CHAINING:
    {
        int hashedIndex = hash1(value, set);
        if((set->cells[hashedIndex]).state == empty)
        {
           break;
        }
        else if(compare((set->cells[hashedIndex]).element, value) == 0)
        {
           return true;
        }
        else
        {
           return linked_search(value, &(set->cells[hashedIndex]));
        }
    } break;
    case HASH_2_LINEAR_PROBING:
    {
        int possibleHashedIndex;
        int hashedIndex = hash2(value, set);
        for (int i=0; i<set->size; i++)
        {
  	       possibleHashedIndex = (hashedIndex+i) % set->size;
          if((set->cells[possibleHashedIndex]).state == empty)
          {
             break;
          }
          else if(compare((set->cells[possibleHashedIndex]).element, value) == 0)
          {
             return true;
          }
        }
        return false;
    } break;
    case HASH_2_QUADRATIC_PROBING:
    {
        int possibleHashedIndex;
        int hashedIndex = hash2(value, set);
        for (int i=0; i<set->size; i++)
        {
  	       possibleHashedIndex = (hashedIndex+(i*i)) % set->size;
          if((set->cells[possibleHashedIndex]).state == empty)
          {
             break;
          }
          else if(compare((set->cells[possibleHashedIndex]).element, value) == 0)
          {
             return true;
          }
        }
        return false;
    } break;
    case HASH_2_DOUBLE_HASHING:
    {
        int possibleHashedIndex;
        int hashedIndex = hash2(value, set);
        for (int i=0; i<set->size; i++)
        {
  	       possibleHashedIndex = (hashedIndex+(i*hash1(value, set))) % set->size;
          if((set->cells[possibleHashedIndex]).state == empty)
          {
             break;
          }
          else if(compare((set->cells[possibleHashedIndex]).element, value) == 0)
          {
            return true;
          }
        }
        return false;
    } break;
    case HASH_2_SEPARATE_CHAINING:
    {
        int hashedIndex = hash2(value, set);
        if((set->cells[hashedIndex]).state == empty)
        {
           break;
        }
        else if(compare((set->cells[hashedIndex]).element, value) == 0)
        {
           return true;
        }
        else
        {
          return linked_search(value, &(set->cells[hashedIndex]));
        }
    } break;
    default:
      fprintf(stderr,
              "The value %d is not supported in hashset.c\n",
              mode);
      exit(23);
    }
}

//Print the statistics of the hash table
void print_stats (struct hashset* set)
{
  if (mode == HASH_1_SEPARATE_CHAINING || mode == HASH_2_SEPARATE_CHAINING)
  {
    printf("Hashtable of capacity %d contains %d elements, with %d elements in its second dimension\n",set->size,set->num_entries,set->secondDimensionSize);
  }
  else
  {
    printf("Hashtable of capacity %d contains %d elements\n",set->size,set->num_entries);
  }
  printf("Hashtable was resized %d times\n",set->resizes);
}

//Print the contents of the hash table
void print_set (struct hashset* set)
{
  printf("Hashtable:\n");
  for (int i = 0; i<set->size;i++)
  {
    if((set->cells[i]).state != 0)
    {
      printf("\t%s\n", (set->cells[i]).element);
      print_linked((set->cells[i]).next);
    }
  }
}
