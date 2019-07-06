/*PGR-GNU*****************************************************************
File: pgr_binaryBreadthFirstSearch.hpp

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

#ifndef INCLUDE_MST_PGR_BINARYBREADTHFIRSTSEARCH_HPP_
#define INCLUDE_MST_PGR_BINARYBREADTHFIRSTSEARCH_HPP_
#pragma once

#include <deque>
#include <algorithm>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"
//******************************************

namespace pgrouting
{
namespace functions
{

template <class G>
class Pgr_binaryBreadthFirstSearch
{
public:
    typedef typename G::V V;
    typedef typename G::E E;
    typedef typename G::B_G B_G;

    std::deque<Path> binaryBreadthFirstSearch(
        G &graph,
        std::vector<int64_t> start_vertex,
        std::vector<int64_t> end_vertex) {

        std::deque<Path> paths;

        for(auto source : start_vertex) {
            std::deque<Path> result_paths = one_to_many_binaryBreadthFirstSearch(
                graph,
                source,
                end_vertex
            );

            paths.insert(
                paths.begin(), 
                std::make_move_iterator(result_paths.begin()),
                std::make_move_iterator(result_paths.end()));
        }

        std::sort(paths.begin(), paths.end(),
                  [](const Path &e1, const Path &e2) -> bool {
                      return e1.end_id() < e2.end_id();
                  });
        std::stable_sort(paths.begin(), paths.end(),
                         [](const Path &e1, const Path &e2) -> bool {
                             return e1.start_id() < e2.start_id();
                         });

        return paths;
    }

    private:

    std::deque<Path> one_to_many_binaryBreadthFirstSearch(
        G &graph,
        int64_t start_vertex,
        std::vector<int64_t> end_vertex) {

        std::deque<Path> paths;
    
        return paths;
    }
};
} // namespace functions
} // namespace pgrouting

#endif // INCLUDE_MST_PGR_BINARYBREADTHFIRSTSEARCH_HPP_
