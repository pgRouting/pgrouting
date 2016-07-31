/*PGR-GNU*****************************************************************
File: contraction_type.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2016 Rohith Reddy
Mail: 

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
#pragma once
#include <iostream>
#include <sstream>
#include <string>

class Contraction_type {
 public:
    typedef enum {
        kNone = 0,
        kDeadEnd
    } types;
    Contraction_type() : m_type(types::kNone) { }
    inline bool is_none() const { return m_type == types::kNone; }
    inline bool is_dead_end() const { return m_type == types::kDeadEnd; }
    inline types type() const { return m_type; }
    inline void set_type(Contraction_type ctype) { m_type = ctype.type(); }

    const std::string type_str() const {
        switch (Contraction_type::type()) {
            case Contraction_type::types::kNone:
            return "None";
            case Contraction_type::types::kDeadEnd:
            return "DeadEnd Contraction";
            default:
            return "Invalid Contraction Type";
        }
    }

 private:
    types m_type;
};
