/*PGR

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

*/

// http://www.cs.rpi.edu/~musser/archive/2005/gsd/restricted/FloydWarshall/FloydWarshall.pdf

#ifndef SRC_JOHNSON_SRC_PGR_JOHNSON_H_
#define SRC_JOHNSON_SRC_PGR_JOHNSON_H_


#include <deque>
#include <vector>
#include <set>

#include <boost/config.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>

extern "C" {
#include "./../../common/src/pgr_types.h"
}

#include "../../common/src/memory_func.hpp"
#include "./../../common/src/basePath_SSEC.hpp"
#include "./../../common/src/baseGraph.hpp"

template < class G > class Pgr_johnson; 

// user's functions
// for development
template < class G >
void
pgr_johnson(G &graph, std::vector< Matrix_cell_t> &rows) {
     Pgr_johnson< G > fn_johnson;
     fn_johnson.johnson(graph, rows);
}

// for postgres 
template < class G >
void
pgr_johnson(G &graph, size_t &result_tuple_count, Matrix_cell_t **postgres_rows) {
     Pgr_johnson< G > fn_johnson;
     fn_johnson.johnson(graph, result_tuple_count, postgres_rows);
}

// template class
template < class G >
class Pgr_johnson {
  // defualt constructors and destructors
        /*
         Matrix_cell_t description:
                int64_t from_vid;
                int64_t to_vid;
                float8 cost;
	*/
 public:
    void
    johnson(G &graph, size_t &result_tuple_count, Matrix_cell_t **postgres_rows) {

        std::vector< std::vector<double>> matrix;
        make_matrix(graph.num_vertices(), matrix);
        inf_plus<double> combine;
        boost::johnson_all_pairs_shortest_paths(
            graph.graph,
            matrix,
                    weight_map(get(&boost_edge_t::cost, graph.graph)).
                    distance_combine(combine).
                    distance_inf(std::numeric_limits<double>::max()).
                    distance_zero(0));

        make_result(graph, matrix, result_tuple_count, postgres_rows);
    }


    public:
    void
    johnson(G &graph, std::vector< Matrix_cell_t> &rows) {

        std::vector< std::vector<double>> matrix;
        make_matrix(boost::num_vertices(graph.graph), matrix);
        inf_plus<double> combine;
        boost::johnson_all_pairs_shortest_paths(
            graph.graph,
            matrix,
		    weight_map(get(&boost_edge_t::cost, graph.graph)).
		    distance_combine(combine).
		    distance_inf(std::numeric_limits<double>::max()).
		    distance_zero(0));

        make_result(graph, matrix, rows);
    }


 private:
    void
    make_matrix( size_t v_size, std::vector< std::vector<double>> &matrix) const{
        matrix.resize(v_size);
        for (size_t i=0; i < v_size; i++)
          matrix[i].resize(v_size);
    }

    // for postgres
    void
    make_result(
      const G &graph,
      const std::vector< std::vector<double> > &matrix,
      size_t &result_tuple_count,
      Matrix_cell_t **postgres_rows) const{
      result_tuple_count = count_rows(graph, matrix);
      *postgres_rows = get_memory(result_tuple_count, (*postgres_rows));


        size_t seq = 0;
        for (size_t i = 0; i < graph.num_vertices(); i++) {
          for (size_t j = 0; j < graph.num_vertices(); j++) {
            if (matrix[i][j] != std::numeric_limits<double>::max()) {
                (*postgres_rows)[seq].from_vid = graph.graph[i].id;
                (*postgres_rows)[seq].to_vid = graph.graph[j].id;
                (*postgres_rows)[seq].cost =  matrix[i][j];
            seq++;
            }  //if
          }  // for j
        }  //for i
    }

    size_t
    count_rows(const G &graph, const std::vector< std::vector<double> > &matrix) const {
        size_t result_tuple_count = 0;
        for (size_t i = 0; i < graph.num_vertices(); i++) {
          for (size_t j = 0; j < graph.num_vertices(); j++) {
            if (matrix[i][j] != std::numeric_limits<double>::max()) {
               result_tuple_count++;
            }  //if
          }  // for j
        }  //for i
        return result_tuple_count;
    }

    void
    make_result(
      G &graph,
      std::vector< std::vector<double> > &matrix, 
      std::vector< Matrix_cell_t> &rows) {
      size_t count = count_rows(graph, matrix);
      rows.resize(count);
      size_t seq = 0;
      
        for (size_t i = 0; i < graph.num_vertices(); i++) {
          for (size_t j = 0; j < graph.num_vertices(); j++) {
            if (matrix[i][j] != std::numeric_limits<double>::max()) {
                rows[seq] = { graph.graph[i].id, graph.graph[j].id, matrix[i][j] } ;
                seq++;
            }  //if
          }  // for j
        }  //for i
    }
    

    template <typename T>
    struct inf_plus {
        T operator()(const T& a, const T& b) const {
            T inf = std::numeric_limits<T>::max();
            if (a == inf || b == inf)
                return inf;
            return a + b;
        }
    };

};

#endif  //  SRC_JOHNSON_SRC_PGR_JOHNSON_H_
