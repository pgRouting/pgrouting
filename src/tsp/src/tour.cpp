/*PGR-GNU*****************************************************************

FILE:

Copyright (c) 2015 pgRouting developers
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
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 ********************************************************************PGR-GNU*/

#include "tsp/tour.h"

#include <algorithm>

#include "cpp_common/pgr_assert.h"


namespace pgrouting {
namespace tsp {

std::ostream& operator<<(
        std::ostream &log,
        const Tour &tour) {
    for (const auto &city : tour.cities) {
        log << city << ", ";
    }
    return log;
}



void Tour::reverse(
        size_t c1,
        size_t c2) {
    pgassert(c1 < c2 && c2 < cities.size());
    std::reverse(
            cities.begin() + (c1 + 1),
            cities.begin() + (c2 + 1));
}

void Tour::slide(
        size_t place,
        size_t first,
        size_t last) {
    pgassert(first < cities.size());
    pgassert(last  < cities.size());
    pgassert(place < cities.size());

    if (place < first) {
        std::rotate(
                cities.begin() + (place + 1),
                cities.begin() + (first + 1),
                cities.begin() + (last + 1));
    } else {
        std::rotate(
                cities.begin() + (first + 1),
                cities.begin() + (last + 1),
                cities.begin() + (place + 1));
    }
}

void Tour::rotate(
        size_t c1,
        size_t c2,
        size_t c3) {
    pgassert(c1 < c2 && c2 < c3 && c3 < cities.size());

    std::rotate(
            cities.begin() + (c1 + 1),
            cities.begin() + (c2 + 1),
            cities.begin() + (c3 + 1));
}


void Tour::swap(
        size_t c1,
        size_t c2 ) {
    pgassert(c1 < c2);

    std::iter_swap(cities.begin() + c1, cities.begin() + c2);
}


}  // namespace tsp
}  // namespace pgrouting
