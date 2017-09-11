/*PGR-GNU*****************************************************************
File: restriction.h

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

#ifndef INCLUDE_DIJKSTRATRSP_RESTRICTION_H_
#define INCLUDE_DIJKSTRATRSP_RESTRICTION_H_
#pragma once

#include <sstream>
#include <deque>
#include <vector>
#include <set>
#include <limits>

#include "c_types/restrict_t.h"

class Restriction {
 private:
        int64_t m_id;
        std::vector< int64_t > m_restrict_edges;
        double m_cost;

 public:
        Restriction() = default;
        explicit Restriction(const Restrict_t &r);

        int64_t id() const {return m_id;}
        void id(const int64_t& value) {m_id = value;}

        double cost() const {return m_cost;}
        void cost(const double& value) {m_cost = value;}

        std::vector< int64_t > restrict_edges() const {return m_restrict_edges;}

        void restrict_edges(const int64_t& value) {
            m_restrict_edges.push_back(value);
        }

        void clear() {
            m_restrict_edges.clear();
        }

        int64_t restriction_size() const {return m_restrict_edges.size();}

        friend std::ostream& operator << (std::ostream &log,
             const Restriction &r);
};

#endif  // INCLUDE_DIJKSTRATRSP_RESTRICTION_H_
