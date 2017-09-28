/*PGR-GNU*****************************************************************

FILE: edgeInfo.h

Copyright (c) 2011 pgRouting developers
Mail: project@pgrouting.org

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
aint64_t with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 ********************************************************************PGR-GNU*/

#ifndef INCLUDE_TRSP_EDGEINFO_H_
#define INCLUDE_TRSP_EDGEINFO_H_

#include <vector>


#include "c_types/pgr_edge_t.h"
#include "cpp_common/pgr_assert.h"

namespace pgrouting {
namespace trsp {


class EdgeInfo {
 public:
     EdgeInfo(
             pgr_edge_t edgeIn,
             size_t edgeIndex);

     inline size_t edgeIndex() const {return m_lEdgeIndex;}

     inline int64_t startNode() const {
         pgassert(m_edge.source >= 0);
         return m_edge.source;
     }
     inline int64_t endNode() const {
         pgassert(m_edge.target >= 0);
         return m_edge.target;}
     inline int64_t edgeID() const {return m_edge.id;}
     inline double cost() const {return m_edge.cost;}
     inline double r_cost() const {return m_edge.reverse_cost;}

     std::vector<int64_t>& endConnedtedEdge() {return m_vecEndConnedtedEdge;}
     std::vector<int64_t>& startConnectedEdge() {return m_vecStartConnectedEdge;}


 private:
     pgr_edge_t m_edge;
     size_t m_lEdgeIndex;
     std::vector<int64_t> m_vecStartConnectedEdge;
     std::vector<int64_t> m_vecEndConnedtedEdge;
};

}  // namespace trsp
}  // namespace pgrouting

#endif  // INCLUDE_TRSP_EDGEINFO_H_
