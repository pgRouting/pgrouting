/*PGR

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

*/

#ifndef SRC_KSP_SRC_PGR_KSP_HPP_
#define SRC_KSP_SRC_PGR_KSP_HPP_

#include <deque>
#include <set>
#include "./../../common/src/basePath_SSEC.hpp"
#include "./../../dijkstra/src/pgr_dijkstra.hpp"

template < class G >
class Pgr_ksp {
 public:
#if 0
        explicit Pgr_ksp(graphType gtype, const int initial_size)
                : Pgr_dijkstra <G>(gtype, initial_size)
           {}

        ~Pgr_ksp(void) {}
#endif
        std::deque<Path> Yen(G &graph, int64_t source, int64_t target, int K);
        void clear();

 private:
    class compPaths {
     public:
        bool operator()(const Path &p1, const Path  &p2) const {
          if (p1.cost < p2.cost) return true;
          if (p1.cost > p2.cost) return false;

          // paths costs are equal now we check by length
          if (p1.path.size()  < p2.path.size()) return true;
          if (p1.path.size()  > p2.path.size()) return false;

          // paths weights & lengths are equal now we check by ID
          unsigned int i;
          for ( i = 0; i < p1.path.size() ; i++)
               if ( p1.path[i].vertex < p2.path[i].vertex) return true;

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

    typedef typename  std::set<Path, compPaths> pSet;
    typedef typename  std::set<Path, compPaths>::iterator pSet_i;
    pSet m_ResultSet;  //!< ordered set of shortest paths
    pSet m_Heap;  //!< the heap
    pSet_i pIt;   //!< iterator for heap and result set
};

#include "./pgr_ksp.cpp"

#endif  // SRC_KSP_SRC_PGR_KSP_HPP_
