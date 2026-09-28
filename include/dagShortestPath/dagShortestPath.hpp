/*PGR-GNU*****************************************************************
File: dagShortestPath.hpp

Copyright (c) 2013-2026 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2018 Sourabh Garg
sourabh.garg.mat@gmail.com

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

#ifndef INCLUDE_DAGSHORTESTPATH_DAGSHORTESTPATH_HPP_
#define INCLUDE_DAGSHORTESTPATH_DAGSHORTESTPATH_HPP_
#pragma once

#include <deque>
#include <set>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>
#include <limits>
#include <map>
#include <cstdint>

#include <boost/config.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dag_shortest_paths.hpp>

#include "cpp_common/path.hpp"
#include "cpp_common/base_graph.hpp"
#include "cpp_common/interruption.hpp"

#include "c_types/ii_t_rt.h"

namespace pgrouting {

class Pgr_dag {
 public:
     using G = pgrouting::DirectedGraph;
     using V = typename G::V;
     using E = typename G::E;


     /** dag 1 to many */
     std::deque<Path> dag(
             G &graph,
             int64_t start_vertex,
             const std::set<int64_t> &end_vertex,
             bool only_cost) ;

     /** combinations */
     std::deque<Path> dag(
             G &graph,
             const std::map<int64_t, std::set<int64_t>> &combinations,
             bool only_cost) ;
 private:
     /** DAG  1 source to many targets */
     bool dag_1_to_many(
             G &graph,
             V source,
             const std::set<V> &targets,
             size_t n_goals = (std::numeric_limits<size_t>::max)()) ;
     void clear();


     //! @name members
     //@{
     std::vector<V> predecessors;
     std::vector< double > distances;
     std::deque<V> nodesInDistance;
     std::ostringstream log;
     //@}
};


namespace algorithms {

std::deque<pgrouting::Path>
dagShortestPath(
        pgrouting::DirectedGraph&graph,
        std::map<int64_t, std::set<int64_t>> &combinations,
        bool only_cost = false);

}  // namespace algorithms
}  // namespace pgrouting

#endif  // INCLUDE_DAGSHORTESTPATH_DAGSHORTESTPATH_HPP_
