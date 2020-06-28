/*PGR-GNU*****************************************************************
File: pgr_LTDTree.hpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2020 Prakash Tiwari
Mail: 85prakash2017@gmail.com

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 ********************************************************************PGR-GNU*/


#ifndef INCLUDE_LTDTREE_PGR_LTDTREE_HPP
#define INCLUDE_LTDTREE_PGR_LTDTREE_HPP
#pragma once
#include <vector>

#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/pgr_messages.h"
#include <iostream>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dominator_tree.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/property_map/vector_property_map.hpp>
#include <boost/type_traits.hpp>
using namespace boost;
using namespace  std;

namespace pgrouting {
    namespace functions {

        template<class Graph>
        class Pgr_LTDTree : public pgrouting::Pgr_messages {
        public:
            typedef typename Graph::V Vertex;
            typedef typename Graph::E_i E_i;
            typedef adjacency_list<
                    listS,
                    listS,
                    bidirectionalS,
                    property<vertex_index_t, std::size_t>, no_property> G;

            typedef pair<int64_t , int64_t> edge; //For making edge list to be used in extract vertices
            vector<edge> edgeList;
            std::vector<pgr_ltdtree_rt> results;
            std::set<int64_t > vertex_set;
            int64_t min_vertex, max_vertex;


            void extract_vertices(
                    Graph &graph,
                    int64_t numVertices){

                E_i ei, ei_end;
                int i;
                for (boost::tie(ei, ei_end) = edges(graph.graph),i = 0; ei != ei_end; ++ei,++i) {
                    int64_t source = graph[graph.source(*ei)].id;
                    vertex_set.insert(source);
                }

                min_vertex = *(vertex_set.begin());
                max_vertex = min_vertex+numVertices-1;

                log<<"\n max: "<<max_vertex<<" min: "<<min_vertex;
                log<<"\n mapping started"<<endl;

                for (boost::tie(ei, ei_end) = edges(graph.graph),i = 0; ei != ei_end; ++ei,++i) {
                    int64_t source = graph[graph.source(*ei)].id;
                    int64_t target = graph[graph.target(*ei)].id;
                    edgeList.push_back(edge (source-min_vertex,target-min_vertex));

                }

            }

/************************************* To check validity of root vertex****************************************/
            bool is_valid_root(int64_t root)
            {
                if(root < min_vertex) return false;
                if(root > max_vertex) return false;
                return true;
            }


/******************** Method to calculate dominator tree and returns result vector ***************************/
            std::vector <pgr_ltdtree_rt> pgr_ltdtree(
                    Graph &graph,
                    int64_t root
                    ){
                   const int64_t numOfVertices=graph.num_vertices();
                   extract_vertices(graph,numOfVertices);
                   if(!is_valid_root(root))
                   {
                       notice<<"Invalid root "<<root<<"\n";
                       return results;
                   }

                   typedef graph_traits<G>::vertex_descriptor Vertex;
                   typedef property_map<G, vertex_index_t>::type IndexMap;
                   typedef iterator_property_map<vector<Vertex>::iterator, IndexMap> PredMap;

                   G g(edgeList.begin(), edgeList.end(),numOfVertices);
                   vector<Vertex> domTreePredVector, domTreePredVector2;
                   IndexMap indexMap(get(vertex_index, g));
                   graph_traits<G>::vertex_iterator uItr, uEnd;
                   int j = 0;
                   for (boost::tie(uItr, uEnd) = vertices(g); uItr != uEnd; ++uItr, ++j)
                   {
                       put(indexMap, *uItr, j);
                   }
                   // Lengauer-Tarjan dominator tree algorithm
                   domTreePredVector =
                           vector<Vertex>(num_vertices(g), graph_traits<G>::null_vertex());
                   PredMap domTreePredMap =
                           make_iterator_property_map(domTreePredVector.begin(), indexMap);
                   lengauer_tarjan_dominator_tree(g, vertex(root-min_vertex, g), domTreePredMap);


/*****************************************Making result vector*************************************/
                   pgr_ltdtree_rt temp;
                   for (boost::tie(uItr, uEnd) = vertices(g); uItr != uEnd; ++uItr)
                   {
                       if (get(domTreePredMap, *uItr) != graph_traits<G>::null_vertex())
                       {
                           temp.vid=(get(indexMap, *uItr)+min_vertex);
                           temp.idom=get(indexMap, get(domTreePredMap, *uItr))+min_vertex;
                           results.push_back(temp);
                       }
                       else
                       {

                           temp.vid=(get(indexMap, *uItr)+min_vertex);
                           temp.idom=0;
                           results.push_back(temp);
                       }

                   }

                return results;
            }
        };
    }
}
#endif  // INCLUDE_LTDTREE_PGR_LTDTREE_HPP
