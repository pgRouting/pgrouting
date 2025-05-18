/*PGR-GNU*****************************************************************
File: allpairs.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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

// http://www.cs.rpi.edu/~musser/archive/2005/gsd/restricted/FloydWarshall/FloydWarshall.pdf

#ifndef INCLUDE_ALLPAIRS_ALLPAIRS_HPP_
#define INCLUDE_ALLPAIRS_ALLPAIRS_HPP_
#pragma once

#include <deque>
#include <vector>
#include <set>
#include <limits>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>

#include "c_types/iid_t_rt.h"
#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/base_graph.hpp"
#include "cpp_common/interruption.hpp"
#include "cpp_common/to_postgres.hpp"


namespace pgrouting  {
namespace detail {

template <typename T>
struct inf_plus {
    T operator()(const T& a, const T& b) const {
        T inf = (std::numeric_limits<T>::max)();
        if (a == inf || b == inf)
            return inf;
        return a + b;
    }
};

}  // namespace detail

template < class G > class Pgr_allpairs;


// for postgres
template < class G >
void
pgr_johnson(
        G &graph,
        size_t &result_tuple_count,
        IID_t_rt **postgres_rows) {
    Pgr_allpairs< G > fn_johnson;
    fn_johnson.johnson(graph, result_tuple_count, postgres_rows);
}


template < class G >
void
pgr_floydWarshall(
        G &graph,
        size_t &result_tuple_count,
        IID_t_rt **postgres_rows) {
    Pgr_allpairs< G > fn_floydWarshall;
    fn_floydWarshall.floydWarshall(graph, result_tuple_count, postgres_rows);
}


// template class
template < class G >
class Pgr_allpairs {
 public:
     void floydWarshall(
             G &graph,
             size_t &result_tuple_count,
             IID_t_rt **postgres_rows) {
         std::vector<std::vector<double>> matrix(graph.num_vertices(), std::vector<double>(graph.num_vertices(), 0));
         detail::inf_plus<double> combine;

         /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
         CHECK_FOR_INTERRUPTS();

         boost::floyd_warshall_all_pairs_shortest_paths(
                 graph.graph,
                 matrix,
                 weight_map(get(&pgrouting::Basic_edge::cost, graph.graph)).
                 distance_combine(combine).
                 distance_inf((std::numeric_limits<double>::max)()).
                 distance_zero(0));

         to_postgres::matrix_to_tuple(graph, matrix, result_tuple_count, postgres_rows);
     }


     void johnson(
             G &graph,
             size_t &result_tuple_count,
             IID_t_rt **postgres_rows) {
         std::vector<std::vector<double>> matrix(graph.num_vertices(), std::vector<double>(graph.num_vertices(), 0));
         detail::inf_plus<double> combine;

         /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
         CHECK_FOR_INTERRUPTS();

         boost::johnson_all_pairs_shortest_paths(
                 graph.graph,
                 matrix,
                 weight_map(get(&pgrouting::Basic_edge::cost, graph.graph)).
                 distance_combine(combine).
                 distance_inf((std::numeric_limits<double>::max)()).
                 distance_zero(0));

         to_postgres::matrix_to_tuple(graph, matrix, result_tuple_count, postgres_rows);
     }



};

}  // namespace pgrouting

#endif  // INCLUDE_ALLPAIRS_ALLPAIRS_HPP_
