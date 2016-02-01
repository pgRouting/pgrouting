#ifndef MY_DIJKSTRA_H_
#define MY_DIJKSTRA_H_


#include <deque>
#include <vector>
#include <set>
#include <iostream>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include "./myGraph.hpp"
#include "./structs.h"
#include "./graphMinimizer.hpp"
using namespace std;
using namespace boost;

template < class G >
class My_dijkstra
:public Graph_Minimizer< G > {
  public:
  typedef deque<Edge> unpackedPath;
  typedef typename boost::graph_traits < G >::edge_descriptor E;
  //constrictor for this class which inherits the base graph
  explicit My_dijkstra(graphType gtype, const int64_t initial_size)
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
  for (int64_t i = 0; i < this->predecessors.size(); ++i)
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
  for (int64_t i = 0; i < this->distances.size(); ++i)
  {
    cout << i << " ---> " << this->distances[i] << endl;
  }
  
}


//creates a linked list of edges in an order along with the cost
typedef typename boost::graph_traits < G >::vertex_descriptor V;
void get_path(V source,V target,Edge **path,int64_t &size)
{
  int64_t path_size=0;

  if (source==target)
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
    int64_t temp_size=path_size-1;
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
    int64_t temp_size=path_size-1;
    temp=target;
    int64_t sid=-1,tid=-1,etype=0,eid=-1;
    while(this->reduced_graph->predecessors[temp]!=temp)
    {
      V v1=this->reduced_graph->predecessors[temp];
      V v2=temp;
      this->reduced_graph->get_vertex_id(this->reduced_graph->predecessors[temp],sid);
      this->reduced_graph->get_vertex_id(temp,tid);

      if (edge(v1, v2,this->reduced_graph->graph).second)
      {
        E e=edge(v2, v1,this->reduced_graph->graph).first;
        etype=this->reduced_graph->graph[e].type;
        eid=this->reduced_graph->graph[e].id;
      }
      
      
      (*path)[temp_size].id=eid;
      (*path)[temp_size].source=sid;
      (*path)[temp_size].target=tid;
      (*path)[temp_size].cost=this->reduced_graph->distances[temp]-this->reduced_graph->distances[this->predecessors[temp]];
      (*path)[temp_size].type=etype;
       //cout <<"yo," << "id:- " << e1.id << " src:- " << e1.source << " dest:- " << e1.target << " cost " << e1.cost << " type "<< e1.type << endl;
      temp_size--;
      temp=this->reduced_graph->predecessors[temp];
    }
  }
}
int64_t get_psuedo_edge_count(Edge **path,int64_t size)
{
  int64_t count=0;
  for (int64_t i = 0; i < size; ++i)
  {
    if ((*path)[i].type!=0)
    count++;
  }
  return count;
}
void dijkstra_on_contracted(int64_t src,int64_t dest,Edge **path,int64_t &size)
{
  Edge *mainPath=NULL;
  unpackedPath srcPath,targetPath;
  int64_t closest_src,closest_target;
  int64_t src_size,target_size;
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
int64_t num_psuedo_edges=get_psuedo_edge_count(&mainPath,size);

int64_t total_size=size+src_size+target_size+num_psuedo_edges;
cout << "total path size is " << total_size << endl;
*path=(Edge*)malloc(total_size*sizeof(Edge));

int64_t temp_size=0;
for (int64_t i = 0; i < src_size; ++i)
{
  (*path)[i].id=srcPath[i-temp_size].id;
  (*path)[i].source=srcPath[i-temp_size].source;
  (*path)[i].target=srcPath[i-temp_size].target;
  (*path)[i].cost=srcPath[i-temp_size].cost;
}
temp_size=src_size;
int64_t ind=src_size;
for (int64_t i = src_size; i < src_size+size; ++i)
{
  if ((mainPath)[ind].type==0)
  {
    (*path)[ind].id=(mainPath)[i-temp_size].id;
    (*path)[ind].source=(mainPath)[i-temp_size].source;
    (*path)[ind].target=(mainPath)[i-temp_size].target;
    (*path)[ind++].cost=(mainPath)[i-temp_size].cost;
  }
  else
  {
    int64_t sid=(mainPath)[ind].source;
    int64_t tid=(mainPath)[ind].target;
    int64_t id=(mainPath)[ind].id;
    //cout << "id:- " << id << endl;
    pair<int64_t,int64_t> pedges=this->psuedoEdges[id];
    //cout << "id1:- " << pedges.first << " id2:- " << pedges.second << endl;
    Edge e1=this->reduced_graph->removed_edges[pedges.first];
    Edge e2=this->reduced_graph->removed_edges[pedges.second];
    //cout << "Psuedo" << endl;
  //cout << "id:- " << e1.id << " src:- " << e1.source << " dest:- " << e1.target << " cost " << e1.cost << " type "<< e1.type << endl;
    // 1st edge
    (*path)[ind].id=e1.id;
    (*path)[ind].source=e1.source;
    (*path)[ind].target=e1.target;
    (*path)[ind++].cost=e1.cost;
    //cout << "id:- " << e2.id << " src:- " << e2.source << " dest:- " << e2.target << " cost " << e2.cost << " type "<< e2.type << endl;
    //2nd edge
    (*path)[ind].id=e2.id;
    (*path)[ind].source=e2.source;
    (*path)[ind].target=e2.target;
    (*path)[ind++].cost=e2.cost;
  }
}
temp_size=src_size+size;
for (int64_t i = src_size+size; i < total_size-num_psuedo_edges; ++i)
{
  (*path)[ind].id=targetPath[i-temp_size].id;
  (*path)[ind].source=targetPath[i-temp_size].source;
  (*path)[ind].target=targetPath[i-temp_size].target;
  (*path)[ind++].cost=targetPath[i-temp_size].cost;
}
size=total_size;
}

void print_reduced_predecessors()
{
  //typedef typename boost::graph_traits < G >::vertex_descriptor V;
  std::cout << "reduced predecessors :" << endl;
  for (int64_t i = 0; i < this->reduced_graph->predecessors.size(); ++i)
  {
    int64_t src,dest;
    this->reduced_graph->get_vertex_id(i,src);
    this->reduced_graph->get_vertex_id(this->reduced_graph->predecessors[i],dest);
    cout << src << " ---> " << dest << endl;
  }
  
}


};
#endif