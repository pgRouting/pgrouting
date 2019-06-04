/*PGR-GNU*****************************************************************
File: pgr_breadthFirstSearch.hpp

Copyright (c) 2019 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2019 Gudesa Venkata Sai AKhil
Mail: gvs.akhil1997@gmail.com

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

#ifndef INCLUDE_MST_PGR_BREADTHFIRSTSEARCH_HPP_
#define INCLUDE_MST_PGR_BREADTHFIRSTSEARCH_HPP_
#pragma once

#include <vector>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/pgr_messages.h"
//******************************************

namespace pgrouting
{
namespace functions
{

template <class G>
class Pgr_breadthFirstSearch
{
    typedef typename G::V V;
    typedef typename G::E E;
    typedef typename G::B_G B_G;

public:

    // 1 to depth
    std::vector<pgr_mst_rt> breadthFirstSearch(
        G &graph,
        int64_t start_vertex,
        int64_t depth){

        pgr_mst_rt dummy = {0, 0, 0, 0, 0.0, 0.0};
        std::vector<pgr_mst_rt> dummyResults = {dummy};
        return dummyResults;

        }

    //many to depth
    std::vector<pgr_mst_rt> breadthFirstSearch(
        G &graph,
        std::vector<int64_t> start_vertex,
        int64_t depth){

        pgr_mst_rt dummy = {0, 0, 0, 0, 0.0, 0.0};
        std::vector<pgr_mst_rt> dummyResults = {dummy};
        return dummyResults;

        }
    


private:
    // Functions

};
} // namespace functions
} // namespace pgrouting

#endif // INCLUDE_MST_PGR_BREADTHFIRSTSEARCH_HPP_
