// Giles Reger, 2019

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "global.h"
#include "pq.h"

// Some helper functions for checking the results of calls in tests
void check_result(int testno, char* expected, char* found){
 if(found == NULL){
   fprintf(stderr,"Test %d Failed when %s expected and NULL found\n",testno,expected);
   exit(-1);
 }
 if(strcmp(expected,found) != 0){
   fprintf(stderr,"Test %d Failed when %s expected and %s found\n",testno,expected,found);
   exit(-1);
 }
}
void assert(int testno, bool value, char* reason){
  if(!value){
   fprintf(stderr,"Test %d Failed as %s\n",testno,reason);
  }
}

// Check that hello and goodbye are popped in the right order
void run_test0(){
  printf("TEST 0\n");
  PriorityQueue queue = initialize_pq(10);
  printf("Initialised...\n");
  insert(queue,"hello",1);
  printf("Inserted hello with priority 1...\n");
  insert(queue,"goodbye",2);
  printf("Inserted goodbye with priority 2...\n");
  check_result(0,"hello",pop_min(queue));
  printf("Popped hello...\n");
  check_result(0,"goodbye",pop_min(queue));
  printf("Popped goodbye...\n");
  assert(0,is_empty(queue)," queue is meant to be empty");
  printf("Queue now empty\n");
  tidy(queue);
}

// Check that two elements with the same priority are popped in the right order
/*
void run_test1(){
  printf("TEST 1\n");
  PriorityQueue queue = initialize_pq(10);
  printf("Initialised...\n");
  insert(queue,"hello",1);
  printf("Inserted hello with priority 1...\n");
  insert(queue,"goodbye",1);
  printf("Inserted goodbye with priority 1...\n");
  insert(queue,"farewell",2);
  printf("Inserted farewell with priority 2...\n");
  check_result(1,"goodbye",pop_min(queue));
  printf("Popped hello...\n");
  check_result(1,"hello",pop_min(queue));
  printf("Popped goodbye...\n");
  check_result(1,"farewell",pop_min(queue));
  printf("Popped farewell...\n");
  assert(1,is_empty(queue)," queue is meant to be empty");
  printf("Queue now empty\n");
  tidy(queue);
}
*/
// Check "is_empty" on an empty queue
void run_test2(){
  printf("TEST 2\n");
  PriorityQueue queue = initialize_pq(10);
  printf("Initialised an empty queue...\n");
  assert(2,is_empty(queue)," queue is meant to be empty");
  printf("Queue now empty\n");
  tidy(queue);
}

// Check "is_empty" on a non-empty queue
void run_test3(){
  printf("TEST 3\n");
  PriorityQueue queue = initialize_pq(10);
  insert(queue,"hello",1);
  printf("Inserted hello with priority 1...\n");
  printf("Initialised an empty queue...\n");
  assert(3,!(is_empty(queue))," queue is not meant to be empty");
  check_result(3,"hello",pop_min(queue));
  printf("Popped hello...\n");
  assert(3,is_empty(queue)," queue is meant to be empty");
  printf("Queue now empty\n");
  tidy(queue);
}

// Check "contains" for a present item
void run_test4(){
  printf("TEST 4\n");
  PriorityQueue queue = initialize_pq(10);
  printf("Initialised...\n");
  insert(queue,"hello",1);
  printf("Inserted hello with priority 1...\n");
  assert(4,contains(queue,"hello",1)," queue should contain hello with priority 1");
  check_result(4,"hello",pop_min(queue));
  printf("Popped hello...\n");
  assert(4,is_empty(queue)," queue is meant to be empty");
  printf("Queue now empty\n");
  tidy(queue);
}

// Check "contains" for an item that isn't present
void run_test5(){
  printf("TEST 5\n");
  PriorityQueue queue = initialize_pq(10);
  printf("Initialised...\n");
  insert(queue,"hello",1);
  printf("Inserted hello with priority 1...\n");
  assert(5,!(contains(queue,"goodbye",2))," queue shouldn't contain goodbye with priority 2");
  check_result(5,"hello",pop_min(queue));
  printf("Popped hello...\n");
  assert(5,is_empty(queue)," queue is meant to be empty");
  printf("Queue now empty\n");
  tidy(queue);
}

// Check "contains" for an item with a different key
void run_test6(){
  printf("TEST 6\n");
  PriorityQueue queue = initialize_pq(10);
  printf("Initialised...\n");
  insert(queue,"hello",1);
  printf("Inserted hello with priority 1...\n");
  assert(6,!(contains(queue,"hello",2))," queue shouldn't contain goodbye with priority 2");
  check_result(6,"hello",pop_min(queue));
  printf("Popped hello...\n");
  assert(6,is_empty(queue)," queue is meant to be empty");
  printf("Queue now empty\n");
  tidy(queue);
}

char* prog_name = 0;
int main (int argc, char *argv[])
{
 prog_name=argv[0];
 if(argc != 2){
   fprintf(stderr,"Supply test number\n");
   return -1;
 }
 char* tmp;
 int test_number = strtol(argv[1],&tmp,0);
 if(*tmp != '\0'){
   fprintf(stderr,"Supply test number as an integer\n");
   return -1;
 }
 switch(test_number){
   case 0: run_test0(); break;
   case 1: run_test1(); break;
   case 2: run_test2(); break;
   case 3: run_test3(); break;
   case 4: run_test4(); break;
   case 5: run_test5(); break;
   case 6: run_test6(); break;
   default:
     fprintf(stderr,"Test number %d not recognised\n",test_number);
 }
 return 0;
}
