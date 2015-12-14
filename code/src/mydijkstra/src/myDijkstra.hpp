
#ifndef MY_DIJKSTRA_H_
#define MY_DIJKSTRA_H_


#include <deque>
#include <vector>
#include <set>
#include <iostream>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include "../../common/src/myGraph.hpp"
#include "../../common/src/structs.h"
using namespace std;
using namespace boost;
template < class G >
class My_dijkstra
:public My_base_graph<G> {
public:
  //constrictor for this class which inherits the base graph
  explicit My_dijkstra(graphType gtype, const int initial_size)
  :My_base_graph<G>(gtype, initial_size) {}

  //initializes the graph with the given edges
  void
  initialize_graph(Edge *data_edges, int64_t count) {
    this->insert_data(data_edges, count);
  }

  // call the dijkstra algorithm and returns the path and its size
  void perform_dijkstra(int src,int dest,Edge **path,int& size)
  {
    this->predecessors.clear();
    this->distances.clear();
    this->predecessors.resize(boost::num_vertices(this->graph));
    this->distances.resize(boost::num_vertices(this->graph));
    typedef typename boost::graph_traits < G >::vertex_descriptor V;
    V source,target;
    if(!this->get_vertex_descriptor(src,source))
    {
      cout << "Source vertex not found" <<  endl;
    }
    if(!this->get_vertex_descriptor(dest,target))
    {
      cout << "Target vertex not found" <<  endl;
    }
    else
    {
      //boost dijkstra_shortest path algorithm 
      dijkstra_shortest_paths(this->graph, source,
          boost::predecessor_map(&this->predecessors[0])
          .weight_map(get(&Edge::cost, this->graph))
          .distance_map(&this->distances[0]));
      get_path(source,target,path,size);
    }


  }


  //prints the predecessors of each vertex
  void print_predecessors()
  {
    //typedef typename boost::graph_traits < G >::vertex_descriptor V;
    std::cout << "predecessors :" << endl;
    for (int i = 0; i < this->predecessors.size(); ++i)
    {
      cout << i << " ---> " << this->predecessors[i] << endl;
    }
    
  }

  //prints the distance from the source to all the other vertices
  void print_distances()
  {
    std::cout << "distances :" << endl;
    for (int i = 0; i < this->distances.size(); ++i)
    {
      cout << i << " ---> " << this->distances[i] << endl;
    }
    
  }


  //creates a linked list of edges in an order along with the cost
  typedef typename boost::graph_traits < G >::vertex_descriptor V;
  void get_path(V source,V target,Edge **path,int &size)
  {
    int path_size=0;
    if (this->predecessors[target]==target)
    {
      return;
    }
    else
    {
      V temp=target;
      while(this->predecessors[temp]!=temp)
      {
        path_size++;
        temp=this->predecessors[temp];
      }
      size=path_size;
      *path=(Edge*)malloc(path_size*sizeof(Edge));
      int temp_size=path_size-1;
      temp=target;
      int64_t sid=-1,tid=-1;
      while(this->predecessors[temp]!=temp)
      {
        this->get_vertex_id(this->predecessors[temp],sid);
        this->get_vertex_id(temp,tid);
        (*path)[temp_size].id=temp_size;
        (*path)[temp_size].source=sid;
        (*path)[temp_size].target=tid;
        (*path)[temp_size].cost=this->distances[temp]-this->distances[this->predecessors[temp]];
       // cout << "id:- " << (*path)[temp_size].id << " src:- " << (*path)[temp_size].source << " dest:- " << (*path)[temp_size].target << " cost " << (*path)[temp_size].cost << endl;
        temp_size--;
        temp=this->predecessors[temp];
      }
    }
  }


};
#endif