/*PGR-GNU*****************************************************************
File: details.hpp

Copyright (c) 2018 Vicky Vergara


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

#include "spanningTree/details.hpp"
#include <vector>
#include <algorithm>

namespace pgrouting {
namespace details {

std::vector<int64_t>
clean_vids(std::vector<int64_t> vids) {
    std::sort(vids.begin(), vids.end());
    vids.erase(
            std::unique(vids.begin(), vids.end()),
            vids.end());
    vids.erase(
            std::remove(vids.begin(), vids.end(), 0),
            vids.end());
    return vids;
}

std::vector<pgr_mst_rt>
get_no_edge_graph_result(
        std::vector<int64_t> vids) {
    std::vector<pgr_mst_rt> results;
    if (vids.empty()) return results;
    for (auto const root : clean_vids(vids)) {
        results.push_back({root, 0, root, -1, 0.0, 0.0});
    }
    return results;
}

}  // namespace details
}  // namespace pgrouting
