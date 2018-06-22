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
#include "yen/pgr_ksp.hpp"

#include <sstream>
#include <deque>
#include <vector>
#include <set>
#include <limits>

#include "cpp_common/pgr_assert.h"
#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/compPaths.h"
#include "cpp_common/pgr_messages.h"

#include "trsp/rule.h"
#include "c_types/line_graph_rt.h"

namespace pgrouting {
namespace yen {

template < typename G >
class Pgr_dijkstraTR : public Pgr_ksp< G > {
	 typedef std::set<Path, compPathsLess> pSet;
 public:
     struct found_goals{};
     class Myvisitor : public Pgr_ksp<G>::Visitor {
      public:
         Myvisitor(
                 pSet &solutions,
                 std::vector<trsp::Rule> &restrictions,
                 bool stop_on_first
                 ):
             m_stop_on_first(stop_on_first),
             m_solutions(solutions),
             m_restrictions(restrictions) {
                //pgassert(m_solutions.size()==1);
         }

         void on_insert_first_solution (const Path path) {
             if (path.empty()) return;
             if (has_restriction(path)) return;

             m_solutions.insert(path);

             if (m_stop_on_first) throw found_goals();
         };

         void on_insert_to_heap(const Path path) const {
             //pgassert(false);
             if (path.empty()) return;
             if (has_restriction(path)) return;

             m_solutions.insert(path);
             pgassert(m_solutions.size()==2);

             if (m_stop_on_first) {
                 throw found_goals();
             }
         }

      //private:
         bool has_restriction(const Path &path) const {
             for (const auto r :  m_restrictions) {
                 if (path.has_restriction(r)) {
                     return true;
                 }
             }

             return false;
         }

         bool m_stop_on_first;
         pSet &m_solutions;
         std::vector<trsp::Rule> &m_restrictions;
         size_t count;
     };


     std::deque<Path> dijkstraTR(
             G& graph,
             const std::vector<pgrouting::trsp::Rule> &restrictions,
             int64_t source,
             int64_t target,
             size_t k,
             bool heap_paths,
             bool stop_on_first,
             bool strict) {
        pgassert(m_restrictions.empty());
        pgassert(this->m_Heap.empty());
        pgassert(this->m_ResultSet.empty());

        this->log << std::string(__FUNCTION__) << "\n";


        m_stop_on_first = stop_on_first;
        m_heap_paths = heap_paths;
        m_strict = strict;
        m_restrictions = restrictions;

        this->log << "m_stop_on_first" << m_stop_on_first << "\n";

        return Yen(graph, source, target, k);
    }


     /*!
      * @param[in] start_vertex original id of vertex
      * @param[in] end_vertex original id of vertex
      * @param[in] K  when k=0 stop at first path without turn restriction
      * when k > 0 do K cycles and return best path without turn restriction
      */
     std::deque<Path> Yen(G &graph,
             int64_t  start_vertex,
             int64_t end_vertex,
             size_t K) {

        this->log << std::string(__FUNCTION__) << "\n";

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

         /*
          * clean up containers
          */
         clear();

         this->v_source = graph.get_V(start_vertex);
         this->v_target = graph.get_V(end_vertex);
         this->m_start = start_vertex;
         this->m_end = end_vertex;
         this->m_K = K;
         delete this->m_vis;
         this->m_vis = new Myvisitor(m_solutions, m_restrictions, m_stop_on_first);



         try {
             executeYen(graph);
         } catch(found_goals &) {
             pgassert(!m_solutions.empty());
             this->log << "On catch m_stop_on_first" << m_stop_on_first << "\n";
             std::deque<Path> solutions(m_solutions.begin(), m_solutions.end());
             return sort_results(solutions);
         } catch (boost::exception const& ex) {
             (void)ex;
             throw;
         } catch (std::exception &e) {
             (void)e;
             throw;
         } catch (...) {
             throw;
         }


         if (!m_solutions.empty()) {
             std::deque<Path> solutions(m_solutions.begin(), m_solutions.end());
             this->log << "outside of catch size" << m_solutions.size() << "\n";
             return sort_results(solutions);
         }

         auto solutions = Pgr_ksp<G>::get_results();

         solutions = inf_cost_on_restriction(solutions);

         return get_results(solutions);
     }

 public:
 private:
     std::deque<Path> sort_results(
             std::deque<Path> &paths) {
         if (paths.empty()) return paths;

         std::stable_sort(paths.begin(), paths.end(),
                 [](const Path &left, const Path &right) -> bool {
                 return (left.countInfinityCost() < right.countInfinityCost());
                 });
         return paths;
     }

     std::deque<Path> get_results (
             std::deque<Path> &paths) {
         if (paths.empty()) return paths;
         if (m_strict) return std::deque<Path>();

         paths = sort_results(paths);
         if (m_heap_paths) return paths;
         auto count = paths.begin()->countInfinityCost();

         paths.erase(
                 std::remove_if(
                     paths.begin(), paths.end(),
                     [&count](const Path &p){return count != p.countInfinityCost();}
                     ),
                 paths.end());

         return paths;
     }



     void executeYen(G &graph) {
         this->log << std::string(__FUNCTION__) << "\n";

         this->curr_result_path = this->getFirstSolution(graph);
         On_insert_first_solution(this->curr_result_path);

         if (this->m_ResultSet.size() == 0) return;  // no path found

         this->log << "m_stop_on_first" << m_stop_on_first << "\n";
         Myvisitor vis(m_solutions, m_restrictions, m_stop_on_first);
         while (this->m_ResultSet.size() < this->m_K) {

             Pgr_ksp< G >::doNextCycle(graph);

             if (this->m_Heap.empty()) break;
             this->curr_result_path = *this->m_Heap.begin();
             this->m_ResultSet.insert(this->curr_result_path);
             this->m_Heap.erase(this->m_Heap.begin());
         }
     }

     /** \brief containers cleanup
      *
      * empties containers
      */
     void clear() {
         Pgr_ksp<G>::clear();
         m_solutions.clear();
     }


     /*! adds to the solution set if the path has no restriction
      *
      * path is added when there is a path and has no restriction
      *
      * @param[in] path to add
      */
     void On_insert_first_solution (const Path path) {
             if (path.empty()) return;
             if (has_restriction(path)) return;

             this->log << "inserting first solution";
             m_solutions.insert(path);

             if (m_stop_on_first) throw found_goals();
     };
#if 0
     void on_insert_to_heap (const Path path) {
             if (path.empty()) return;
             if (has_restriction(path)) return;

             m_solutions.insert(path);

             if (m_stop_on_first) throw found_goals();
         }
#endif

     bool has_restriction(const Path &path) const {
         for (const auto r :  m_restrictions) {
             if (path.has_restriction(r)) {
                 return true;
             } else {
             }
         }

         return false;
     }

     /*! sets an inf value on agg_cost on the vertex/edge where the restriction begins
      *
      * @params[in] path that is being analized
      */
     Path inf_cost_on_restriction(Path &path) {
         this->log << std::string(__FUNCTION__) << "\n";
         for (const auto r : m_restrictions) {
             path = path.inf_cost_on_restriction(r);
         }
         return path;
     }

     std::deque<Path> inf_cost_on_restriction(std::deque<Path> &paths) {
         this->log << std::string(__FUNCTION__) << "\n";
         if (paths.empty()) return paths;
         for (auto &p : paths) {
             p = inf_cost_on_restriction(p);
         }
         return paths;
     }


 private:
	 std::vector<pgrouting::trsp::Rule> m_restrictions;
	 bool m_strict;

	 //! ordered set of shortest paths
     pSet m_solutions;

     bool m_stop_on_first;
     bool m_heap_paths;


};

}  // namespace yen
}  // namespace pgrouting



#endif  // INCLUDE_DIJKSTRATR_PGR_DIJKSTRATR_HPP_
