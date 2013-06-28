/*
 * Shortest path algorithm for PostgreSQL
 *
 * Copyright (c) 2005 Sylvain Pasche
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#include <exception>
#include <vector>
#include <boost/config.hpp>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include "k_targets_sp.h"

using namespace std;
using namespace boost;

/*
//    FIXME: use this to avoid heap allocation ?

void* operator new(size_t size)
{
return palloc(size);
}

void operator delete(void *p)
{
    pfree(p);
}

*/

// Maximal number of nodes in the path (to avoid infinite loops)
#define MAX_NODES 100000000

struct Vertex
{
    int id;
    float8 cost;
};

// Adds an edge to the graph.
// Edge id, cost, source and target ids and coordinates are copied also
template <class G, class E>
static void
graph_add_edge(G &graph, E &e, int id, int source, int target, float8 cost)
{
  bool inserted;

  if (cost < 0) // edges are not inserted in the graph if cost is negative
    return;

  tie(e, inserted) = add_edge(source, target, graph);

  graph[e].cost = cost;
  graph[e].id = id;

  typedef typename graph_traits<G>::vertex_descriptor Vertex;
  Vertex s = vertex(source, graph);
  Vertex t = vertex(target, graph);
}


int onetomany_dijkstra_boostdist(edge_t *edges, unsigned int count,
        int start_vertex, int *end_vertices, int nb_targets,
        bool directed, bool has_reverse_cost,
#ifdef PGR_MERGE
        pgr_cost_t **dists,
#else
        dist_fromto_t **dists,
#endif
        char **err_msg)
{
  try {    
    // FIXME: use a template for the directedS parameters
    typedef adjacency_list < listS, vecS, directedS, no_property, Vertex> graph_t;
    typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
    typedef graph_traits < graph_t >::edge_descriptor edge_descriptor;
    typedef std::pair<int, int> Edge;

    // FIXME: compute this value
    const unsigned int num_nodes = ((directed && has_reverse_cost ? 2 : 1) * count) + 100;

    graph_t graph(num_nodes);

    //property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, graph);

    for (std::size_t j = 0; j < count; ++j)
    {
        edge_descriptor e;
        graph_add_edge<graph_t, edge_descriptor>(graph, e,
                   edges[j].id, edges[j].source,
                   edges[j].target, edges[j].cost);

        if (!directed || (directed && has_reverse_cost))
        {
          float8 cost;

          if (has_reverse_cost)
          {
              cost = edges[j].reverse_cost;
          }
          else
          {
              cost = edges[j].cost;
          }

          graph_add_edge<graph_t, edge_descriptor>(graph, e,
                 edges[j].id,
                 edges[j].target,
                 edges[j].source,
                 cost);
          }
    }

    std::vector<vertex_descriptor> predecessors(num_vertices(graph));

    vertex_descriptor _source = vertex(start_vertex, graph);

    if (_source < 0 /*|| _source >= num_nodes*/) 
    {
        *err_msg = (char *) "Starting vertex not found";
        return -1;
    }

    vertex_descriptor _target[nb_targets];
    for (int i = 0; i < nb_targets; i++)
    {
        _target[i] = vertex(end_vertices[i], graph);


        if (_target[i] < 0 )
        {
            *err_msg = (char *) "Ending vertex %d not found", i;
            return -1;
        }
    }        

    std::vector<float8> distances(num_vertices(graph));

    dijkstra_shortest_paths(graph, _source,
                predecessor_map(&predecessors[0]).
                weight_map(get(&Vertex::cost, graph))
                .distance_map(&distances[0]));

    std::vector< std::vector<int> > path_vect(nb_targets);

    int max;

    int index_of_last_path_vertex = 0;
    int sum_path_sizes = 0, i = 0, j = 0;
    bool no_path[nb_targets];
    for (i = 0; i < nb_targets; i++)
    {
        no_path[i] = false;
        max = MAX_NODES;
        path_vect[i].push_back(_target[i]);

        while (_target[i] != _source && !no_path[i]) 
        {
            if (_target[i] == predecessors[_target[i]]) 
            {
                //"No path found"
                path_vect[i].clear();
                path_vect[i].push_back(end_vertices[i]);
                path_vect[i].push_back(start_vertex);
                no_path[i] = true;
                break;
            }

            _target[i] = predecessors[_target[i]];
            path_vect[i].push_back(_target[i]);

            if (!max--) 
            {
                *err_msg = (char *) "Overflow";
                return -1;
            }
        }
        sum_path_sizes += path_vect[i].size();
    }

#ifdef PGR_MERGE
    *dists = (pgr_cost_t *) malloc(sizeof(pgr_cost_t) * nb_targets + 1);
    if (! *dists) {
        *err_msg = (char *) "Error: out of memory";
        return -1;
    }
#else
    *dists = (dist_fromto_t *) malloc(sizeof(dist_fromto_t) * nb_targets + 1);
#endif

    for (int numTarget = 0; numTarget < nb_targets; numTarget++)
    {

#ifdef PGR_MERGE
        (*dists)[numTarget].seq = numTarget;
        (*dists)[numTarget].id1 = path_vect[numTarget].at(path_vect[numTarget].size() -1);
        (*dists)[numTarget].id2 = path_vect[numTarget].at(0);
#else
        (*dists)[numTarget].vertex_id_source = path_vect[numTarget].at(path_vect[numTarget].size() -1);
        (*dists)[numTarget].vertex_id_target = path_vect[numTarget].at(0);
        (*dists)[numTarget].edge_id_source = -1;
        (*dists)[numTarget].edge_id_target = -1;
#endif
        (*dists)[numTarget].cost = 0.0;
        if (no_path[numTarget]){
            (*dists)[numTarget].cost = -1.0;
        }
        else {
            for(i = path_vect[numTarget].size() - 1, j = index_of_last_path_vertex; i >= 0; i--, j++)
            {
                graph_traits < graph_t >::vertex_descriptor v_src;
                graph_traits < graph_t >::vertex_descriptor v_targ;
                graph_traits < graph_t >::edge_descriptor e;
                graph_traits < graph_t >::out_edge_iterator out_i, out_end;

                if (i == 0) continue;

                v_src = path_vect[numTarget].at(i);
                v_targ = path_vect[numTarget].at(i - 1);

                for (tie(out_i, out_end) = out_edges(v_src, graph); out_i != out_end; ++out_i)
                {
                    graph_traits < graph_t >::vertex_descriptor v, targ;
                    e = *out_i;
                    v = source(e, graph);
                    targ = target(e, graph);

                    if (targ == v_targ) {
#ifndef PGR_MERGE
                        if ((*dists)[numTarget].edge_id_source < 0)
                            (*dists)[numTarget].edge_id_source = graph[*out_i].id;
#endif
                        (*dists)[numTarget].cost += graph[*out_i].cost;
                        break;
                    }
                }
#ifndef PGR_MERGE
                (*dists)[numTarget].edge_id_target = graph[*out_i].id;
#endif

            }
            index_of_last_path_vertex = j;
        }
    }

    return EXIT_SUCCESS;
  }
  catch(std::exception& e) {
      *err_msg = (char *) e.what();
      return -1;
  }
  catch (...) {
      *err_msg = (char *) "Unknown exception caught!";
      return -1;
  }
};

template <class T>
string toString( T argument){
    string r;
    stringstream s;
    s << argument;
    r = s.str();
    return r;
};

int 
onetomany_dijkstra_boostpath(edge_t *edges, unsigned int count,
        int start_vertex, int *end_vertices, int nb_targets,
        bool directed, bool has_reverse_cost,
#ifdef PGR_MERGE
        pgr_cost_t **pathdists,
        int *path_count,
#else
        path_fromto_t **pathdists,
#endif
        char **err_msg )
{
try {

    // FIXME: use a template for the directedS parameters
    typedef adjacency_list < listS, vecS, directedS, no_property, Vertex> graph_t;
    typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
    typedef graph_traits < graph_t >::edge_descriptor edge_descriptor;
    typedef std::pair<int, int> Edge;

    // FIXME: compute this value
    const unsigned int num_nodes = ((directed && has_reverse_cost ? 2 : 1) * count) + 100;

    graph_t graph(num_nodes);

    //property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, graph);

    for (std::size_t j = 0; j < count; ++j)
    {
        edge_descriptor e;
        graph_add_edge<graph_t, edge_descriptor>(graph, e,
                   edges[j].id, edges[j].source,
                   edges[j].target, edges[j].cost);

        if (!directed || (directed && has_reverse_cost))
        {
          float8 cost;

          if (has_reverse_cost)
          {
              cost = edges[j].reverse_cost;
          }
          else
          {
              cost = edges[j].cost;
          }

          graph_add_edge<graph_t, edge_descriptor>(graph, e,
                 edges[j].id,
                 edges[j].target,
                 edges[j].source,
                 cost);
          }
    }

    std::vector<vertex_descriptor> predecessors(num_vertices(graph));

    vertex_descriptor _source = vertex(start_vertex, graph);

    if (_source < 0 /*|| _source >= num_nodes*/) 
    {
        *err_msg = (char *) "Starting vertex not found";
        return -1;
    }

    vertex_descriptor _target[nb_targets];
    for (int i = 0; i < nb_targets; i++)
    {
        _target[i] = vertex(end_vertices[i], graph);


        if (_target[i] < 0)
        {
            *err_msg = (char *) "Ending vertex %d not found", i;
            return -1;
        }
    }        

    std::vector<float8> distances(num_vertices(graph));
    // calling Boost function

    dijkstra_shortest_paths(graph, _source,
                predecessor_map(&predecessors[0]).
                weight_map(get(&Vertex::cost, graph))
                .distance_map(&distances[0]));

    std::vector< std::vector<int> > path_vect(nb_targets);

    int max = MAX_NODES;
    int index_of_last_path_vertex = 0;
    int sum_path_sizes = 0, i = 0, j = 0;
    bool no_path[nb_targets];

    for (i = 0; i < nb_targets; i++)
    {
        no_path[i] = false;
        max = MAX_NODES;
        path_vect[i].push_back(_target[i]);

        while (_target[i] != _source && !no_path[i]) 
        {
            if (_target[i] == predecessors[_target[i]]) 
            {
                //No path found
                path_vect[i].clear();
                path_vect[i].push_back(end_vertices[i]);
                path_vect[i].push_back(start_vertex);
                no_path[i] = true;
                break;
            }
            _target[i] = predecessors[_target[i]];
            path_vect[i].push_back(_target[i]);

            if (!max--) 
            {
                *err_msg = (char *) "Exceeded MAX_NODES";
                return -1;
            }
        }
        sum_path_sizes += path_vect[i].size();
    }
#ifdef PGR_MERGE
    *pathdists = (pgr_cost_t *) malloc(sizeof(pgr_cost_t) * sum_path_sizes + nb_targets + 1);
    if (! *pathdists) {
        *err_msg = (char *) "Error: out of memory";
        return -1;
    }

    int seq = 0;
    for (int numTarget = 0; numTarget < nb_targets; numTarget++) {
        int id1 = path_vect[numTarget].at(0);

        if (no_path[numTarget]) {
            (*pathdists)[seq].seq = seq;
            (*pathdists)[seq].id1 = id1;
            (*pathdists)[seq].id2 = -1;
            (*pathdists)[seq].cost = -1.0;
            seq++;
            continue;
        }

        for(i = path_vect[numTarget].size() - 1, j = index_of_last_path_vertex; i >= 0; i--, j++) {
            graph_traits < graph_t >::vertex_descriptor v_src;
            graph_traits < graph_t >::vertex_descriptor v_targ;
            graph_traits < graph_t >::edge_descriptor e;
            graph_traits < graph_t >::out_edge_iterator out_i, out_end;

            if (i == 0) continue;

            v_src = path_vect[numTarget].at(i);
            v_targ = path_vect[numTarget].at(i - 1);

            for (tie(out_i, out_end) = out_edges(v_src, graph); out_i != out_end; ++out_i) {
                graph_traits < graph_t >::vertex_descriptor v, targ;
                e = *out_i;
                targ = target(e, graph);

                if (targ == v_targ) {
                    (*pathdists)[seq].seq = seq;
                    (*pathdists)[seq].id1 = id1;
                    (*pathdists)[seq].id2 = graph[*out_i].id; 
                    (*pathdists)[seq].cost = graph[*out_i].cost;
                    seq++;
                    break;
                }
            }
        }
        index_of_last_path_vertex = j;
    }

    *path_count = seq;

#else

    *pathdists = (path_fromto_t *) malloc(sizeof(path_fromto_t) * nb_targets + 1);

    std::string edgeIDLists [nb_targets];
    for (int numTarget = 0; numTarget < nb_targets; numTarget++)
    {

        (*pathdists)[numTarget].vertex_id_source = path_vect[numTarget].at(path_vect[numTarget].size() -1);
        (*pathdists)[numTarget].vertex_id_target = path_vect[numTarget].at(0);
        (*pathdists)[numTarget].edge_id_source = -1;
        (*pathdists)[numTarget].edge_id_target = -1;
        (*pathdists)[numTarget].cost = 0;
        edgeIDLists[numTarget] = "";

        if (no_path[numTarget]){

            (*pathdists)[numTarget].cost = -1;
            edgeIDLists[numTarget] = "NOT FOUND";
        }
        else {
            for(i = path_vect[numTarget].size() - 1, j = index_of_last_path_vertex; i >= 0; i--, j++)
            {
                graph_traits < graph_t >::vertex_descriptor v_src;
                graph_traits < graph_t >::vertex_descriptor v_targ;
                graph_traits < graph_t >::edge_descriptor e;
                graph_traits < graph_t >::out_edge_iterator out_i, out_end;

                if (i == 0) 
                {
                    continue;
                }

                v_src = path_vect[numTarget].at(i);
                v_targ = path_vect[numTarget].at(i - 1);

                for (tie(out_i, out_end) = out_edges(v_src, graph); out_i != out_end; ++out_i)
                {

                    graph_traits < graph_t >::vertex_descriptor v, targ;
                    e = *out_i;
                    v = source(e, graph);
                    targ = target(e, graph);

                    if (targ == v_targ)
                    {
                        if ((*pathdists)[numTarget].edge_id_source < 0)
                            (*pathdists)[numTarget].edge_id_source = graph[*out_i].id;
                        else
                            edgeIDLists[numTarget] += ", ";
                        edgeIDLists[numTarget] += toString(graph[*out_i].id);
                        (*pathdists)[numTarget].cost += graph[*out_i].cost;
                        break;
                    }
                }
                (*pathdists)[numTarget].edge_id_target = graph[*out_i].id;
            }
        }
        (*pathdists)[numTarget].the_way = (char*)malloc(sizeof(char) * edgeIDLists[numTarget].size() + 1);

        strcpy((*pathdists)[numTarget].the_way, edgeIDLists[numTarget].c_str());
        index_of_last_path_vertex = j;
    }
#endif

    return EXIT_SUCCESS;
  }
  catch(std::exception& e) {
      *err_msg = (char *) e.what();
      return -1;
  }
  catch (...) {
      *err_msg = (char *) "Unknown exception caught!";
      return -1;
  }
}

