#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <assert.h>

#include "general.h"
#include "graph.h"
#include "pq.h"


/**
 * Implement the interface given in pq.h.
 *
 * We make a few proposals here, but you are welcome to deviate from that, as long as you
 * correctly implement the interface with the specified worst-case complexities!
 *
 */



/**
 * First, we propose a 1-based indexing into the heap, using 0 as "invalid index".
 *
 * Wrapping a heap-index into a structure prevents its accidental confusion with element indexes (Which might save you from a lot of hard-to-find bugs!).
 *
 */

typedef struct {size_t i;} hidx_t;


const hidx_t INVALID_HIDX = {0};
const hidx_t hidx_first = {1};

size_t idx_of(hidx_t i) {return i.i - 1;} /// Convert to zero-based index
bool hidx_is_invalid(hidx_t i) {return i.i == 0;}

// Index operations
hidx_t parent(hidx_t i) {return (hidx_t){i.i / 2}; }
hidx_t left(hidx_t i) {return (hidx_t){i.i * 2}; }
hidx_t right(hidx_t i) {return (hidx_t){i.i * 2 + 1}; }
bool has_parent(hidx_t i) {return parent(i).i>0; }
bool has_left(size_t n, hidx_t i) {return left(i).i<=n; }
bool has_right(size_t n, hidx_t i) {return right(i).i<=n; }

/**
 * The priority queue has 3 main components:
 *   1. An array D that maps nodes to priorities. It is independent from the content of the actual queue.
 *   2. An array H of nodes that encodes the heap of size heap_size.
 *   3. An array I, that maps each node to its current index in H. If a node u is not in the queue, we set H[u] = INVALID_HIDX.
 *
 */
struct _DPQ_t {
    size_t N;     // Number of elements
    size_t heap_size;    // Current size of heap
    weight_t *D;  // Priorities
    node_t *H;    // The heap
    hidx_t *I;    // Index of element in heap.
};


DPQ_t *DPQ_new(size_t N) {
   // Allocate the new priority queue
   DPQ_t *new_dpq = malloc(sizeof(DPQ_t));

   // Set the heap size to 0
   new_dpq->heap_size = 0;

   // Assign the number of elements
   new_dpq->N = N;

   // Allocate the memory for each array
   new_dpq->D = malloc(N * sizeof(weight_t));
   new_dpq->H = malloc(N * sizeof(node_t));
   new_dpq->I = malloc(N * sizeof(hidx_t));

   // Set the priority of the nodes to infinite and the nodes to invalid
   for (int index = 0; index < N; index++)
   {
      new_dpq->D[index] = weight_inf();
      new_dpq->H[index] = INVALID_NODE;
      new_dpq->I[index] = INVALID_HIDX;
   } //for

   // Initialize the structure. Allocate enough space for all arrays to hold N elements

   // Return the new priority queue
   return new_dpq;
}


void DPQ_delete(DPQ_t *pq) {
   // Free everything in pq
   free(pq->D);
   free(pq->H);
   free(pq->I);

   // Free pq itself
   free(pq);
}


weight_t *DPQ_dist_free(DPQ_t *pq) {
   weight_t *res=pq->D;

   // Free everything but the D-array
   free(pq->H);
   free(pq->I);
   free(pq);

   // Return the D-Array
   return res;
}


weight_t DPQ_prio(DPQ_t const *pq, node_t u) {
   // Its a good idea to use assertions to ensure the preconditions!
   // This makes programs more likely to fail close to the reason of the error, rarher than later, when the (indirect) consequences become apparent.
   assert(u<pq->N);

   return pq->D[u]; /// *** return priority of node u
}


/**
 * It is a good idea to wrap repetitive patterns into an own function (though, in C, it can be a pain).
 * This reduces the redundancy, and the chance of accidentally getting one of the repeated versions wrong.
 *
 * In this case, we get the priority associated with a heap index.
 */
weight_t _DPQ_hprio(DPQ_t const *pq, hidx_t i) {
   return DPQ_prio(pq,pq->H[idx_of(i)]);
}


void _DPQ_swap(DPQ_t *pq, hidx_t i, hidx_t j) {
   // Make a temporary node to hold the swapped item
   node_t temporaryiNode = pq->H[idx_of(i)];

   // Swap node i with node j
   pq->I[pq->H[idx_of(i)]] = j;
   pq->H[idx_of(i)] = pq->H[idx_of(j)];

   // Update j with the temporary node for i
   pq->I[pq->H[idx_of(j)]] = i;
   pq->H[idx_of(j)] = temporaryiNode;

   //node_t id = pq->H[idx_of(i)];
   //pq->H[idx_of(i)] = pq->H[idx_of(j)];
   //pq->H[idx_of(j)] = id;

   //hidx_t idx = pq->I[pq->H[idx_of(i)]];
   //pq->I[pq->H[idx_of(i)]] = pq->I[pq->H[idx_of(j)]];
   //pq->I[pq->H[idx_of(j)]] = idx;
}


void _DPQ_sift_up(DPQ_t *pq, hidx_t i) {
   // While a parent exists assign its index to p
   while (has_parent(i))
   {
      hidx_t p = parent(i);

      // Swap if the weight of the current node is less than the parent node
      if (weight_less(_DPQ_hprio(pq, i), _DPQ_hprio(pq, p)))
      {
         // Swap the parent node with the current node
         _DPQ_swap(pq, i, p);
         // Set the current node to the parent
         i = p;
      } //if
      // When the weights are correctly arranged leave the function
      else
      {
         break;
      } //esle
   } //while
}


void _DPQ_sift_down(DPQ_t *pq, hidx_t i) {
   while(true)
   {
      // Get the index of the left and right childs of the current node
      hidx_t leftChild = left(i);
      hidx_t rightChild = right(i);

      //Stop when the end of the queue is reached
      if (idx_of(leftChild) >= pq->heap_size)
      {
         return;
      } //if

      // Sets the smallest child as the left child
      hidx_t smallestChild = leftChild;

      // If the weight of the right child is smaller then set this as smallest
      if (idx_of(rightChild) < pq->heap_size && weight_less(_DPQ_hprio(pq, rightChild), _DPQ_hprio(pq, leftChild)))
      {
         smallestChild = rightChild;
      }

      // If the smallest child has a lower weight than the current node the swap
      if (weight_less(_DPQ_hprio(pq, smallestChild), _DPQ_hprio(pq, i)))
      {
         _DPQ_swap(pq, i, smallestChild);
      } //if
      // If the current node has a smaller weight than that of the smallest child then leave the function
      else
      {
         return;
      } //else

      // Assign the current node to the smallest child
      i = smallestChild;
   } //while
}


bool DPQ_contains(DPQ_t const *pq, node_t u) {
   assert(u<pq->N);

   return !hidx_is_invalid(pq->I[u]); // Use I to check if node is on heap
}


void DPQ_insert(DPQ_t *pq, node_t u, weight_t w) {
   // Assertions to check that the queue doesn't already contain the node
   assert(u < pq->N);
   assert(!DPQ_contains(pq, u));

   // Assign the node and its weight to the last element in the heap
   pq->D[u] = w;
   pq->H[pq->heap_size] = u;

   // Increment the heap size
   pq->heap_size ++;

   // Set the last index of the heap to the node to be added
   hidx_t last;
   last.i = pq->heap_size;
   pq->I[u].i = last.i;

   // Use the sift up function to leave the node in the correct place
   _DPQ_sift_up(pq, pq->I[u]);
}


bool DPQ_is_empty(DPQ_t const *pq) {
   return pq->heap_size==0;
}


node_t DPQ_pop_min(DPQ_t *pq) {
   assert(pq->heap_size>0);
   // Swap first with last element, decrement heap size, then sift down

   // Swap the first and last element
   hidx_t lastIndex;
   lastIndex.i = pq->heap_size;
   _DPQ_swap(pq, hidx_first, lastIndex);

   // Node to be removed
   node_t nodeToPop = pq->H[idx_of(lastIndex)];

   // Decrement the heap size
   pq->heap_size--;

   // Sift down the previous last element
   _DPQ_sift_down(pq, hidx_first);

   // Set the index array to invalid for the popped node
   pq->I[nodeToPop] = INVALID_HIDX;

   // Return the node to be popped
   return nodeToPop;
}


void DPQ_decrease_key(DPQ_t *pq, node_t u, weight_t w) {
  assert(u<pq->N);
  assert(DPQ_contains(pq,u));
  assert(weight_less(w, pq->D[u]));

  // Pass the weight to u, get the index and sift down
  pq->D[u] = w;
  hidx_t i = pq->I[u];
  _DPQ_sift_up(pq, i);
}
