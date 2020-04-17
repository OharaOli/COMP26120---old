
// Dynamic programming for 0/1 knapsack
// (C) Joshua Knowles
// The variable names here follow standard usage in formulating
// dynamic programming via recursive definitions

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

FILE *fp;  // file pointer for reading the input files
int Capacity;     // capacity of the knapsack (total weight that can be stored)
int Nitems;    // number of items available
int *item_weights;  // vector of item weights
int *item_values;  // vector of item profits or values
int *temp_indexes;  // list of temporary item indexes for sorting items by value/weight
int QUIET=0; // this can be set to 1 to suppress output
int best_value; // total value packed in the best solution


// function prototypes
extern void read_knapsack_instance(char *filename);
int DP(int *v,int *wv, int n, int W, int *solution);
extern int check_evaluate_and_print_sol(int *sol,  int *total_value, int *total_weight);

int main(int argc, char *argv[1])
{
  int *solution;    // binary vector indicating items to pack
  int total_value, total_weight;  // total value and total weight of items packed

  read_knapsack_instance(argv[1]);

  if((solution = (int *)malloc((Nitems+1)*sizeof(int)))==NULL)
    {
      fprintf(stderr,"Problem allocating table for DP\n");
      exit(1);
    }

  best_value = DP(item_values,item_weights,Nitems,Capacity,solution);
  printf("\n");
  printf("\nBest solution...\n");
  check_evaluate_and_print_sol(solution,&total_weight,&total_value);
  printf("Optimal value = %d\n", best_value);
  return(0);
}

//int DP(int *v,int *wv, int n, int W, int *solution)
int DP(int *values,int *weights, int numberOfItems, int capacity, int *solution)
{
  // the dynamic programming function for the knapsack problem
  // the code was adapted from p17 of http://www.es.ele.tue.nl/education/5MC10/solutions/knapsack.pdf

  // v array holds the values / profits / benefits of the items
  // wv array holds the sizes / weights of the items
  // n is the total number of items
  // W is the constraint (the weight capacity of the knapsack)
  // solution: a 1 in position n means pack item number n+1. A zero means do not pack it.

  //int **V, **keep;  // 2d arrays for use in the dynamic programming solution
  // keep[][] and V[][] are both of size (n+1)*(W+1)

  int i, w, K;

  // Dynamically allocate memory for variables V and keep
  /* ADD CODE HERE */

  int *V[numberOfItems + 1];
  for (i = 0; i < numberOfItems + 1; i++)
  {
      V[i] = (int *)malloc((capacity + 1) * sizeof(int));
  } //for

  int *keep[numberOfItems + 1];
  for (i = 0; i < numberOfItems + 1; i++)
  {
      keep[i] = (int *)malloc((capacity + 1) * sizeof(int));
  } //for


  //  set the values of the zeroth row of the partial solutions table to zero
  /* ADD CODE HERE */

  // Where the number of items is 0, set the value to 0
  for (i = 0; i < capacity + 1; i++)
  {
     V[0][i] = 0;
  } //for
  // Where the weight is 0, set the value to 0
  for (i = 0; i < numberOfItems + 1; i++)
  {
     V[i][0] = 0;
  } //for


  // main dynamic programming loops , adding one item at a time and looping through weights from 0 to W
  /* ADD CODE HERE */

  // Loop through each item and weight and check wether the previous weight should
  // remain of if including the current item increases the value whilst remaining
  // the correct weight
  for (i = 1; i < numberOfItems + 1; i++)
  {
     for (w = 1; w < capacity + 1; w++)
     {
         // V[item][weight]
         // If the value of including the new item is greater than not
         if ((weights[i] <= w) && (values[i] + V[i-1][w-weights[i]] > V[i - 1][w]))
         {
            V[i][w] = values[i] + V[i-1][w-weights[i]];
            // Set keep to 1 as this will be used in the solution
            keep[i][w] = 1;
         } //if
         // If the previous value is greater (or the weight is too large) then keep
         // the previous value
         else
         {
            V[i][w] = V[i-1][w];
            // Set keep to 0 as this won't be used in the final solution
            keep[i][w] = 0;
         } //else
     } //for each capacity
  } //for each item

  // now discover which items were in the optimal solution
  /* ADD CODE HERE */

  // Set K as the capacity
  K = capacity;

  // Go down through each item and check whether it should be added to the solution
  for (i = numberOfItems; i > 0; i--)
  {
     // If the item should be added include it in the solution and reduce the
     // remaining capacity, else don't include it in the solution
     if (keep[i][K] == 1)
     {
        solution[i] = 1;
        K = K - weights[i];
     } //if
     else
     {
        solution[i] = 0;
     } //else
  } //for
  return V[numberOfItems][capacity];
}
