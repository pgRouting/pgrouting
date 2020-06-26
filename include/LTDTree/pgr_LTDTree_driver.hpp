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

//#include <boost/test/minimal.hpp>
#include <iostream>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dominator_tree.hpp>
//#include <boost/property_map/property_map.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/property_map/vector_property_map.hpp>
#include <boost/type_traits.hpp>
using namespace boost;
using namespace  std;

namespace pgrouting {
    namespace functions {

        template<class Graph>
        class Pgr_LTDTree {
        public:
            typedef typename Graph::V Vertex; //Note here G is base_graph
            typedef typename Graph::E_i E_i;

            typedef adjacency_list<
                    listS,
                    listS,
                    bidirectionalS,
                    property<vertex_index_t, std::size_t>, no_property> G;


            //To calculate dominator tree
            std::vector <pgr_ltdtree_rt> llengauer_tarjan_dominator_tree(
                    Graph &graph,
                    int64_t root
                    ){

                /***********************Typedefs of pfRouting**************/


/*************************************************************/
                typedef pair<int64_t , int64_t> edge;
                vector<edge> edges;
                /*
                E_i ei, ei_end;
                int i;
                for (boost::tie(ei, ei_end) = edges(graph.graph),i = 0; ei != ei_end; ++ei,++i) {
                    int64_t source = graph[graph.source(*ei)].id;
                    int64_t target = graph[graph.target(*ei)].id;
                    edgeList.push_back(edge (source-1,target-1));
                }
                const auto numOfVertices=graph.num_vertices();
                */
                const auto numOfVertices = 8;
                edges.push_back(edge(0, 1));
                edges.push_back(edge(1, 2));
                edges.push_back(edge(1, 3));
                edges.push_back(edge(2, 7));
                edges.push_back(edge(3, 4));
                edges.push_back(edge(4, 5));
                edges.push_back(edge(4, 6));
                edges.push_back(edge(6, 4));
                edges.push_back(edge(5, 7));
                G g(
                        edges.begin(), edges.end(),
                        numOfVertices);

                typedef graph_traits<G>::vertex_descriptor Vertex;
                typedef property_map<G, vertex_index_t>::type IndexMap;
                typedef
                iterator_property_map<vector<Vertex>::iterator, IndexMap>
                        PredMap;

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

                lengauer_tarjan_dominator_tree(g, vertex(root, g), domTreePredMap);

                vector<int> idom(num_vertices(g));
                for (boost::tie(uItr, uEnd) = vertices(g); uItr != uEnd; ++uItr)
                {
                    if (get(domTreePredMap, *uItr) != graph_traits<G>::null_vertex())
                        idom[get(indexMap, *uItr)] =
                                get(indexMap, get(domTreePredMap, *uItr));
                    else
                        idom[get(indexMap, *uItr)] = (numeric_limits<int>::max)();
                }

                ///
                boost::tie(uItr, uEnd) = vertices(g);
                std::vector<pgr_ltdtree_rt> results;
                int x=0;
                for(int i: idom)
                {
                    //cout<<"idom of "<<get(indexMap, *uItr)<<"  is "<<i<<endl;
                    if(i!=(numeric_limits<int>::max)())
                    {
                        results[x].idom=i+1;
                        results[x].vid=x+1;
                    }
                    else
                    {
                        results[x].idom=0;
                        results[x].vid=x+1;
                    }

                    x++;

                }


              //  results[0].idom=7;
               // results[0].vid=3;
                //std::vector<pgr_ltdtree_rt> results;
                return results;


            }


        };
    }
}


#endif  // INCLUDE_LTDTREE_PGR_LTDTREE_HPP
