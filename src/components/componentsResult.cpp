/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2017 Maoguang Wang
Mail: xjtumg1007@gmail.com

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

#include "components/componentsResult.h"

#include <vector>
#include <algorithm>

namespace pgrouting {
namespace algorithms {

namespace detail {

std::vector<pgr_components_rt>
componentsResult(
        std::vector< std::vector< int64_t > > components) {
    // sort identifier
    size_t num_comps = components.size();
    for (size_t i = 0; i < num_comps; i++) {
        std::sort(components[i].begin(), components[i].end());
    }
    sort(components.begin(), components.end());

    // generate results
    std::vector< pgr_components_rt > results;
    for (size_t i = 0; i < num_comps; i++) {
        int64_t tempComp = components[i][0];
        size_t sizeCompi = components[i].size();
        for (size_t j = 0; j < sizeCompi; j++) {
            pgr_components_rt tmp;
            tmp.identifier = components[i][j];
            tmp.n_seq = static_cast< int > (j + 1);
            tmp.component = tempComp;
            results.push_back(tmp);
        }
    }
    return results;
}

}  // namespace detail

}  // namespace algorithms
}  // namespace pgrouting
