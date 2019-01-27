/*PGR-GNU*****************************************************************
file: pgr_delauny.hpp

Copyright (c) 2018 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2018 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

#ifndef INCLUDE_ALPHASHAPE_PGR_DELAUNY_HPP_
#define INCLUDE_ALPHASHAPE_PGR_DELAUNY_HPP_
#pragma once

#include <vector>
#include <iosfwd>

#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/pgr_messages.h"
#include "cpp_common/bline.hpp"

// this one is included before somehow
#include "c_types/pgr_edge_xy_t.h"

namespace pgrouting {
namespace alphashape {

using BG = boost::adjacency_list<
        boost::setS, boost::vecS,
        boost::undirectedS,
        XY_vertex, Basic_edge >;
using G = graph::Pgr_base_graph <BG, XY_vertex, Basic_edge>;


class Pgr_delauny : public Pgr_messages {

 public:
     Pgr_delauny() = default;
     Pgr_delauny(const std::vector<Pgr_edge_xy_t> &edges);


     friend std::ostream& operator<<(std::ostream&, const Pgr_delauny&);

 private:

     std::vector<Bline> alpha_edges(double alpha) const;
     G graph;
};


}  // namespace alphashape
}  // namespace pgrouting

#endif  // INCLUDE_ALPHASHAPE_PGR_DELAUNY_HPP_
