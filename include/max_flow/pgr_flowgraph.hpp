/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2016 Andrea Nardelli
Mail: nrd.nardelli@gmail.com

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

#ifndef INCLUDE_MAX_FLOW_PGR_FLOWGRAPH_HPP_
#define INCLUDE_MAX_FLOW_PGR_FLOWGRAPH_HPP_
#pragma once

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>


namespace pgrouting {

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    Traits;
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS,
        // Vertex properties
        boost::property<boost::vertex_index_t, int64_t,
        boost::property<boost::vertex_color_t, boost::default_color_type,
        boost::property<boost::vertex_distance_t, int64_t,
        boost::property<boost::vertex_predecessor_t, Traits::edge_descriptor>
        > > >,
        // Edge properties
        boost::property<boost::edge_capacity_t, int64_t,
        boost::property<boost::edge_residual_capacity_t, int64_t,
        boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >
    FlowGraph;

}  // namespace pgrouting

#endif  // INCLUDE_MAX_FLOW_PGR_FLOWGRAPH_HPP_
