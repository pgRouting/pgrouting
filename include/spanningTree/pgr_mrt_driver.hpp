/*PGR-GNU*****************************************************************
File: pgr_mrt_driver.hpp

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


#ifndef INCLUDE_PGR_MRT_DRIVER_HPP
#define INCLUDE_PGR_MRT_DRIVER_HPP
#pragma once
#include <vector>

#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/pgr_messages.h"
#include <iostream>
#include <algorithm>

#include "c_types/pgr_mrt_rt.h"
#include <boost/graph/two_graphs_common_spanning_trees.hpp>

#include <boost/graph/depth_first_search.hpp>

using namespace boost;
using namespace  std;

namespace pgrouting {
    namespace functions {
    /**TODO***
    1. Return proper results
    2. SOlve all the errors
    *******/
        

        template<class Graph>
        class Pgr_mrt : public pgrouting::Pgr_messages {
        public:
            std::vector<pgr_mrt_rt> results;
            typedef typename Graph::E E;
            typedef typename Graph::E_i E_i;
            typedef typename Graph::V V;

            std::vector<pgr_mrt_rt> pgr_mrt(
                Graph &graph_1,
                Graph &graph_2,
                bool directed

                    ){
                log<<"Inside main boost driver"<<endl;
                if(directed)
                {
                    //log<<"Edge "<<E<<endl;


                    log<<"In directed graph total edges G1 "<<graph_1.num_edges()<<endl;
                    log<<"In directed graph total edges G2 "<<graph_2.num_edges()<<endl;
                    log<<"Need coding"<<endl;
                 
                }
                else
                {/*
                
                    log<<"Working with undirected graph"<<endl;

                    std::vector<bool> inL;

                    std::vector< std::vector<bool> > coll;
                    boost::tree_collector<
                            std::vector< std::vector<bool> >,
                            std::vector<bool>
                    > tree_collector(coll);
                    boost::two_graphs_common_spanning_trees
                            (
                                    graph_1.graph,
                                    graph_2.graph,
                                    tree_collector,
                                    inL
                            );
                   */         
                           
                }
                return results;

            }
        };
    }
}

#endif  // INCLUDE_PGR_MRT_DRIVER_HPP
