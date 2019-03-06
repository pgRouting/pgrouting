/*PGR-GNU*****************************************************************
File: compPaths.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2017 Vidhan Jain
Mail: vidhanj1307@gmail.com

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

#include <cmath>
#include <limits>

#include "cpp_common/pgr_assert.h"
#include "cpp_common/compPaths.h"
#include "cpp_common/basePath_SSEC.hpp"


namespace pgrouting {

bool compPathsLess::operator()(const Path &p1, const Path &p2) const {
    /*
     * less cost is best
     */
    if (!(std::fabs(p2.tot_cost() - p1.tot_cost())
                <
                std::numeric_limits<double>::epsilon())) {
    if (p1.tot_cost() > p2.tot_cost())
        return  false;
    if (p1.tot_cost() < p2.tot_cost())
        return  true;
    }

    // paths costs are equal now check by length
    if (p1.size() > p2.size())
        return false;
    if (p1.size() < p2.size())
        return true;

//    pgassert(p1.tot_cost() == p2.tot_cost());
    pgassert(p1.size() == p2.size());

    // paths weights & lengths are equal now check by node ID
    unsigned int i;
    for (i = 0; i < p1.size(); i++) {
        if (p1[i].node >  p2[i].node)
            return false;
        if (p1[i].node <  p2[i].node)
            return true;
    }

//    pgassert(p1.tot_cost() == p2.tot_cost());
    pgassert(p1.size() == p2.size());
#ifdef NDEBUG
    for (i = 0; i < p1.size(); i++) {
        pgassert(p1[i].node == p2[i].node);
    }
#endif

    // we got here and everything is equal
    return false;
}

}  // namespace pgrouting
