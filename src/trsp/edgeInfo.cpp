/*PGR-GNU*****************************************************************

FILE: edgeInfo.cpp

Copyright (c) 2017 pgRouting developers
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

#include "trsp/edgeInfo.h"


namespace pgrouting {
namespace trsp {

EdgeInfo::EdgeInfo(
            pgr_edge_t edgeIn,
            size_t edgeIndex) :
        m_edge(edgeIn),
        m_edgeIndex(edgeIndex),
        m_startConnectedEdge(),
        m_endConnectedEdge()

        {}

}  // namespace trsp
}  // namespace pgrouting

