
#include "myAstar.hpp"
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include "my_astar.h"

using namespace std;
using namespace boost;
typedef adjacency_list<vecS, vecS, directedS, Vertex,Edge> G;
//calculates the shortest path,returns the pathsize and also the path as an array of edges
int compute_astar(int64_t source,int64_t target,Edge *edges,int64_t num_vertices,int64_t num_edges,Edge **path)
{
  int64_t path_size=0;
  typedef My_Astar<G> Graph;
  Graph g(DIRECTED,num_vertices);
  g.initialize_graph(edges,num_edges);
  g.perform_astar(source,target,path,path_size);
  return path_size;
}