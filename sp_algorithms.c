#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <assert.h>

#include "graph.h"
#include "pq.h"
#include "sp_algorithms.h"




sssp_result_t *bfs(graph_t const *g, node_t src, node_t dst) {

   // Variable to store the number of edges that have been explored
   unsigned long long stat_edges_explored=0;

   // Initialize the number of nodes, the predecessor and the distance map
   size_t N = graph_get_num_nodes(g);
   node_t *pred = CALLOC(node_t, N);
   weight_t *dist = CALLOC(weight_t, N);

   // Initialize a new priority queue and values
   DPQ_t *pq = DPQ_new(N);
   bool *visitedNodes = CALLOC(bool, N);

   for (int index = 0; index < N; index++)
   {
      pred[index] = INVALID_NODE;
      dist[index] = weight_inf();
      visitedNodes[index] = false;
   } //for

   // Handle the source node
   pred[src] = src;
   dist[src] = weight_zero();
   visitedNodes[src] = true;

   // Insert the source node into the priority queue
   DPQ_insert(pq, src, dist[src]);

   // Variable to control the running of the loop
   bool stopWhile = false;

   // Empty nodes from the PQ until the destination is reached
   while (!DPQ_is_empty(pq) && !stopWhile)
   {
      // Pop the minimum node to be used for checking
      node_t currentNode = DPQ_pop_min(pq);

      // Check nodes with edges to the current node and add them to the priority queue
      for (edge_tgt_t const *tgt = graph_succs_begin(g, currentNode); tgt!=graph_succs_end(g, currentNode); tgt++)
      {
         // Node discovered from the current node
         node_t discoveredNode = tgt->v;

         // Check if the discovered node has been visisted
         if (!visitedNodes[discoveredNode])
         {
            // Set the node to visited and update the distance
            visitedNodes[discoveredNode] = true;

            pred[discoveredNode] = currentNode;
            dist[discoveredNode] = weight_add(dist[currentNode], weight_of_int(1));

            // Add the target node to the priority queue
            DPQ_insert(pq, discoveredNode, dist[discoveredNode]);

            // If the discovered node is the destination then stop
            if (discoveredNode == dst)
            {
               stopWhile = true;
               break;
            }
         } //if

         // Increment the number of nodes relaxed
         stat_edges_explored++;

      } //for
   } //while

   // Free the pq and visited nodes array
   free(visitedNodes);
   DPQ_delete(pq);

   // Return the result of the search
   return sssp_result_new(N,src,dst,false,pred,dist,stat_edges_explored);
}





sssp_result_t *bellman_ford(graph_t const *g, node_t src) {

   // Variable to store the number of edges that have been explored
   unsigned long long stat_edges_explored=0;

   // Initialize the number of nodes, the predecessor and the distance map
   size_t N = graph_get_num_nodes(g);
   node_t *pred = CALLOC(node_t, N);
   weight_t *dist = CALLOC(weight_t, N);

   // If in a round nothing changes we can stop
   bool changeMade = false;

   // Initialize the predecessor and distance map
   for (int index = 0; index < N; index++)
   {
      pred[index] = INVALID_NODE;
      dist[index] = weight_inf();
   } //for

   // Handle the source node
   pred[src] = src;
   dist[src] = weight_zero();

   // Repeat at most (N-1) times
   for (int index = 0; index < N; index++)
   {
      // At the start no changes have been made
      changeMade = false;

      // For each node
      for (int nodeIndex = 0; nodeIndex < N; nodeIndex++)
      {
         // For each edge
         for (edge_tgt_t const *tgt = graph_succs_begin(g, nodeIndex); tgt!=graph_succs_end(g, nodeIndex); ++tgt)
         {
            // Assign the node and weights
            node_t node = tgt->v;
            weight_t weight = tgt->w;

            // Relax edges
            if (weight_less(weight_add(dist[nodeIndex], weight), dist[node]))
            {
               // Update the weight and predecessor maps
               dist[node] = weight_add(dist[nodeIndex], weight);
               pred[node] = nodeIndex;

               // A change has now been made
               changeMade = true;
            } //if

            // Increment the number of explored nodes
            stat_edges_explored++;

         } //for(edges)
      } //for(nodes)

      // If no changes have been made we know the algorithm is complete
      if (!changeMade)
      {
         break;
      } //if

   } //for(N-1)

   // Check for negative weight cycles (negative weights allowed)
   bool negativeWeightCycle = false;

   // For each node
   for (int nodeIndex = 0; nodeIndex < N; nodeIndex++)
   {
      for (edge_tgt_t const *tgt = graph_succs_begin(g, nodeIndex); tgt!=graph_succs_end(g, nodeIndex); ++tgt)
      {
         // Assign the node and the weight
         node_t node = tgt->v;
         weight_t weight = tgt->w;

         // If the sum of the weight and the distance is less than the distance from the edge
         // then we have a negative weight cycle
         if (weight_less(weight_add(dist[nodeIndex], weight), dist[node]))
         {
            // We have a negative weight cycle so we should set the edge to infinite
            negativeWeightCycle = true;
            dist[node] = weight_neg_inf();
         } //if
      } //for(edges)
   } //for(nodes)

   // Return the result
   return sssp_result_new(N, src, INVALID_NODE, negativeWeightCycle, pred, dist, stat_edges_explored);
}




sssp_result_t *dijkstra(graph_t const *g, node_t src, node_t dst) {

   // Variable to store the number of edges that have been explored
   unsigned long long stat_edges_explored=0;

   // Initialize the number of nodes, the predecessor and the distance map
   size_t N = graph_get_num_nodes(g);
   node_t *pred = CALLOC(node_t, N);
   weight_t *dist = CALLOC(weight_t, N);

   // Initialize a new priority queue and values for the predecessor and distance map
   DPQ_t *pq = DPQ_new(N);

   for (int index = 0; index < N; index++)
   {
      pred[index] = INVALID_NODE;
      dist[index] = weight_inf();
      DPQ_insert(pq, index, dist[index]);
   } //for

   // Handle the source node and decrease the key
   pred[src] = src;
   dist[src] = weight_zero();
   DPQ_decrease_key(pq, src, dist[src]);

   // Run for all the nodes in the priority queue
   while (!DPQ_is_empty(pq))
   {
      // The node with the minimum estimate is used as the current node
      node_t currentNode = DPQ_pop_min(pq);

      // Check all the edges for the current node
      for (edge_tgt_t const *tgt = graph_succs_begin(g, currentNode); tgt!=graph_succs_end(g, currentNode); ++tgt)
      {
         // Assign the node and the weight
         node_t node = tgt->v;
         weight_t weight = tgt->w;

         // Relax the edge
         if (weight_less(weight_add(dist[currentNode], weight), dist[node]))
         {
            pred[node] = currentNode;
            dist[node] = weight_add(dist[currentNode], weight);

            // Decrease the key
            DPQ_decrease_key(pq, node, weight_add(dist[currentNode], weight));
         } //if

         // Another edge has been explored
         stat_edges_explored++;
      } //for
   } //while

   // Free the priority queue
   DPQ_delete(pq);

   // Return the result
   return sssp_result_new(N, src, INVALID_NODE, false, pred, dist, stat_edges_explored);

}




sp_result_t *astar_search(graph_t const *g, node_t src, node_t dst, weight_t const* h) {

   // Variable to store the number of edges that have been explored
   unsigned long long stat_edges_explored=0;

   // Initialize the number of nodes, the predecessor and the distance map
   size_t N = graph_get_num_nodes(g);
   node_t *pred = CALLOC(node_t, N);
   weight_t *dist = CALLOC(weight_t, N);

   // Initialize a new priority queue and values for the predecessor and distance map
   DPQ_t *pq = DPQ_new(N);

   for (int index = 0; index < N; index++)
   {
      pred[index] = INVALID_NODE;
      dist[index] = weight_inf();
      DPQ_insert(pq, index, dist[index]);
   } //for

   // Handle the source node and decrease the key
   pred[src] = src;
   dist[src] = weight_zero();
   DPQ_decrease_key(pq, src, dist[src]);

   // Run until the priotiy queue is empty
   while (!DPQ_is_empty(pq))
   {
      // The current node is that with the minimum estimate
      node_t currentNode = DPQ_pop_min(pq);

      // For all the edges of the current node
      for (edge_tgt_t const *tgt = graph_succs_begin(g, currentNode); tgt!=graph_succs_end(g, currentNode); ++tgt)
      {
         // Assign the node and the weight
         node_t node = tgt->v;
         weight_t weight = tgt->w;

         // Relax the edge
         if (weight_less(weight_add(dist[currentNode], weight), dist[node]))
         {
            pred[node] = currentNode;
            dist[node] = weight_add(dist[currentNode], weight);

            // Decrease the key with heuristics
            DPQ_decrease_key(pq, node, weight_add(weight_add(dist[currentNode], weight), h[node]));
         } //if

         // If the node is the destination then exit
         if (node == dst)
         {
            break;
         } //if

         // Another edge has been explored
         stat_edges_explored++;
      } //for
   } //while

   // Free the priority queue
   DPQ_delete(pq);

   // Return the result
   return sssp_to_sp_result(sssp_result_new(N, src, dst, false, pred, dist, stat_edges_explored),dst);
}























//
