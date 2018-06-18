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
     std::deque<Path> dijkstraTR(
             G& graph,
             const std::vector<pgrouting::trsp::Rule> &restrictions,
             int64_t source,
             int64_t target,
             bool only_cost,
             bool strict) {
         if (source == target) {
             return std::deque<Path>();
         }

         if (!graph.has_vertex(source) || !graph.has_vertex(target)) {
             return std::deque<Path>();
         }

         v_source = graph.get_V(source);
         v_target = graph.get_V(target);

         m_start = source;
         m_end = target;
         m_restrictions = restrictions;
         m_strict = strict;
         m_only_cost = only_cost;

         return Yen(graph, source, target, 10, true);
     }


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
	class compPaths {
     public:
		 bool operator()(const Path &p1, const Path &p2) const {
			 /*
			  * less cost is best
			  */
			 if (p1.tot_cost() > p2.tot_cost())
				 return  false;
			 if (p1.tot_cost() < p2.tot_cost())
				 return  true;

              pgassert(p1.tot_cost() == p2.tot_cost());

              // paths costs are equal now check by length
              if (p1.size() > p2.size())
                  return false;
              if (p1.size() < p2.size())
                  return true;

              pgassert(p1.tot_cost() == p2.tot_cost());
              pgassert(p1.size() == p2.size());

              // paths weights & lengths are equal now check by node ID
              unsigned int i;
              for (i = 0; i < p1.size(); i++) {
                  if (p1[i].node >  p2[i].node)
                      return false;
                  if (p1[i].node <  p2[i].node)
                      return true;
              }

              pgassert(p1.tot_cost() == p2.tot_cost());
              pgassert(p1.size() == p2.size());
#ifdef NDEBUG
              for (i = 0; i < p1.size(); i++) {
                  pgassert(p1[i].node == p2[i].node);
              }
#endif

              // we got here and everything is equal
              return false;
          }
     };

     /*!
      * @param[in] start_vertex original id of vertex
      * @param[in] end_vertex original id of vertex
      * @param[in] K  when k=0 stop at first path without turn restriction
      * when k > 0 do K cycles and return best path without turn restriction
      */
     std::deque<Path> Yen(G &graph,
             int64_t  start_vertex,
             int64_t end_vertex,
             int K,
             bool heap_paths) {
         /*
          * No path: already in destination
          */
         if ((start_vertex == end_vertex)) {
             return std::deque<Path>();
         }

         /*
          * no path: disconnected vertices
          */
         if (!graph.has_vertex(start_vertex)
                 || !graph.has_vertex(end_vertex)) {
             return std::deque<Path>();
         }

         m_ResultSet.clear();
         m_Heap.clear();

         v_source = graph.get_V(start_vertex);
         v_target = graph.get_V(end_vertex);
         m_start = start_vertex;
         m_end = end_vertex;
         executeYen(graph, K);

         while (!m_ResultSet.empty()) {
             m_Heap.insert(*m_ResultSet.begin());
             m_ResultSet.erase(m_ResultSet.begin());
         }

         std::deque<Path> l_ResultList(m_Heap.begin(), m_Heap.end());

#if 0
         std::stable_sort(l_ResultList.begin(), l_ResultList.end(),
                 [](const Path &left, const Path &right) -> bool {
                 for (size_t i = 0;
                         i < (std::min)(left.size(), right.size());
                         ++i) {
                 if (left[i].node < right[i].node) return true;
                 if (left[i].node > right[i].node) return false;
                 }
                 return false;
                 });

         std::stable_sort(l_ResultList.begin(), l_ResultList.end(),
                 [](const Path &left, const Path &right) {
                 return left.size() < right.size();});
#endif

         if (!heap_paths && l_ResultList.size() > (size_t) K)
             l_ResultList.resize(K);

         l_ResultList = inf_cost_on_restriction(l_ResultList);

         return l_ResultList;
     }

     void executeYen(G &graph, int K) {
         clear();
         curr_result_path = getFirstSolution(graph);
         add_to_solution_set(curr_result_path);

         if (m_ResultSet.size() == 0) return;  // no path found

         while (m_ResultSet.size() < (unsigned int) K) {
             doNextCycle(graph);
             if (m_Heap.empty()) break;
             curr_result_path = *m_Heap.begin();
             m_ResultSet.insert(curr_result_path);
             m_Heap.erase(m_Heap.begin());
             /*
              * without the next line withpointsKSP hungs with:
              * c++ 4.6
              * Debug mode
              */
#ifndef NDEBUG
             log << "end of while heap size" << m_Heap.size();
#endif
         }
     }

     Path getFirstSolution(G &graph) {
         Pgr_dijkstra< G > fn_dijkstra;
         auto path = fn_dijkstra.dijkstra(graph, m_start, m_end);

         if (path.empty()) return path;
         m_ResultSet.insert(path);
		 return path;
     }

     void clear() {
         m_Heap.clear();
         m_ResultSet.clear();
         m_pathWithNoRestrictions.clear();
     }


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

     /*! adds to the solution set if the path has no restriction
      *
      * path is added when there is a path and has no restriction
      *
      * @param[in] path to add
      */
     void add_to_solution_set(const Path &path) {
         if (path.empty()) return;
         if (has_restriction(path)) return;

         m_pathWithNoRestrictions.insert(path);
     }



     /*! sets an inf value on agg_cost on the vertex/edge where the restriction begins
      *
      * @params[in] path that is being analized
      */
     Path inf_cost_on_restriction(Path &path) {
         for (const auto r : m_restrictions) {
             path = path.inf_cost_on_restriction(r);
         }
         return path;
     }

     std::deque<Path> inf_cost_on_restriction(std::deque<Path> &paths) {
         for (auto &p : paths) {
             p = inf_cost_on_restriction(p);
         }
         return paths;
     }

     bool has_restriction(const Path &path) const {
         auto r = m_restrictions.begin();
         while (r != m_restrictions.end() && path.has_restriction(*r)) {
             ++r;
         }

         return r != m_restrictions.end();
     }

	 void doNextCycle(G &graph) {
		 int64_t spurNodeId;


		 for (unsigned int i = 0; i < curr_result_path.size(); ++i) {
			 spurNodeId = curr_result_path[i].node;

			 auto rootPath = curr_result_path.getSubpath(i);

			 for (const auto &path : m_ResultSet) {
				 if (path.isEqual(rootPath)) {
					 if (path.size() > i + 1) {
						 graph.disconnect_edge(path[i].node,     // from
								 path[i + 1].node);  // to
					 }
				 }
			 }

			 removeVertices(graph, rootPath);

			 Pgr_dijkstra< G > fn_dijkstra;
			 auto spurPath = fn_dijkstra.dijkstra(graph, spurNodeId, m_end);

			 if (spurPath.size() > 0) {
				 rootPath.appendPath(spurPath);
				 m_Heap.insert(rootPath);
                 add_to_solution_set(rootPath);
			 }

			 graph.restore_graph();
		 }
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

	 void removeVertices(G &graph, const Path &subpath) {
		 for (const auto &e : subpath)
			 graph.disconnect_vertex(e.node);
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

	 typedef std::set<Path, compPaths> pSet;
	 pSet m_ResultSet;  //!< ordered set of shortest paths
	 pSet m_Heap;  //!< the heap

	 //! ordered set of shortest paths
	 pSet m_pathWithNoRestrictions;

 public:
	 std::ostringstream log;
};



#endif  // INCLUDE_DIJKSTRATR_PGR_DIJKSTRATR_HPP_
