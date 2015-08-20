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

	//BOOST_CONCEPT_ASSERT(( VertexListGraphConcept<G> ));
	//BOOST_CONCEPT_ASSERT(( EdgeListGraphConcept<G> ));

 public:
    //! \brief the constructor
    explicit Pgr_warshall(graphType gtype, const int initial_size)
     :Pgr_base_graph<G>(gtype, initial_size) {}


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

    void
    warshall(std::deque<Path> &paths) {
        typedef typename boost::graph_traits < G >::vertex_descriptor V;

        std::vector < float > d(boost::num_vertices(this->graph), (std::numeric_limits < float >::max)());
        float DMatrix[boost::num_vertices(this->graph)][boost::num_vertices(this->graph)];
        std::map< V, std::map<V, float> > Dmatrix;
#if 1
        floyd_warshall_all_pairs_shortest_paths(
                     this->graph,
                     Dmatrix
                     );
#endif
      // get the results
      // this->get_path(path, v_source, v_target);
      return;
    }

};

#endif // SRC_WARSHALL_SRC_PGR_WARSHALL_H_
