//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//
// Revision History:
//   01 April 2001: Modified to use new <boost/limits.hpp> header. (JMaddock)
//
#ifndef BOOST_GRAPH_GRAPH_SEARCH_EDGE_VISITORS_HPP
#define BOOST_GRAPH_GRAPH_SEARCH_EDGE_VISITORS_HPP

#include <iosfwd>
#include <boost/config.hpp>
#include <boost/property_map.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/limits.hpp>
#include <boost/graph/detail/is_same.hpp>

namespace boost {

  //========================================================================
  // Event Tags

  namespace detail {
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
