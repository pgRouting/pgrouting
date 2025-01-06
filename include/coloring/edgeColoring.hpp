/*PGR-GNU*****************************************************************
File: edgeColoring.hpp

Generated with Template by:
Copyright (c) 2021 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2021 Veenit Kumar
Mail: 123sveenit@gmail.com
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

#ifndef INCLUDE_COLORING_EDGECOLORING_HPP_
#define INCLUDE_COLORING_EDGECOLORING_HPP_
#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <cstdint>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/version.hpp>

#include "cpp_common/edge_t.hpp"
#include "c_types/ii_t_rt.h"
#include "cpp_common/assert.hpp"
#include "cpp_common/messages.hpp"

namespace pgrouting {
namespace functions {

class Pgr_edgeColoring : public Pgr_messages {
 public:
    using EdgeColoring_Graph =
        boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
        int64_t, boost::no_property>;

    using V       = boost::graph_traits<EdgeColoring_Graph>::vertex_descriptor;
    using E       = boost::graph_traits<EdgeColoring_Graph>::edge_descriptor;
    using V_it    = boost::graph_traits<EdgeColoring_Graph>::vertex_iterator;
    using E_it    = boost::graph_traits<EdgeColoring_Graph>::edge_iterator;

 public:
    std::vector<II_t_rt> edgeColoring();

    explicit Pgr_edgeColoring(const std::vector<Edge_t>&);
    Pgr_edgeColoring() = delete;

#if BOOST_VERSION >= 106800
    friend std::ostream& operator<<(std::ostream &, const Pgr_edgeColoring&);
#endif

 private:
    V get_boost_vertex(int64_t id) const;
    int64_t get_vertex_id(V v) const;
    int64_t get_edge_id(E e) const;


 private:
    EdgeColoring_Graph graph;
    std::map<int64_t, V> id_to_V;
    std::map<V, int64_t> V_to_id;
    std::map<E, int64_t> E_to_id;
};

}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_COLORING_EDGECOLORING_HPP_
