
#ifndef MY_ASTAR_H_
#define MY_ASTAR_H_


#include <deque>
#include <vector>
#include <set>
#include <iostream>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include "../../common/src/myGraph.hpp"
#include "../../common/src/structs.h"
#include "../../contraction/src/graphMinimizer.hpp"
#include <boost/graph/astar_search.hpp>

#include <cmath>    // for sqrt
using namespace std;
using namespace boost;
#define MAX_NODES 100000000


struct found_goal {}; // exception for termination

// visitor that terminates when we find the goal
template <class Vertex>
class my_astar_goal_visitor : public boost::default_astar_visitor
{
public:
  my_astar_goal_visitor(Vertex goal) : m_goal(goal) {}
      template <class Graph>
  void examine_vertex(Vertex u, Graph& g) {
    if(u == m_goal)
      throw found_goal();
  }
private:
  Vertex m_goal;
};

// Heuristic function which tells us how far the current node is
// from the target node.
//
// (|dx|+|dy|)/2 is used currently.
template <class Graph, class CostType>
class mean_heuristic : public astar_heuristic<Graph, CostType>
{
public:
  typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
  mean_heuristic(Graph& g, Vertex goal):m_g(g), m_goal(goal){}
  CostType operator()(Vertex u)
  {
    CostType dx = m_g[m_goal].x - m_g[u].x;
    CostType dy = m_g[m_goal].y - m_g[u].y;
    return (::fabs(dx)+::fabs(dy))/2;
  } 
private:
  Graph& m_g;
  Vertex m_goal;
};

template <class Graph, class CostType>
class euclidean_heuristic : public astar_heuristic<Graph, CostType>
{
public:
  typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
  euclidean_heuristic(Graph& g, Vertex goal):m_g(g), m_goal(goal){}
  CostType operator()(Vertex u)
  {
    CostType dx = m_g[m_goal].x - m_g[u].x;
    CostType dy = m_g[m_goal].y - m_g[u].y;
    return sqrt(dx * dx + dy * dy);
  } 
private:
  Graph& m_g;
  Vertex m_goal;
};


template < class G >
class My_Astar
:public Graph_Minimizer< G > {
public:

  typedef deque<Edge> unpackedPath;
  typedef typename boost::graph_traits < G >::vertex_descriptor V;
  typedef typename boost::graph_traits < G >::edge_descriptor E;
  typedef typename boost::graph_traits < G >::vertex_iterator V_i;
  typedef typename boost::graph_traits < G >::edge_iterator E_i;
  typedef typename boost::graph_traits < G >::out_edge_iterator EO_i;
  typedef typename boost::graph_traits < G >::in_edge_iterator EI_i;
  typedef typename std::map< int64_t, V > id_to_V;
  typedef typename std::map< V, int64_t > V_to_id;
  typedef typename std::map< int64_t, std::vector<V> > degree_to_V;
  typedef typename std::map< int64_t, std::vector<V> >::iterator degree_to_V_i;
  typedef typename std::vector<V>::iterator Q_i;
  typedef typename id_to_V::const_iterator LI;
  typedef typename V_to_id::const_iterator RI;
  //constrictor for this class which inherits the base graph
  explicit My_Astar(graphType gtype, const int initial_size)
  :Graph_Minimizer<G>(gtype, initial_size) {}

  //initializes the graph with the given edges
  void
  initialize_graph(Edge *data_edges, int64_t count) {
    this->initialize_graph_minimizer(data_edges, count);
    
  }


  // adds edge to the graph and also to the vertex maps respectively
  void graph_add_edge(const Edge &edge ) {
    bool inserted;
    LI vm_s, vm_t;
    E e;

    vm_s = this->id_V_map.find(edge.source);
    if (vm_s == this->id_V_map.end()) {
      this->id_V_map[edge.source]=  num_vertices;
      this->V_id_map[this->num_vertices++] = edge.source;
      vm_s = this->id_V_map.find(edge.source);
      this->graph[vm_s->second].id=edge.source;
      this->graph[vm_s->second].contractions=0;
      this->graph[vm_s->second].x=edge.s_x;
      this->graph[vm_s->second].y=edge.s_y;
    }

    vm_t = this->id_V_map.find(edge.target);
    if (vm_t == this->id_V_map.end()) {
      this->id_V_map[edge.target]=  num_vertices;
      this->V_id_map[this->num_vertices++] = edge.target;
      vm_t = this->id_V_map.find(edge.target);
      this->graph[vm_t->second].id=edge.target;
      this->graph[vm_t->second].contractions=0;
      this->graph[vm_s->second].x=edge.t_x;
      this->graph[vm_s->second].y=edge.t_y;
    }

    if (edge.cost >= 0) {
      boost::tie(e, inserted) =
      boost::add_edge(vm_s->second, vm_t->second,this->graph);
      this->graph[e].cost = edge.cost;
      this->graph[e].id = edge.id;
      this->graph[e].source=edge.source;
      this->graph[e].target=edge.target;
      this->graph[e].s_x=edge.s_x;
      this->graph[e].s_y=edge.s_y;
      this->graph[e].t_x=edge.t_x;
      this->graph[e].t_y=edge.t_y;
      //graph[e].type=edge.type;
      this->graph[e].type=0;
    }

    if (edge.revcost >= 0) {
      boost::tie(e, inserted) =
      boost::add_edge(vm_t->second, vm_s->second, this->graph);
      this->graph[e].cost = edge.revcost;
      this->graph[e].id = edge.id;
      this->graph[e].source=edge.target;
      this->graph[e].target=edge.source;
      this->graph[e].s_x=edge.s_x;
      this->graph[e].s_y=edge.s_y;
      this->graph[e].t_x=edge.t_x;
      this->graph[e].t_y=edge.t_y;
    }
  }


  // call the astar algorithm and returns the path and its size
  void perform_astar(int64_t src,int64_t dest,Edge **path,int64_t& size)
  {
    this->predecessors.clear();
    this->distances.clear();
    this->predecessors.resize(boost::num_vertices(this->graph));
    this->distances.resize(boost::num_vertices(this->graph));
    typedef typename boost::graph_traits < G >::vertex_descriptor V;
    V source=-1,target=-1;
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
      try
      {
        astar_search(this->graph, source,
          euclidean_heuristic<G, float>(this->graph, target),
          boost::predecessor_map(&this->predecessors[0])
          .weight_map(get(&Edge::cost, this->graph))
          .distance_map(&this->distances[0]).
          visitor(my_astar_goal_visitor<V>(target)));
      }
    catch(found_goal fg) { // found a path to the goal
      get_path(source,target,path,size);  
      return ;
    }
    
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
void astar_on_contracted(int64_t src,int64_t dest,Edge **path,int64_t &size)
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
  try
  {
    astar_search(this->reduced_graph->graph, source,
      euclidean_heuristic<G, float>(this->reduced_graph->graph, target),
      boost::predecessor_map(&this->reduced_graph->predecessors[0])
      .weight_map(get(&Edge::cost, this->reduced_graph->graph))
      .distance_map(&this->reduced_graph->distances[0]).
      visitor(my_astar_goal_visitor<V>(target)));
  }
    catch(found_goal fg) { // found a path to the goal
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
      return;
    }

    



  }
  void print_path(Edge **path,int64_t size)
  {
    cout << "Path for astar......" << endl; 
    for (int i = 0; i < size; ++i)
    {
      cout << "id:- " << (*path)[i].id << " src:- " << (*path)[i].source << " dest:- " << (*path)[i].target << " cost " << (*path)[i].cost << endl;
    }
  }


};
#endif