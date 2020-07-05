/*PGR-GNU*****************************************************************
File:  pgr_allpairs.hpp

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

#ifndef INCLUDE_ALLPAIRS_PGR_ALLPAIRS_HPP_
#define INCLUDE_ALLPAIRS_PGR_ALLPAIRS_HPP_
#pragma once


#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>


#include <deque>
#include <vector>
#include <set>
#include <limits>


#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/interruption.h"

// TODO(vicky) don't keep it here
#include "cpp_common/pgr_alloc.hpp"

template < class G > class Pgr_allpairs;

// user's functions
template < class G >
void
pgr_johnson(G &graph, std::vector< Matrix_cell_t> &rows) {
    Pgr_allpairs< G > fn_johnson;
    fn_johnson.johnson(graph, rows);
}

template < class G >
void
pgr_floydWarshall(G &graph, std::vector< Matrix_cell_t> &rows) {
    Pgr_allpairs< G > fn_floydWarshall;
    fn_floydWarshall.floydWarshall(graph, rows);
}

// for postgres
template < class G >
void
pgr_johnson(
        G &graph,
        size_t &result_tuple_count,
        Matrix_cell_t **postgres_rows) {
    Pgr_allpairs< G > fn_johnson;
    fn_johnson.johnson(graph, result_tuple_count, postgres_rows);
}


template < class G >
void
pgr_floydWarshall(
        G &graph,
        size_t &result_tuple_count,
        Matrix_cell_t **postgres_rows) {
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
             Matrix_cell_t **postgres_rows) {
         std::vector< std::vector<double>> matrix;
         make_matrix(graph.num_vertices(), matrix);
         inf_plus<double> combine;

         /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
         CHECK_FOR_INTERRUPTS();

         boost::floyd_warshall_all_pairs_shortest_paths(
                 graph.graph,
                 matrix,
                 weight_map(get(&pgrouting::Basic_edge::cost, graph.graph)).
                 distance_combine(combine).
                 distance_inf((std::numeric_limits<double>::max)()).
                 distance_zero(0));

         make_result(graph, matrix, result_tuple_count, postgres_rows);
     }

     void floydWarshall(
             G &graph,
             std::vector< Matrix_cell_t> &rows) {
         std::vector< std::vector<double>> matrix;
         make_matrix(graph.num_vertices(), matrix);
         inf_plus<double> combine;

         /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
         CHECK_FOR_INTERRUPTS();

         boost::floyd_warshall_all_pairs_shortest_paths(
                 graph.graph,
                 matrix,
                 weight_map(get(&pgrouting::Basic_edge::cost, graph.graph)).
                 distance_combine(combine).
                 distance_inf((std::numeric_limits<double>::max)()).
                 distance_zero(0));

         make_result(graph, matrix, rows);
     }

     void johnson(
             G &graph,
             size_t &result_tuple_count,
             Matrix_cell_t **postgres_rows) {
         std::vector< std::vector<double>> matrix;
         make_matrix(graph.num_vertices(), matrix);
         inf_plus<double> combine;

         /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
         CHECK_FOR_INTERRUPTS();

         boost::johnson_all_pairs_shortest_paths(
                 graph.graph,
                 matrix,
                 weight_map(get(&pgrouting::Basic_edge::cost, graph.graph)).
                 distance_combine(combine).
                 distance_inf((std::numeric_limits<double>::max)()).
                 distance_zero(0));

         make_result(graph, matrix, result_tuple_count, postgres_rows);
     }


     void johnson(
             G &graph,
             std::vector< Matrix_cell_t> &rows) {
         std::vector< std::vector<double>> matrix;
         make_matrix(graph.num_vertices(), matrix);
         inf_plus<double> combine;

         /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
         CHECK_FOR_INTERRUPTS();

         boost::johnson_all_pairs_shortest_paths(
                 graph.graph,
                 matrix,
                 weight_map(get(&pgrouting::Basic_edge::cost, graph.graph)).
                 distance_combine(combine).
                 distance_inf((std::numeric_limits<double>::max)()).
                 distance_zero(0));

         make_result(graph, matrix, rows);
     }

 private:
     void make_matrix(
             size_t v_size,
             std::vector< std::vector<double>> &matrix) const {
         // TODO(vicky) in one step
         matrix.resize(v_size);
         for (size_t i=0; i < v_size; i++)
             matrix[i].resize(v_size);
     }

     void make_result(
             const G &graph,
             const std::vector< std::vector<double> > &matrix,
             size_t &result_tuple_count,
             Matrix_cell_t **postgres_rows) const {
         result_tuple_count = count_rows(graph, matrix);
         *postgres_rows = pgr_alloc(result_tuple_count, (*postgres_rows));


         size_t seq = 0;
         for (typename G::V v_i = 0; v_i < graph.num_vertices(); v_i++) {
             for (typename G::V v_j = 0; v_j < graph.num_vertices(); v_j++) {
                 if (v_i == v_j) continue;
                 if (matrix[v_i][v_j] != (std::numeric_limits<double>::max)()) {
                     (*postgres_rows)[seq].from_vid = graph[v_i].id;
                     (*postgres_rows)[seq].to_vid = graph[v_j].id;
                     (*postgres_rows)[seq].cost =  matrix[v_i][v_j];
                     seq++;
                 }  // if
             }  // for j
         }  // for i
     }


     size_t count_rows(
             const G &graph,
             const std::vector< std::vector<double> > &matrix) const {
         size_t result_tuple_count = 0;
         for (size_t i = 0; i < graph.num_vertices(); i++) {
             for (size_t j = 0; j < graph.num_vertices(); j++) {
                 if (i == j) continue;
                 if (matrix[i][j] != (std::numeric_limits<double>::max)()) {
                     result_tuple_count++;
                 }  // if
             }  // for j
         }  // for i
         return result_tuple_count;
     }

     void make_result(
             G &graph,
             std::vector< std::vector<double> > &matrix,
             std::vector< Matrix_cell_t> &rows) {
         size_t count = count_rows(graph, matrix);
         rows.resize(count);
         size_t seq = 0;

         for (typename G::V v_i = 0; v_i < graph.num_vertices(); v_i++) {
             for (typename G::V v_j = 0; v_j < graph.num_vertices(); v_j++) {
                 if (matrix[v_i][v_j] != (std::numeric_limits<double>::max)()) {
                     rows[seq] =
                     {graph[v_i].id, graph[v_j].id, matrix[v_i][v_j]};
                     seq++;
                 }  // if
             }  // for j
         }  // for i
     }

     template <typename T>
     struct inf_plus {
         T operator()(const T& a, const T& b) const {
             T inf = (std::numeric_limits<T>::max)();
             if (a == inf || b == inf)
                 return inf;
             return a + b;
         }
     };
};


#endif  // INCLUDE_ALLPAIRS_PGR_ALLPAIRS_HPP_
