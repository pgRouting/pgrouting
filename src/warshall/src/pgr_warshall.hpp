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

#ifndef SRC_WARSHALL_SRC_PGR_WARSHALL_H_
#define SRC_WARSHALL_SRC_PGR_WARSHALL_H_


#include <deque>
#include <vector>
#include <set>

#include <boost/config.hpp>

//#include <boost/concept_check.hpp>
//#include <boost/concept/assert.hpp> 
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>

#include "./../../common/src/basePath_SSEC.hpp"
#include "./../../common/src/baseGraph.hpp"
#include "postgres.h"


template < class G >
class Pgr_warshall
  :public Pgr_base_graph<G> {


 public:
    //! \brief the constructor
    explicit Pgr_warshall(graphType gtype, const int initial_size)
     :Pgr_base_graph<G>(gtype, initial_size) {
}


    /*! \brief Perfom the inizialization of the graph
     For Dijkstra  only requieres the data insertion.

     Any other different initialization needs a different function.
     \param[in] data_edges
     \param[in] count
    */
    void
    initialize_graph(pgr_edge_t *data_edges, int64_t count) {
          this->graph_insert_data(data_edges, count);
    }


        /*
         path_element_t description:
                int64_t vertex_id; -- used here to store row vertex_id (from_id)
                int64_t edge_id;   -- used here to store clolumn vertex_id (to_id)
                float8 cost;       -- the distance between from_id to to_id
	*/
    void
    warshall(matrix_cell_t **result, int64_t &size) {
	int64_t node_count = boost::num_vertices(this->graph);
	std::vector< std::vector<double>> Dmatrix(node_count);
        for(int i = 0; i < node_count; i++) {
            Dmatrix[i].resize(node_count);
	}

//	warshall(Dmatrix);
//        size = node_count * node_count;

	// The matrix might be very very big
	// so I will be deleting row by row from the vector
	#if 0
	int64_t iRows = 0;
	int64_t i = 0;
	while (Dmatrix.begin() != Dmatrix.end()) {
	    // allocating enough space for the next row
            if (result == 0) *result = (path_element_t *) malloc(node_count * sizeof(path_element_t));
            else  *result = (path_element_t *) realloc(result, (iRows + 1) * node_count * sizeof(path_element_t));
	    int64_t row = Dmatrix.size()-1;   // working form end to beginnig
	    int64_t column = 0;
            for(const auto value : Dmatrix.back()) {
                result[i]->vertex_id = row;
                result[i]->edge_id = column;
                result[i]->cost = value;
                ++column;
		++i;
            }
	    Dmatrix.pop_back();  // reducing the size of the original matrix to make space
            ++iRows;
        }
	#endif
      return;
    }


    public:
    void
    warshall(std::vector< std::vector<double>> &Dmatrix) {

 std::cout << "ya entre";

        inf_plus<double> combine;
	    //bool valid = 
         boost::floyd_warshall_all_pairs_shortest_paths(
            this->graph,
            Dmatrix,
		    weight_map(get(&boost_edge_t::cost, this->graph)).
		    // distance_compare(compare).  // leaving the default
		    distance_combine(combine).
		    distance_inf(std::numeric_limits<double>::max()).
		    distance_zero(0));

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

#endif // SRC_WARSHALL_SRC_PGR_WARSHALL_H_
