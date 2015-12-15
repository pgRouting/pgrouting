#ifndef MY_BASE_GRAPH_HPP_
#define MY_BASE_GRAPH_HPP_

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <deque>
#include <vector>
#include <set>
#include <map>
#include <limits>
#include "structs.h"

using namespace std;

template <class G>
class My_base_graph {
public:
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
  G graph;                //The graph with edges as bundled properties
  int64_t num_vertices;  //number of vertices
  graphType gType;      //enumtype (DIRECTED or UNDIRECTED)

  degree_to_V degree_to_V_map;
  //the user recognises a vertex through a number "id"

  //internally we represent it through a vertex descriptor "V"

  
  id_to_V  id_V_map;   //id--->V mapping
  V_to_id  V_id_map;  //V--->id mapping

  // A queue of removed edges (stored because u may need to add them back later)
  std::deque<Edge> removed_edges;

  //used by My_dijkstra class as it inherits My_base_graph
  std::vector<V> predecessors;
  std::vector<float> distances;

  //constructor for this class
  explicit My_base_graph< G >(graphType gtype, const int initial_size)
  : graph(initial_size),num_vertices(0),gType(gtype)
  {}

  // Inserts edges of in the form of struct "Edge" into the graph
  void insert_data(const Edge *data_edges, int64_t count) {
  	int64_t vcount=0;
    for (unsigned int i = 0; i < count; ++i) {
      //cout << data_edges[i].source << ", " << data_edges[i].target << endl;
  		graph_add_edge(data_edges[i]);
  	}
    calculateDegrees();
  }


  // adds edge to the graph and also to the vertex maps respectively
  void graph_add_edge(const Edge &edge ) {
  	bool inserted;
  	LI vm_s, vm_t;
  	E e;

  	vm_s = id_V_map.find(edge.source);
  	if (vm_s == id_V_map.end()) {
  		id_V_map[edge.source]=  num_vertices;
  		V_id_map[num_vertices++] = edge.source;
  		vm_s = id_V_map.find(edge.source);
      graph[vm_s->second].id=edge.source;
      graph[vm_s->second].contractions=0;
  	}

  	vm_t = id_V_map.find(edge.target);
  	if (vm_t == id_V_map.end()) {
  		id_V_map[edge.target]=  num_vertices;
  		V_id_map[num_vertices++] = edge.target;
  		vm_t = id_V_map.find(edge.target);
      graph[vm_t->second].id=edge.target;
  	 graph[vm_t->second].contractions=0;
    }

  	//if (edge.cost >= 0) {
  		boost::tie(e, inserted) =
  		boost::add_edge(vm_s->second, vm_t->second, graph);
  		graph[e].cost = edge.cost;
  		graph[e].id = edge.id;
  	//}

  	/*if (edge.revcost >= 0) {
  		boost::tie(e, inserted) =
  		boost::add_edge(vm_t->second, vm_s->second, graph);
  		graph[e].cost = edge.revcost;
  		graph[e].id = edge.id;
  	}*/
  }


  // removes a vertex (removes all edges connected to it,and its id and descriptors from both the maps)
  void remove_vertex(int64_t p_vertex) {
      V g_vertex;
      Edge d_edge;
      // nothing to do, the vertex doesnt exist
      if (!get_vertex_descriptor(p_vertex, g_vertex)) return;
      EO_i out, out_end;
      // store the edges that are going to be removed
      for (boost::tie(out, out_end) = out_edges(g_vertex, graph);
        out != out_end; ++out) {
            d_edge.id = graph[*out].id;
            d_edge.source = graph[source(*out, graph)].id;
            d_edge.target = graph[target(*out, graph)].id;
            //cout << "Removing " << d_edge.source << " " << d_edge.target << endl;
            d_edge.cost = graph[*out].cost;
            d_edge.revcost = -1;
            removed_edges.push_back(d_edge);
      }

      if (gType == DIRECTED) {
          EI_i in, in_end;
          for (boost::tie(in, in_end) = in_edges(g_vertex, graph);
            in != in_end; ++in) {
                d_edge.id = graph[*in].id;
                d_edge.source = graph[source(*in, graph)].id;
                d_edge.target = graph[target(*in, graph)].id;
                d_edge.cost = graph[*in].cost;
                d_edge.revcost = -1;
                removed_edges.push_back(d_edge);
          }
      }

      V d_vertex = boost::vertex(id_V_map.find(p_vertex)->second, graph);
      
      // delete incomming and outgoing edges from the vertex
      boost::clear_vertex(d_vertex, graph);
  }

  //returns a vertex_descriptor for a vertex with a particular id
  bool get_vertex_descriptor(int64_t id,V &v)
  {
  	LI vertex_ptr = id_V_map.find(id);

      if (vertex_ptr == id_V_map.end())
          return false;

      v = vertex(vertex_ptr->second, graph);
      return true;
  }


  //returns a id for a vertex with a particular vertex_descriptor 
  bool get_vertex_id(V v,int64_t &id)
  {
  	RI id_ptr = V_id_map.find(v);

      if (id_ptr == V_id_map.end())
          return false;

      id = id_ptr->second;
      return true;
  }


  //prints the graph
  void print_graph() {
        EO_i out, out_end;
        V_i vi;
        for (vi = vertices(graph).first; vi != vertices(graph).second; ++vi) {
            std::cout << "vdesc:- " << (*vi) << " out_edges(" << graph[(*vi)].id << ") " << "degree:- " << graph[(*vi)].degree << " contractions:- " << graph[(*vi)].contractions << endl ;
            for (boost::tie(out, out_end) = out_edges(*vi, graph);
              out != out_end; ++out) {
              std::cout << ' ' << *out << "=(" << graph[source(*out, graph)].id
                        << ", " << graph[target(*out, graph)].id << ") = "
                        <<  graph[*out].cost <<"\t";
            }
            std::cout << std::endl;
        }
        /*std::cout << "\n i, distance, predecesor\n"; 
        for (unsigned int i = 0; i < distances.size(); i++) {
            std::cout << i+1 << ", " << distances[i] << ", " << predecessors[i] << "\n";
        }*/
    }


    void calculateDegrees()
    {
      EO_i out, out_end;
        V_i vi;
        int count;
        for (vi = vertices(graph).first; vi != vertices(graph).second; ++vi) {
            count=0;
            for (boost::tie(out, out_end) = out_edges(*vi, graph);
              out != out_end; ++out) {
              count++;
            }
            graph[(*vi)].degree=count;
            degree_to_V_map[count].push_back(*vi);
        }
    }
    void print_Vertex_Degree()
    {
      cout << "Printing V_degree map" << endl;
      V_i vi;
      for (vi = vertices(graph).first; vi != vertices(graph).second; ++vi) {
          cout << graph[(*vi)].id << "-->" << graph[(*vi)].degree << endl;
      }
    }

    void print_Degree_Vertices()
    {
      cout << "Printing degree_V map" << endl;
      degree_to_V_i it1;
      Q_i it2;
      for ( it1=degree_to_V_map.begin(); it1!=degree_to_V_map.end(); ++it1)
      {
        cout << it1->first << "-->" ;
        for (it2=it1->second.begin(); it2 !=it1->second.end(); ++it2)
        {
           cout << graph[(*it2)].id << ", ";
        }
        cout << endl;
      }
    }


};



#endif