#include "sorting.h"
#include <stdio.h>
#include <stdlib.h>

void sort(struct darray* arr, int select){

  switch(select){
    case 2: insertion_sort(arr); break;
    case 3: quick_sort(arr, 0, (arr->size - 1)); break;
    case 4: bucket_sort(arr); break;
    case 5: merge_sort(arr, 0, (arr->size - 1)); break;
    case 6: bubble_sort(arr); break;
    default:
      fprintf(stderr,"The value %d is not supported in sorting.c\n",select);
      // Keep this exit code as the tests treat 23 specially
      exit(23);
  }
}


// You may find this helpful
void swap(char* *a, char* *b)
{
        char* temp = *a;
        *a = *b;
        *b = temp;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void insertion_sort(struct darray* arr)
{
   //Loop through all the elements in the array, inserting them into the correct
   //position
   for(int index = 1; index < arr->size; index++)
   {
      //Pick out the value to be inserted
      Value_Type insert_value = arr->cells[index];
      int j = index - 1;

      //While the value to the left of the one to be inserted is less than it
      //swap the values around
      while(j >= 0 && (compare(arr->cells[j], insert_value)) > 0)
      {
         arr->cells[j + 1] = arr->cells[j];
         j = j - 1;
      } //while
      //Move the value back into the correct place once it is found
      arr->cells[j + 1] = insert_value;
   } //for
} //insertion_sort

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Helper function that takes the last element as a pivot, places it at the
//correct position. All the smaller items are placed on the left, all the larger
//ones on the right
int partition(struct darray* arr, int low, int high )
{
    Value_Type pivot = arr->cells[high];
    int i = low - 1;
    for(int j = low; j <= high -1; j++)
    {
        if(compare(arr->cells[j] , pivot) < 0)
        {
            i = i + 1;
            swap(&arr->cells[i], &arr->cells[j]);
        } //if
    } //for
    swap(&arr->cells[i + 1], &arr->cells[high]);
    return(i + 1);
} //partition

void quick_sort(struct darray* arr, int low, int high)
{
   //As long as the starting index is less than the ending index
   if(low < high)
   {
      //Find the partition index using the helper function
      int partition_index = partition(arr, low, high);
      //Recursively call to sort the elements before and after the partition
      quick_sort(arr, low, (partition_index - 1));
      quick_sort(arr, (partition_index + 1), high);
   } //if
} //quick_sort

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void bucket_sort(struct darray* arr)
{}
   /*
   //Create buckets
   vector<Value_Type> bucket[arr->size];

   //Put array elements in different buckets
   for(int index = 0; index < arr->size; index++)
   {
      int bucketIndex = arr->size*arr[index];
      bucket[bucketIndex].push_back(arr[i]);
   } //for

   //Sort each bucket
   for(int index = 0; index < arr->size; index++)
   {
      sort(bucket[index].begin(), bucket[index].end());
   } //for

   //Combine the sorted buckets
   int arrayIndex = 0;
   for(int i = 0; i < arr->size; i++)
   {
      for(int k = 0; k < bucket[i].size(); k++)
      {
         arr[arrayIndex++] = bucket[i][k];
      } //for
   } //for
} //bucket_sort
*/
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Helper function to merge two halves of the array being sorted by the merge
//sort function
void merge(int arr[], int leftIndex, int middleIndex, int rightIndex)
{
    int leftArraySize = middleIndex - leftIndex + 1;
    int rightArraySize =  rightIndex - middleIndex;

    //Create two temporary arrays for each half
    Value_Type *leftArray = malloc(leftArraySize * sizeof(Value_Type));
    Value_Type *rightArray = malloc(rightArraySize * sizeof(Value_Type));
    leftArray[leftArraySize];
    rightArray[rightArraySize];

    //Copy the data to these temporary variables
    for (int i = 0; i < leftArraySize; i++)
    {
        leftArray[i] = arr[leftIndex + i];
    } //for
    for (int j = 0; j < rightArraySize; j++)
    {
        rightArray[j] = arr[middleIndex + 1+ j];
    } //for

    //Merge the temporary arrays back into original array
    int mergeIndex = leftIndex; // Initial index of merged subarray
    leftIndex = 0; // Initial index of first subarray
    rightIndex = 0; // Initial index of second subarray
    while (leftIndex < leftArraySize && rightIndex < rightArraySize)
    {
        if (leftArray[leftIndex] <= rightArray[rightIndex])
        {
            arr[mergeIndex] = leftArray[leftIndex];
            leftIndex++;
        } //if
        else
        {
            arr[mergeIndex] = rightArray[rightIndex];
            rightIndex++;
        } //else
        mergeIndex++;
    } //while

    //Copy any remaining elements from the left or right temporary arrays back
    //into array
    while (leftIndex < leftArraySize)
    {
        arr[mergeIndex] = leftArray[leftIndex];
        leftIndex++;
        mergeIndex++;
    } //while

    while (rightIndex < rightArraySize)
    {
        arr[mergeIndex] = rightArray[rightIndex];
        rightIndex++;
        mergeIndex++;
    } //while
} //merge

void merge_sort(struct darray* arr, int leftIndex, int rightIndex)
{
   if(leftIndex < rightIndex)
   {
      //Find the index of the middle of the array
      int middleIndex = (leftIndex + rightIndex) / 2;

      //Recursively sort the two halves of the list
      merge_sort(arr, leftIndex, middleIndex);
      merge_sort(arr, middleIndex + 1, rightIndex);

      //Merge the sorted lists using the helper function
      merge(arr, leftIndex, middleIndex, rightIndex);
   }
} //merge_sort

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void bubble_sort(struct darray* arr)
{
  for(int i = 0; i < arr->size; i++)
  {
    for(int k = 0; k < arr->size - i - 1; k++)
    {
      if(compare(arr->cells[k], arr->cells[k + 1]) > 0)
      {
        Value_Type swapValue = arr->cells[k];
        arr->cells[k] = arr->cells[k + 1];
        arr->cells[k + 1] = swapValue;
      } //if
    } //for
  } //for
} //bubble_sort
