/*PGR-GNU*****************************************************************

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
#ifndef EDGE_VISITORS_HPP
#define EDGE_VISITORS_HPP

#include <iosfwd>
#include <boost/config.hpp>
#include <boost/version.hpp>

#if BOOST_VERSION > 103900
#include <boost/property_map/property_map.hpp>
#else
#include <boost/property_map.hpp>
#endif

#include <boost/graph/graph_traits.hpp>
#include <boost/limits.hpp>

#if BOOST_VERSION <= 14700
#include <boost/graph/detail/is_same.hpp>
#else
#include <boost/type_traits/is_same.hpp>
#endif

namespace boost 
{
  // Event Tags

  namespace detail 
  {
    // For partial specialization workaround
    enum event_edge_visitor_enum
    { 
      on_initialize_edge_num, on_start_edge_num,
      on_discover_edge_num, on_finish_edge_num
    };
  }
							    
  struct on_initialize_edge { enum { num = detail::on_initialize_edge_num }; };
  struct on_start_edge { enum { num = detail::on_start_edge_num }; };
  struct on_discover_edge { enum { num = detail::on_discover_edge_num }; };
  struct on_finish_edge { enum { num = detail::on_finish_edge_num }; };

}

#endif
