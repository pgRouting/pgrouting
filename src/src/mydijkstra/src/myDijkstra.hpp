
#ifndef MY_DIJKSTRA_H_
#define MY_DIJKSTRA_H_


#include <deque>
#include <vector>
#include <set>
#include <iostream>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include "../../common/src/baseGraph.hpp"
#include "../../common/src/structs.h"
#include "../../contraction/src/graphMinimizer.hpp"
using namespace std;
using namespace boost;

template < class G >
class My_dijkstra
:public Graph_Minimizer< G > {
public:
  typedef deque<Edge> unpackedPath;
  //constrictor for this class which inherits the base graph
  explicit My_dijkstra(graphType gtype, const int initial_size)
  :Graph_Minimizer<G>(gtype, initial_size) {}

  //initializes the graph with the given edges
  void
  initialize_graph(Edge *data_edges, int64_t count) {
    this->initialize_graph_minimizer(data_edges, count);
    
  }

  // call the dijkstra algorithm and returns the path and its size
  void perform_dijkstra(int64_t src,int64_t dest,Edge **path,int64_t& size)
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
      int64_t src,dest;
      this->get_vertex_id(i,src);
      this->get_vertex_id(this->predecessors[i],dest);
      cout << src << " ---> " << dest << endl;
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
  void get_path(V source,V target,Edge **path,int64_t &size)
  {
    int64_t path_size=0;

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
      cout << "Path is of size " << path_size << endl;
      (*path)=(Edge *)malloc(sizeof(Edge)*path_size);
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

  void get_reduced_path(V source,V target,Edge **path,int64_t &size)
  {
    int64_t path_size=0;

    if (this->reduced_graph->predecessors[target]==target)
    {

      cout << "No path" << endl;
      return;
    }
    else
    {
      V temp=target;
      while(this->reduced_graph->predecessors[temp]!=temp)
      {
        path_size++;
        temp=this->reduced_graph->predecessors[temp];
      }
      size=path_size;
      cout << "Path is of size " << path_size << endl;
      (*path)=(Edge *)malloc(sizeof(Edge)*path_size);
      int temp_size=path_size-1;
      temp=target;
      int64_t sid=-1,tid=-1;
      while(this->reduced_graph->predecessors[temp]!=temp)
      {
        this->reduced_graph->get_vertex_id(this->reduced_graph->predecessors[temp],sid);
        this->reduced_graph->get_vertex_id(temp,tid);
        (*path)[temp_size].id=temp_size;
        (*path)[temp_size].source=sid;
        (*path)[temp_size].target=tid;
        (*path)[temp_size].cost=this->reduced_graph->distances[temp]-this->reduced_graph->distances[this->predecessors[temp]];
       // cout << "id:- " << (*path)[temp_size].id << " src:- " << (*path)[temp_size].source << " dest:- " << (*path)[temp_size].target << " cost " << (*path)[temp_size].cost << endl;
        temp_size--;
        temp=this->reduced_graph->predecessors[temp];
      }
    }
  }
  void dijkstra_on_contracted(int64_t src,int64_t dest,Edge **path,int64_t &size)
  {
    Edge *mainPath=NULL;
    unpackedPath srcPath,targetPath;
    int64_t closest_src,closest_target;
    int src_size,target_size;
    this->find_source_vertex(src,closest_src,srcPath);
    this->find_target_vertex(dest,closest_target,targetPath);
    src_size=srcPath.size();
    target_size=targetPath.size();
    cout << "source:- " << closest_src << ", target:- " << closest_target << endl;
    cout << "source size: " << src_size << endl;
    cout << "target size: " << target_size << endl;
    this->reduced_graph->predecessors.clear();
    this->reduced_graph->distances.clear();
    this->reduced_graph->predecessors.resize(boost::num_vertices(this->reduced_graph->graph));
    this->reduced_graph->distances.resize(boost::num_vertices(this->reduced_graph->graph));
    typedef typename boost::graph_traits < G >::vertex_descriptor V;
    V source,target;
    if(!this->reduced_graph->get_vertex_descriptor(closest_src,source))
    {
      cout << "Source vertex not found" <<  endl;
      return;
    }
    if(!this->reduced_graph->get_vertex_descriptor(closest_target,target))
    {
      cout << "Target vertex not found" <<  endl;
      return;
    }

      //boost dijkstra_shortest path algorithm 
    dijkstra_shortest_paths(this->reduced_graph->graph, source,
      boost::predecessor_map(&this->reduced_graph->predecessors[0])
      .weight_map(get(&Edge::cost, this->reduced_graph->graph))
      .distance_map(&this->reduced_graph->distances[0]));

    get_reduced_path(source,target,&mainPath,size);


    int total_size=size+src_size+target_size;
    cout << "total path size is " << total_size << endl;
    *path=(Edge*)malloc(total_size*sizeof(Edge));
    int temp_size=0;
    for (int i = 0; i < src_size; ++i)
    {
      (*path)[i].id=srcPath[i-temp_size].id;
      (*path)[i].source=srcPath[i-temp_size].source;
      (*path)[i].target=srcPath[i-temp_size].target;
      (*path)[i].cost=srcPath[i-temp_size].cost;
    }
    temp_size=src_size;
    for (int i = src_size; i < src_size+size; ++i)
    {
      (*path)[i].id=(mainPath)[i-temp_size].id;
      (*path)[i].source=(mainPath)[i-temp_size].source;
      (*path)[i].target=(mainPath)[i-temp_size].target;
      (*path)[i].cost=(mainPath)[i-temp_size].cost;
    }
    temp_size=src_size+size;
    for (int i = src_size+size; i < total_size; ++i)
    {
      (*path)[i].id=targetPath[i-temp_size].id;
      (*path)[i].source=targetPath[i-temp_size].source;
      (*path)[i].target=targetPath[i-temp_size].target;
      (*path)[i].cost=targetPath[i-temp_size].cost;
    }
    size=total_size;
  }

  void print_reduced_predecessors()
  {
    //typedef typename boost::graph_traits < G >::vertex_descriptor V;
    std::cout << "reduced predecessors :" << endl;
    for (int i = 0; i < this->reduced_graph->predecessors.size(); ++i)
    {
      int64_t src,dest;
      this->reduced_graph->get_vertex_id(i,src);
      this->reduced_graph->get_vertex_id(this->reduced_graph->predecessors[i],dest);
      cout << src << " ---> " << dest << endl;
    }
    
  }


};
#endif