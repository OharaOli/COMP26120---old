#!/bin/sh

make clean; make

 ./sp pq
 ./sp bfs
 ./sp bellman-ford
 ./sp bellman-ford-neg
 ./sp dijkstra
 ./sp dijkstra astar
