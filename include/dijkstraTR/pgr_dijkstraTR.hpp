/*PGR-GNU*****************************************************************
File: pgr_dijkstraTR.hpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2017 Vidhan Jain
Mail: vidhanj1307@gmail.com

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
#ifndef INCLUDE_DIJKSTRATR_PGR_DIJKSTRATR_HPP_
#define INCLUDE_DIJKSTRATR_PGR_DIJKSTRATR_HPP_
#pragma once

#include "dijkstra/pgr_dijkstra.hpp"

#include <sstream>
#include <deque>
#include <vector>
#include <set>
#include <limits>

#include "cpp_common/pgr_assert.h"
#include "cpp_common/basePath_SSEC.hpp"

#include "trsp/rule.h"
#include "c_types/line_graph_rt.h"
#include "lineGraph/pgr_lineGraph.hpp"


template < class G >
class Pgr_dijkstraTR {
 public:
     Path dijkstraTR(
             G& graph,
             const std::vector<pgrouting::trsp::Rule> &restrictions,
             int64_t source,
             int64_t target,
             bool only_cost,
             bool strict) {
         if (source == target) {
             return Path();
         }

         if (!graph.has_vertex(source) || !graph.has_vertex(target)) {
             return Path();
         }

         v_source = graph.get_V(source);
         v_target = graph.get_V(target);

         m_start = source;
         m_end = target;
         m_restrictions = restrictions;
         m_strict = strict;
         m_only_cost = only_cost;

         return executeDijkstraTR(graph);
     }

     void clear() {}

/*
 * IDEA

select * FROM pgr_withPoints('SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id !=10 UNION select id, source, target, cost , -1 from edge_table WHERE id = 10', $$select id as edge_id, 'r'::CHAR AS side, 0.000001::FLOAT as fraction FROM edge_table WHERE id = 10$$, -1, 5);
 seq | path_seq | node | edge |   cost   | agg_cost
-----+----------+------+------+----------+----------
   1 |        1 |   -1 |   10 | 0.999999 |        0
   2 |        2 |   10 |   12 |        1 | 0.999999
   3 |        3 |   11 |   13 |        1 | 1.999999
   4 |        4 |   12 |   15 |        1 | 2.999999
   5 |        5 |    9 |    9 |        1 | 3.999999
   6 |        6 |    6 |    8 |        1 | 4.999999
   7 |        7 |    5 |   -1 |        0 | 5.999999
(7 rows)
*/
 private:
     Path executeDijkstraTR(G& graph) {
         clear();
         curr_result_path = getDijkstraSolution(graph);
         log << curr_result_path;

         log << "restrictions\n";
         for (auto r : m_restrictions) {
             log << r << "-";
             log << curr_result_path.has_restriction(r) << "\n";
         }

         curr_result_path = get_no_restrictions_path(curr_result_path);
         return curr_result_path;
     }


     Path getDijkstraSolution(G& graph) {
         Pgr_dijkstra< G > fn_dijkstra;
         return  fn_dijkstra.dijkstra(graph, m_start, m_end);
     }

     Path get_no_restrictions_path(Path &path) {
         auto r = m_restrictions.begin();
         while (r != m_restrictions.end() && path.find_restriction(*r) == path.end()) {
             ++r;
         }

         /*
          * There is no restriction found
          */
         if (r == m_restrictions.end()) return path;

         log << "Found Restriction " << (*r) << "\n";
         return path;
     }


     bool has_a_restriction(int64_t edge, int64_t index) {
         return false;
     }

 private:
     typedef typename G::V V;
     V v_source;
     V v_target;
     int64_t m_start;
     int64_t m_end;
     std::vector<pgrouting::trsp::Rule> m_restrictions;
     std::vector< int64_t > m_edges_in_path;
     bool m_only_cost;
     bool m_strict;
     Path curr_result_path;

 public:
     std::ostringstream log;
};



#endif  // INCLUDE_DIJKSTRATR_PGR_DIJKSTRATR_HPP_
