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
        std::vector< std::vector< int64_t > > &components) {
    // sort identifier
    for (auto &component : components) {
        std::sort(component.begin(), component.end());
    }
    sort(components.begin(), components.end());

    // generate results
    std::vector< pgr_components_rt > results;
    for (const auto component : components) {
        auto component_id = component[0];
        for (const auto element : component) {
            results.push_back({component_id, element});
        }
    }
    return results;
}

}  // namespace detail

}  // namespace algorithms
}  // namespace pgrouting
