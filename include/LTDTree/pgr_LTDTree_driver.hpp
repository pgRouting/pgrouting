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

        struct DominatorCorrectnessTestSet
        {
            typedef pair<int, int> edge;

            int numOfVertices;
            vector<edge> edges;
            vector<int> correctIdoms;
        };

        template<class Graph>
        class Pgr_LTDTree {
        public:
            typedef typename Graph::V Vertex; //Note here G is base_graph

           // typedef typename G::E_i i;
            //typedef typename G::vertIndex iiii;

            //typedef typename G::vertIndex IndexMap;
            //typedef typename pgrouting::graph::Pgr_base_graph< G, pgrouting::Basic_vertex, pgrouting::Basic_edge >::vertIndex ind;
           // typedef property_map<G, vertex_index_t>::type IndexMap;

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
/*
                auto IndexMap =graph.vertIndex;
                auto vvvv=graph.num_vertices();

                //  iterator_property_map<std::vector<Vertex>::iterator, IndexMap> PredMap; //We need to make_iterator_pro._map
                //IndexMap indexMap(get(pgrouting::Basic_vertex::vertex_index, graph));
                boost::iterator_property_map<std::vector<Vertex>::iterator, IndexMap> PredMap;

                vector<Vertex> domTreePredVector, domTreePredVector2;

                IndexMap indexMap(get(vertex_index, graph.graph));

                typedef typename Graph::V_i uItr, uEnd;
                int j = 0;
                for (boost::tie(uItr, uEnd) = vertices(graph.graph); uItr != uEnd; ++uItr, ++j)
                {
                    put(indexMap, *uItr, j);
                }

                // Lengauer-Tarjan dominator tree algorithm

                domTreePredVector =
                        vector<Vertex>(graph.num_vertices(graph.graph), boost::graph_traits<Graph>::null_vertex());
                PredMap domTreePredMap =
                        make_iterator_property_map(domTreePredVector.begin(), indexMap);

                lengauer_tarjan_dominator_tree(g, graph.get_V(root), domTreePredMap);

                vector<int> idom(graph.num_vertices(graph));
                /*for (boost::tie(uItr, uEnd) = vertices(g); uItr != uEnd; ++uItr)
                {
                    if (get(domTreePredMap, *uItr) != graph_traits<G>::null_vertex())
                        idom[get(indexMap, *uItr)] =
                                get(indexMap, get(domTreePredMap, *uItr));
                    else
                        idom[get(indexMap, *uItr)] = (numeric_limits<int>::max)();
                }
                 */



















                // Make property maps
                /***************Calculating******************/


                // Tarjan's paper//Week 3 work
                typedef DominatorCorrectnessTestSet::edge edge;

                DominatorCorrectnessTestSet testSet[1];


                testSet[0].numOfVertices = 13;
                testSet[0].edges.push_back(edge(0, 1));
                testSet[0].edges.push_back(edge(0, 2));
                testSet[0].edges.push_back(edge(0, 3));
                testSet[0].edges.push_back(edge(1, 4));
                testSet[0].edges.push_back(edge(2, 1));
                testSet[0].edges.push_back(edge(2, 4));
                testSet[0].edges.push_back(edge(2, 5));
                testSet[0].edges.push_back(edge(3, 6));
                testSet[0].edges.push_back(edge(3, 7));
                testSet[0].edges.push_back(edge(4, 12));
                testSet[0].edges.push_back(edge(5, 8));
                testSet[0].edges.push_back(edge(6, 9));
                testSet[0].edges.push_back(edge(7, 9));
                testSet[0].edges.push_back(edge(7, 10));
                testSet[0].edges.push_back(edge(8, 5));
                testSet[0].edges.push_back(edge(8, 11));
                testSet[0].edges.push_back(edge(9, 11));
                testSet[0].edges.push_back(edge(10, 9));
                testSet[0].edges.push_back(edge(11, 0));
                testSet[0].edges.push_back(edge(11, 9));
                testSet[0].edges.push_back(edge(12, 8));
                testSet[0].correctIdoms.push_back((numeric_limits<int>::max)());
                testSet[0].correctIdoms.push_back(0); // correctIdoms.push_back(0); represents that 0 is the idom of vertex 0
                testSet[0].correctIdoms.push_back(0); //// correctIdoms.push_back(0); represents that 0 is the idom of 1
                testSet[0].correctIdoms.push_back(0);
                testSet[0].correctIdoms.push_back(0);
                testSet[0].correctIdoms.push_back(0);
                testSet[0].correctIdoms.push_back(3); // correctIdoms.push_back(0); represents that 3 is the idom of 5
                testSet[0].correctIdoms.push_back(3);
                testSet[0].correctIdoms.push_back(0);
                testSet[0].correctIdoms.push_back(0);
                testSet[0].correctIdoms.push_back(7);
                testSet[0].correctIdoms.push_back(0);
                testSet[0].correctIdoms.push_back(4);




                for (size_t i = 0; i < sizeof(testSet)/sizeof(testSet[0]); ++i)
                {
                    const int numOfVertices = testSet[i].numOfVertices;

                    G g(
                            testSet[i].edges.begin(), testSet[i].edges.end(),
                            numOfVertices);

                    typedef graph_traits<G>::vertex_descriptor Vertex;
                    typedef property_map<G, vertex_index_t>::type IndexMap;
                    IndexMap indexMap(boost::get(vertex_index, g));
                    typedef
                    iterator_property_map<vector<Vertex>::iterator, IndexMap>
                            PredMap;

                    vector<Vertex> domTreePredVector, domTreePredVector2;
                    //IndexMap indexMap(get(vertex_index, g));
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

                    lengauer_tarjan_dominator_tree(g, vertex(0, g), domTreePredMap);

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
                    int x=0;
                    for(int i: idom)
                    {
                        cout<<"idom of "<<x<<"  is "<<i<<endl;
                        x++;
                    }
                    // copy(idom.begin(), idom.end(), ostream_iterator<int>(cout, " "));
                    cout << endl;

                    // dominator tree correctness test


                }




/*************************************************************/
                std::vector<pgr_ltdtree_rt> results;
                results[0].idom=7;
                results[0].vid=3;
                return results;


            }


        };
    }
}


#endif  // INCLUDE_LTDTREE_PGR_LTDTREE_HPP
