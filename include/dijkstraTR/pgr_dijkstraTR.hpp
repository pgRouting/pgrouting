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

#if 0
         for (auto &path : curr_result_path) {
             m_edges_in_path.push_back(path.edge);
         }
         while (m_edges_in_path.size() && m_edges_in_path.back() == -1) {
             m_edges_in_path.pop_back();
         }

         log << "Edges in m_edges_in_path:-------------------\n";
         for (auto &it : m_edges_in_path) log << it << "\n";
         log << "---------------------------------------------\n";
         bool sol = has_restriction();
         log << "Result of valid solution: " << sol << "\n";
         if (sol) curr_result_path = Path();
#endif
         return curr_result_path;
     }


     Path getDijkstraSolution(G& graph) {
         Pgr_dijkstra< G > fn_dijkstra;
         return  fn_dijkstra.dijkstra(graph, m_start, m_end);
     }

     bool has_restriction() {
#if 0
         auto sort_cmp = [](const pgrouting::trsp::Rule& left,
                 const pgrouting::trsp::Rule& right) -> bool {
             return left.restrict_edges()[0] <= right.restrict_edges()[0];
         };
         std::stable_sort(m_restrictions.begin(), m_restrictions.end(),
                 sort_cmp);

         log << "\nRestriction array after sorting.\n";
         for (auto &it : m_restrictions) log << it << "\n";
         log << "\nEnd\n";

         size_t index = 0;
         for (auto &edge : m_edges_in_path) {
             if (has_a_restriction(edge, index))
                 return true;
             index++;
         }
#endif
         return false;
     }

#if 0
     Path_t* has_restriction(Path path, pgrouting::trsp::Rule rule) {
         return std::search(path.begin(), path.end(), rule.begin(), rule.end(),
                 [](Path_t p, int64_t e) { return p.edge = e;}
                 );
     }

#endif


     bool has_a_restriction(int64_t edge, int64_t index) {
#if 0
         auto lower_bound_cmp = [](
                 const Restriction& r, const int64_t& target) {
             return r.restrict_edges()[0] < target;
         };
         auto edge_index = std::lower_bound(m_restrictions.begin(),
                 m_restrictions.end(), edge,
                 lower_bound_cmp) - m_restrictions.begin();
         log << "\nResult generated from lower_bound\n";
         while (edge_index < (int64_t)m_restrictions.size()) {
             auto r_edges = m_restrictions[edge_index].restrict_edges();
             if (r_edges[0] != edge) break;
             log << m_restrictions[edge_index] << "\n";
             bool okay = true;
             size_t temp_edge_index = index;

             for (auto &edge_id : r_edges) {
                 if (temp_edge_index >= m_edges_in_path.size() ||
                         m_edges_in_path[temp_edge_index] != edge_id) {
                     okay = false;
                     break;
                 }
                 temp_edge_index++;
             }
             log << "\nokay value = " << okay <<"\n";
             if (okay) return true;
             edge_index++;
         }
         log << "Ends Here\n";
#endif
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
