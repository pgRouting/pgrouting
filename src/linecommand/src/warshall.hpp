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
#include "./../../warshall/src/pgr_warshall.hpp"

template <typename G> 
void process_warshall(G &graph, const std::vector<std::string> &tokens) {
      std::string::size_type sz;

      Pgr_warshall< G > fn_warshall;

      if (tokens.size() == 1) {
        std::cout << "Performing warshall\n";
        std::vector< std::vector<double>> matrix;
        matrix.resize(graph.numb_vertices());
        for (size_t i=0; i< graph.numb_vertices(); i++)
          matrix[i].resize(graph.numb_vertices());
        fn_warshall.warshall(graph, matrix);
        std::cout << "\t\t\tTHE OPUTPUT\n";
        std::cout << "seq\tfrom\tnode\tedge\tcost\n";
        size_t seq = 0;
        for (size_t i = 0; i< matrix.size(); i++) {
          for (size_t j = 0; j< matrix[i].size(); j++) {
            if (matrix[i][j] != std::numeric_limits<double>::max()) {
                std::cout << seq++ << "\t" << graph.graph[i].id << "\t" <<  graph.graph[j].id << "\t" << matrix[i][j] << "\n";
            }
          }
        }
      } else {
        std::cout << "unknown number of parameters\n";
      }
}

