/*PGR-GNU*****************************************************************

FILE: rule.h

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

#ifndef INCLUDE_TRSP_RULE_H_
#define INCLUDE_TRSP_RULE_H_


#include <vector>
#include <sstream>
#include "c_types/restriction_t.h"

namespace pgrouting {
namespace trsp {



class Rule {
 public:
    explicit Rule(Restriction_t r);

    inline double cost() const {
        return m_cost;
    }

    const std::vector<int64_t> precedencelist() const;

    inline int64_t dest_id() const {
        return m_dest_id;
    }


    friend std::ostream& operator<<(std::ostream& log, const Rule &r);

 private:
    int64_t m_dest_id;
    double m_cost;
    std::vector<int64_t> m_precedencelist;
};




}  // namespace trsp
}  // namespace pgrouting

#endif  // INCLUDE_TRSP_RULE_H_
