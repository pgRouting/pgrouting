/*PGR-GNU*****************************************************************
File: one_to_one_dijkstra_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
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

#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#endif

#include <sstream>
#include <deque>
#include <vector>
#include "./pgr_dijkstra.hpp"
#include "./one_to_one_dijkstra_driver.h"


#include "../../common/src/pgr_alloc.hpp"
extern "C" {
#include "./../../common/src/pgr_types.h"
}

// CREATE OR REPLACE FUNCTION pgr_dijkstra(
// sql text,
// start_vids BIGINT,
// end_vid BIGINT,
//  directed BOOLEAN default true,

void
do_pgr_one_to_one_dijkstra(
        pgr_edge_t  *data_edges,
        size_t total_tuples,
        int64_t start_vid,
        int64_t end_vid,
        bool directed,
        bool only_cost,
        General_path_element_t **return_tuples,
        size_t *return_count,
        char ** err_msg) {
  std::ostringstream log;
  try {
      graphType gType = directed? DIRECTED: UNDIRECTED;

      Path path;

      if (directed) {
          log << "Working with directed Graph\n";
          pgrouting::DirectedGraph digraph(gType);
          digraph.graph_insert_data(data_edges, total_tuples);
          pgr_dijkstra(digraph, path, start_vid, end_vid, only_cost);
      } else {
          log << "Working with Undirected Graph\n";
          pgrouting::UndirectedGraph undigraph(gType);
          undigraph.graph_insert_data(data_edges, total_tuples);
          pgr_dijkstra(undigraph, path, start_vid, end_vid, only_cost);
      }

      size_t count(0);

      count = path.size();

      if (count == 0) {
          (*return_tuples) = NULL;
          (*return_count) = 0;
          log <<
              "No paths found between Starting and any of the Ending vertices\n";
          *err_msg = strdup(log.str().c_str());
          return;
      }

      (*return_tuples) = pgr_alloc(count, (*return_tuples));
      size_t sequence = 0;
      path.generate_postgres_data(return_tuples, sequence);
      (*return_count) = sequence;

#ifndef DEBUG
      *err_msg = strdup("OK");
#else
      *err_msg = strdup(log.str().c_str());
#endif

      return;
  } catch ( ... ) {
      log << "Caught unknown expection!\n";
      *err_msg = strdup(log.str().c_str());
      return;
  }
}





