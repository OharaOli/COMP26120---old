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

  //Initial Value as no nodes are visited
  unsigned long long stat_edges_explored=0;

  //Initialization for predescor map, distance map and the N
  size_t N = graph_get_num_nodes(g);
  node_t *pred = CALLOC(node_t,N);
  weight_t *dist = CALLOC(weight_t,N);

  //boolean array to keep track of which nodes have been visited
  bool *visited = CALLOC(bool,N);

  //Initialize a new priority queue
  DPQ_t *pq = DPQ_new(N);

  //Initialize all values
  for(int i=0; i<N; i++){
    visited[i] = false;
    pred[i] = INVALID_NODE;
    dist[i] = weight_inf();
  }

  //For src node it has been seen, the distance to it is zero and it has no predesccors
  //except iteslf
  visited[src] = true;
  dist[src] = weight_zero();
  pred[src] = src;

  bool stop = false;

  //insertes the src into the PQ
  DPQ_insert(pq, src, dist[src]);

  //empties the PQ till destination node is found
  while (!DPQ_is_empty(pq) && !stop) {

    //Pops the node which will be used for checking
    node_t u = DPQ_pop_min(pq);

    //All adjecent nodes are checked with v as the node
    for (edge_tgt_t const *tgt = graph_succs_begin(g,u); tgt!=graph_succs_end(g,u); ++tgt){
      node_t v = tgt->v;

      //checks if the node has been visited
      if(!visited[v]){
        visited[v] = true;

        //sets the distance as distance from u + 1 as a count and the predesscor map to the initial node u
        pred[v] = u;
        dist[v] = weight_add(dist[u],weight_of_int(1));

        //inserts v into PQ and increments the number of num of nodes relaxed
        DPQ_insert(pq,v,dist[v]);
        //exists the while loop when destination is found
        if(v==dst){
          break;
          stop = true;
        }
      }//if
      stat_edges_explored++;
    }//for
  }//while

  //deletes the PQ after use as it is not passed
  DPQ_delete(pq);
  free(visited);

  return sssp_result_new(N,src,dst,false,pred,dist,stat_edges_explored);
}//bfs()

sssp_result_t *bellman_ford(graph_t const *g, node_t src) {

  //Variables to keep track of predecessor map, number of nodes in graph and distance map
  size_t N = graph_get_num_nodes(g);
  node_t *pred = CALLOC(node_t,N);
  weight_t *dist = CALLOC(weight_t,N);

  //var to hold the number of explored edges
  unsigned long long stat_edges_explored=0;

  //check for early stopping
  bool madeChange = false;

  //Initialization for predessor map and distance map including src node
  for(int i=0; i<N; i++){
    dist[i] = weight_inf();
    pred[i] = INVALID_NODE;
  }
  dist[src] = weight_zero();
  pred[src] = src;


  //Run upto N-1 times
  for(int i=0; i<N; i++){
    //initially no change made
    madeChange = false;

    //run  through all nodes
    for(int u=0; u<N; u++){

      //run through all edges in nodes
      for(edge_tgt_t const *tgt = graph_succs_begin(g,u); tgt!=graph_succs_end(g,u); ++tgt){

        //node and weight are assigned
        node_t n = tgt->v;
        weight_t w = tgt->w;

        //relax edges
        if( weight_less( weight_add( dist[u],w ),dist[n] ) ){
          dist[n] = weight_add(dist[u],w);
          pred[n] = u;
          //change has been made
          madeChange = true;
          //increment the number of explored nodes
        }//if
        stat_edges_explored++;
      }//for all
    }//inner for loop
    if(!madeChange)
        break;
  }//outer for loop

  //check for negative cycles
  bool has_negative_weight_cycle = false;

  //for all nodes
  for(int i=0; i<N; i++){

    //for all edges of nodes
    for(edge_tgt_t const *tgt = graph_succs_begin(g,i); tgt!=graph_succs_end(g,i); ++tgt){
      weight_t w = tgt->w;
      node_t v = tgt->v;

      //check if the sum of weight and distance  is less than distance from edge
      //then it has negative cycles and sets those edges to - inf because they should be unreachable
      if(weight_less(weight_add(dist[i],w),dist[v])){
        dist[v] = weight_neg_inf();
        has_negative_weight_cycle = true;
      }//if
    }//for edges
  }//for nodes

  return sssp_result_new(N, src, INVALID_NODE, has_negative_weight_cycle, pred, dist, stat_edges_explored);
}

sssp_result_t *dijkstra(graph_t const *g, node_t src, node_t dst) {

  //Variables to keep track of predecessor map, number of nodes in graph and distance map
  size_t N = graph_get_num_nodes(g);
  node_t *pred = CALLOC(node_t,N);
  weight_t *dist = CALLOC(weight_t,N);

  //Initialize a priotiy queue
  DPQ_t *pq = DPQ_new(N);

  //var to keep track of how many edges have been exlpored
  unsigned long long stat_edges_explored=0;

  //Initial values
  for(int i=0; i<N; i++){
    pred[i] = INVALID_NODE;
    dist[i] = weight_inf();
    DPQ_insert(pq,i,dist[i]);
  }//for
  dist[src] = weight_zero();
  pred[src] = src;

  //Decrease key for src
  DPQ_decrease_key(pq,src,dist[src]);

  //run till pq is empty
  while (!DPQ_is_empty(pq)) {
    //Pop element from pq
    node_t u = DPQ_pop_min(pq);

    //All edges are checked
    for (edge_tgt_t const *tgt = graph_succs_begin(g,u); tgt!=graph_succs_end(g,u); ++tgt){

      //Gets node and it's weight
      node_t v = tgt->v;
      weight_t w = tgt->w;

      //Relax
      if( weight_less( weight_add( dist[u],w ),dist[v] ) ){
        dist[v] = weight_add(dist[u],w);
        pred[v] = u;

        //Decrease key for old node
        DPQ_decrease_key(pq,v,weight_add(dist[u],w));
      }//if
      stat_edges_explored++;
    }//for
  }//while

  //delete existing pq for valgrind errors
  DPQ_delete(pq);

  return sssp_result_new(N, src, INVALID_NODE, false, pred, dist, stat_edges_explored);
}

sp_result_t *astar_search(graph_t const *g, node_t src, node_t dst, weight_t const* h) {

  //Initial variables needed
  size_t N = graph_get_num_nodes(g);
  node_t *pred = CALLOC(node_t,N);
  weight_t *dist = CALLOC(weight_t,N);

  //Initial PQ is called
  DPQ_t *pq = DPQ_new(N);

  //no nodes visited initially
  unsigned long long stat_edges_explored=0;

  //Initial values
  for(int i=0; i<N; i++){
    pred[i] = INVALID_NODE;
    dist[i] = weight_inf();
    DPQ_insert(pq,i,dist[i]);
  }
  dist[src] = weight_zero();
  pred[src] = src;

  //Decrease the key of src in PQ
  DPQ_decrease_key(pq,src,dist[src]);

  //check if pq is not empty
  while (!DPQ_is_empty(pq)) {

    //pops a node from PQ
    node_t u = DPQ_pop_min(pq);

    //runs through all edges of node
    for (edge_tgt_t const *tgt = graph_succs_begin(g,u); tgt!=graph_succs_end(g,u); ++tgt){

      //get node and its weight
      node_t v = tgt->v;
      weight_t w = tgt->w;

      //relaxes edge on node
      if( weight_less( weight_add(dist[u],w), dist[v] )){
        dist[v] = weight_add(dist[u],w);
        pred[v] = u;
        //decreases key with new montone heuristics
        DPQ_decrease_key(pq,v,weight_add(weight_add(dist[u],w),h[v])) ;
      }//if
      if(v==dst)
        break;
      stat_edges_explored++;
    }//for
  }//while

  //deletes PQ for no valgrind issues
  DPQ_delete(pq);

  return sssp_to_sp_result(sssp_result_new(N, src, dst, false, pred, dist, stat_edges_explored),dst);
}
