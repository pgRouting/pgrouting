/*PGR-GNU*****************************************************************

FILE: rule.cpp

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
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 ********************************************************************PGR-GNU*/

#include "cpp_common/rule.h"

#include <vector>
#include <algorithm>

#include "c_types/restriction_t.h"


namespace pgrouting {
namespace trsp {

    Rule::Rule(const Restriction_t &r) :
        m_cost(r.cost),
        m_precedencelist(r.via, r.via + r.via_size),
        m_all(r.via, r.via + r.via_size) {
            m_dest_id = m_precedencelist.back();
            m_precedencelist.pop_back();
            std::reverse(m_precedencelist.begin(), m_precedencelist.end());
        }

    const std::vector<int64_t>
    Rule::precedencelist() const {
        return m_precedencelist;
    }

    const std::vector<int64_t>
    Rule::precedences() const {
        return m_all;
    }

    std::ostream& operator<<(std::ostream& log, const Rule &r) {
        log << "(";
        for (const auto e : r.m_all) {
            log << e << ",";
        }
        log <<  ")";
        return log;
    }


}  // namespace trsp
}  // namespace pgrouting

