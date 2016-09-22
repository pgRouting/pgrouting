/*PGR-GNU*****************************************************************
File: pgr_ksp.hpp

Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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

#pragma once

#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#ifdef unlink
#undef unlink
#endif
#endif

#include <sstream>
#include <deque>
#include <vector>
#include <set>
#include "./../../dijkstra/src/pgr_dijkstra.hpp"
#include "./../../common/src/pgr_assert.h"
#include "./../../common/src/basePath_SSEC.hpp"
#include <limits>

template < class G >
class Pgr_ksp {
 public:
     std::deque<Path> Yen(G &graph, int64_t source, int64_t target, int K, bool heap_paths);
     void clear();

 private:
     class compPaths {
      public:
          bool operator()(const Path &p1, const Path &p2) const {
              auto EPSILON = std::numeric_limits<double>::epsilon();
              /*
               * less cost is best
               */
              if (!(std::fabs(p1.tot_cost() - p2.tot_cost()) < EPSILON))
                  return  (p1.tot_cost() < p2.tot_cost());

              pgassert(std::fabs(p1.tot_cost() - p2.tot_cost()) < EPSILON);

              // paths costs are equal now check by length
              if (p1.size() != p2.size())
                  return (p1.size() < p2.size());

              pgassert(p1.tot_cost() == p2.tot_cost());
              pgassert(p1.size() == p2.size());

              // paths weights & lengths are equal now check by node ID
              unsigned int i;
              for (i = 0; i < p1.size(); i++) {
                  if (p1[i].node != p2[i].node)
                      return (p1[i].node < p2[i].node);
              }

              // we got here and everything is equal
              return false;
          }
     };

     //! the actual algorithm
     void executeYen(G &graph, int top_k);

     /** @name Auxiliary function for yen's algorithm */
     ///@{

     //! Performs the first Dijkstra of the algorithm
     void getFirstSolution(G &graph);
     //! Performs the next cycle of the algorithm
     void doNextCycle(G &graph);
     //! stores in subPath the first i elements of path
     void removeVertices(G &graph, const Path &path);
     ///@}

 private:
     /** @name members */
     ///@{
     typedef typename G::V V;
     V v_source;  //!< source descriptor
     V v_target;  //!< target descriptor
     int64_t m_start;  //!< source id
     int64_t m_end;   //!< target id

     Path curr_result_path;  //!< storage for the current result

     typedef std::set<Path, compPaths> pSet;
     pSet m_ResultSet;  //!< ordered set of shortest paths
     pSet m_Heap;  //!< the heap
     std::ostringstream log;
};

#include "./pgr_ksp.cpp"

