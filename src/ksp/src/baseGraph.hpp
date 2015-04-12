#ifndef BASE_GRAPH_H
#define BASE_GRAPH_H

#include "postgres.h"
#include <boost/config.hpp>

#include <iostream>
#include <iterator>
#include <exception>

#include <stdlib.h>
#include <fstream>

#include <boost/bimap.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>

/****************************************
SIMULATES  PGR_TYPES.H
****************************************/
#include "./pgr_types.h"


/****************************************
SIMULATES A GENERAL GRAPH FILE
****************************************/

void empty_path(pgr_path_t **path, int64_t start_vertex) {
  (*path) = (pgr_path_t *) malloc(sizeof(pgr_path_t));
  (*path)[0].seq  = 1;
  (*path)[0].source  = start_vertex;
  (*path)[0].edge  = -1;
  (*path)[0].cost  = 0;
}

template <class G>
class Pgr_base_graph {
 public:

  typedef typename boost::graph_traits < G >::vertex_descriptor V;
  typedef typename boost::graph_traits < G >::edge_descriptor E ;
  typedef typename boost::graph_traits < G >::edge_iterator E_i;
  typedef typename boost::graph_traits < G >::out_edge_iterator EO_i;
  typedef typename boost::graph_traits < G >::in_edge_iterator EI_i;
  typedef typename boost::graph_traits < G >::vertex_iterator V_i, V_iend;


  // store a map for the vertices
  typedef typename boost::bimap< int64_t, int64_t > bm_type;
  typedef typename bm_type::left_iterator LI;
  typedef typename bm_type::value_type VT;

  G graph;
  int64_t numb_vertices;
  graphType m_gType;

  bm_type  vertices_map;

  // used by dijkstra
  std::vector<V> predecessors;
  std::vector<float8> distances;

  // used for modification
  std::deque<pgr_edge_t> removed_edges;

  explicit Pgr_base_graph< G >(graphType gtype, const int initial_size)
     : graph(initial_size),
       numb_vertices(0),
       m_gType(gtype)
     {}


 public:

void disconnect_edge(int64_t p_from, int64_t p_to) {
      V g_from;
      V g_to;
      pgr_edge_t d_edge;
      // nothing to do, the vertex doesnt exist
      if (!get_gVertex(p_from, g_from)) return;
      if (!get_gVertex(p_to, g_to)) return;
      EO_i out, out_end;
      for (boost::tie(out, out_end) = out_edges(g_from, graph); out != out_end; ++out) {
            if (target(*out,graph) == g_to) {
                d_edge.id = graph[*out].id;
                d_edge.source = graph[source(*out,graph)].id;
                d_edge.target = graph[target(*out,graph)].id;
                d_edge.cost = graph[*out].cost;
                d_edge.reverse_cost = -1;
                removed_edges.push_back(d_edge);
            }
      }
      boost::remove_edge(g_from,g_to, graph);
}
      

void disconnect_vertex(int64_t p_vertex) {
      V g_vertex;
      pgr_edge_t d_edge; 
      // nothing to do, the vertex doesnt exist
      if (!get_gVertex(p_vertex, g_vertex)) return;
      EO_i out, out_end;
      for (boost::tie(out, out_end) = out_edges(g_vertex, graph); out != out_end; ++out) {
            d_edge.id = graph[*out].id;
            d_edge.source = graph[source(*out,graph)].id;
            d_edge.target = graph[target(*out,graph)].id;
            d_edge.cost = graph[*out].cost;
            d_edge.reverse_cost = -1;
            removed_edges.push_back(d_edge);
      }

      if (m_gType == DIRECTED) {
          EI_i in, in_end;
          for (boost::tie(in, in_end) = in_edges(g_vertex, graph); in != in_end; ++in) {
                d_edge.id = graph[*in].id;
                d_edge.source = graph[source(*in,graph)].id;
                d_edge.target = graph[target(*in,graph)].id;
                d_edge.cost = graph[*in].cost;
                d_edge.reverse_cost = -1;
                removed_edges.push_back(d_edge);
          }
      }

      V  d_vertex = boost::vertex( vertices_map.left.find(p_vertex)->second, graph);
      boost::clear_vertex(d_vertex,graph);
}

void restore_graph() {
      while (removed_edges.size() != 0 ) {
          graph_add_edge(removed_edges[0]);
          removed_edges.pop_front();
      }
};

          



void
print_graph() {
    EO_i out, out_end;
    V_i vi;

    for (vi = vertices(graph).first; vi!=vertices(graph).second; ++vi) {
        std::cout << (*vi) << " out_edges(" << graph[(*vi)].id << "):";
        for     (boost::tie(out, out_end) = out_edges(*vi, graph); out != out_end; ++out) {
            std::cout << ' ' << *out << "=(" << graph[source(*out,graph)].id << "," << graph[target(*out,graph)].id << ") = " <<  graph[*out].cost <<"\t";
        }
        std::cout << std::endl;
    }
} 

protected:
bool
get_gVertex(int64_t vertex_id, V &gVertex) {

  LI vertex_ptr = vertices_map.left.find(vertex_id);
 
  if (vertex_ptr == vertices_map.left.end())
      return false ;
 
  gVertex = vertex(vertex_ptr->second, graph);
  return true;
}  

private:
void
set_gVertex_id(int64_t vertex_id, int64_t &gVertex_id) {
  // look for it int the map
  LI vertex_ptr = vertices_map.left.find(vertex_id);

  // it doesn't exist
  if (vertex_ptr  == vertices_map.left.end()) {
    // insert it
    vertex_ptr = vertices_map.insert( VT (vertex_id, numb_vertices++));
    // vertex_ptr = vertices_map.left.find(edge.source);
  }

  // get the gVertex_id
  gVertex_id = vertex_ptr->second;
}

public:
void
get_path( Path &path,
          V source, V target) {

  V target_back = target;
  int64_t result_size = 1;

  if (target == predecessors[target]) {
       path.clear();
       return;
  };

  while (target != source) {
    if (target == predecessors[target]) break;
    result_size++;
    target = predecessors[target];
  }


  target = target_back;
  int seq = result_size;

  path.push_front(seq, graph[target].id, -1, 0);
  while (target != source) {
    if (target == predecessors[target]) break;
    --seq;
    path.push_front(seq,
        graph[predecessors[target]].id,  // source
        get_edge_id(graph,               // edge
                    predecessors[target],   // from
                    target,                 // to
                    distances[target] - distances[predecessors[target]] //dist(from,to)
        ),
        distances[target] - distances[predecessors[target]] // dist(from,to)
    );
    target = predecessors[target];
  }
  return;
}



public:  //  ???
int64_t
get_edge_id(G &graph, V from, V to, float8 distance) { 
    typename boost::graph_traits < G >::edge_descriptor e;
    typename boost::graph_traits < G >::out_edge_iterator out_i, out_end;
    V v_source, v_target;
    for (boost::tie(out_i, out_end) = boost::out_edges(from, graph);
        out_i != out_end; ++out_i) {
            e = *out_i;
            v_target = target(e, graph);
            if ((to == v_target) && (distance == graph[*out_i].cost))
                    return  graph[*out_i].id;
    }
    return -2;
}


private:
void
graph_add_edge(const pgr_edge_t &edge )
{
  bool inserted;
  LI vm_s, vm_t;
  E e;

  vm_s= vertices_map.left.find(edge.source);
  if (vm_s == vertices_map.left.end()) {
    vertices_map.insert( VT (edge.source, numb_vertices++));
    vm_s= vertices_map.left.find(edge.source);
  }

  vm_t = vertices_map.left.find(edge.target);
  if (vm_t == vertices_map.left.end()) {
    vertices_map.insert( VT (edge.target, numb_vertices++));
    vm_t= vertices_map.left.find(edge.target);
  }

  if (edge.cost >= 0) {
    boost::tie(e, inserted) = boost::add_edge(vm_s->second, vm_t->second, graph);
    graph[e].cost = edge.cost;
    graph[e].id = edge.id;
  }

  if (edge.reverse_cost >= 0) {
    boost::tie(e, inserted) = boost::add_edge(vm_t->second, vm_s->second, graph);
    graph[e].cost = edge.reverse_cost;
    graph[e].id = edge.id;
  }
}

public:
void
graph_insert_data(const pgr_edge_t *data_edges, int64_t count) {
  //typedef typename boost::graph_traits <G>::edge_descriptor E;
  //int64_t numb_vertices = 0;
  for (unsigned int i = 0; i < count; ++i) { 
     graph_add_edge(data_edges[i]);
  }
  for (unsigned int i = 0; i < vertices_map.size(); ++i )
     graph[i].id = vertices_map.right.find(i)->second;
}

};

#endif  // BASE_GRAPH_H
