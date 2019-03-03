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
#ifndef INCLUDE_YEN_PGR_TURNRESTRICTEDPATH_HPP_
#define INCLUDE_YEN_PGR_TURNRESTRICTEDPATH_HPP_
#pragma once

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
#include "cpp_common/rule.h"
#include "c_types/line_graph_rt.h"

namespace pgrouting {
namespace yen {

template < typename G >
class Pgr_turnRestrictedPath : public Pgr_ksp< G > {
     typedef std::set<Path, compPathsLess> pSet;

 public:
     Pgr_turnRestrictedPath() = default;
     struct found_goals{};
     class Myvisitor : public Pgr_ksp<G>::Visitor {
      public:
         Myvisitor(
                 pSet &solutions,
                 std::vector<trsp::Rule> &restrictions,
                 bool stop_on_first):
             m_stop_on_first(stop_on_first),
             m_solutions(solutions),
             m_restrictions(restrictions) {
             }

         void on_insert_first_solution(const Path path) const {
             if (path.empty()) return;
             if (has_restriction(path)) return;

             m_solutions.insert(path);

             if (m_stop_on_first) throw found_goals();
         }

         void on_insert_to_heap(const Path path) const {
             if (path.empty()) return;
             if (has_restriction(path)) return;

             m_solutions.insert(path);

             if (m_stop_on_first) {
                 throw found_goals();
             }
         }

      private:
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
     };


     std::deque<Path> turnRestrictedPath(
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

        m_stop_on_first = stop_on_first;
        m_strict = strict;
        m_restrictions = restrictions;
        m_heap_paths = heap_paths;

        return Yen(graph, source, target, k);
    }


     /*!
      * @param[in] graph
      * @param[in] start_vertex original id of vertex
      * @param[in] end_vertex original id of vertex
      * @param[in] K  when k=0 stop at first path without turn restriction
      * when k > 0 do K cycles and return best path without turn restriction
      */
     std::deque<Path> Yen(G &graph,
             int64_t  start_vertex,
             int64_t end_vertex,
             size_t K) {
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
         Pgr_ksp<G>::m_heap_paths = true;
         delete this->m_vis;
         this->m_vis = new Myvisitor(
                 m_solutions,
                 m_restrictions,
                 m_stop_on_first);



         try {
              Pgr_ksp< G >::executeYen(graph);
         } catch(found_goals &) {
             pgassert(!m_solutions.empty());
             std::deque<Path> solutions(m_solutions.begin(), m_solutions.end());
             return solutions;
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
             return solutions;
         }

         auto solutions = Pgr_ksp<G>::get_results();

         return get_results(solutions);
     }

 private:
     std::deque<Path> get_results(
             std::deque<Path> &paths) {
         if (paths.empty()) return paths;
         if (m_strict) return std::deque<Path>();

         paths = inf_cost_on_restriction(paths);

         std::stable_sort(paths.begin(), paths.end(),
                 [](const Path &left, const Path &right) -> bool {
                 return (left.countInfinityCost() < right.countInfinityCost());
                 });

         auto count = paths.begin()->countInfinityCost();


         if (m_heap_paths) return paths;

         paths.erase(
                 std::remove_if(
                     paths.begin(), paths.end(),
                     [&count](const Path &p){
                     return count != p.countInfinityCost();
                     }),
                 paths.end());

         return paths;
     }


     /** \brief containers cleanup
      *
      * empties containers
      */
     void clear() {
         Pgr_ksp<G>::clear();
         m_solutions.clear();
     }



     /*! sets an inf value on agg_cost on the vertex/edge where the restriction begins
      *
      * @param[in] paths that is being analized
      */
     std::deque<Path> inf_cost_on_restriction(std::deque<Path> &paths) {
         if (paths.empty()) return paths;
         for (auto &p : paths) {
             for (const auto r : m_restrictions) {
                 p = p.inf_cost_on_restriction(r);
             }
         }
         return paths;
     }


 private:
     std::vector<pgrouting::trsp::Rule> m_restrictions;
     bool m_strict;
     pSet m_solutions;
     bool m_stop_on_first;
     bool m_heap_paths;
};

}  // namespace yen
}  // namespace pgrouting



#endif  // INCLUDE_YEN_PGR_TURNRESTRICTEDPATH_HPP_
