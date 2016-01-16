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


      if (tokens.size() == 1) {
        std::cout << "Performing warshall\n";
        std::vector< Matrix_cell_t> matrix_rows;

        pgr_warshall(graph, matrix_rows);

        std::cout << "\t\t\tTHE OPUTPUT\n";
        std::cout << "seq\tfrom\tto\tcost\n";
        size_t seq = 0;
        for (size_t i = 0; i < matrix_rows.size(); i++) {
            std::cout << seq++ << "\t" << matrix_rows[i].from_vid << "\t" <<  matrix_rows[i].to_vid << "\t" << matrix_rows[i].cost << "\n";
        }
#ifdef TEST_POSTGRES
        size_t result_tuple_count;
        Matrix_cell_t *postgres_rows = NULL;
        pgr_warshall(graph, result_tuple_count, &postgres_rows);
        std::cout << "\t\t\tTHE POSTGRESQL OPUTPUT\n";
        std::cout << "seq\tfrom\tto\tcost\n";
        seq = 0;
        for (size_t i = 0; i < result_tuple_count; i++) {
            std::cout << seq++ << "\t" << postgres_rows[i].from_vid << "\t" <<  postgres_rows[i].to_vid << "\t" << postgres_rows[i].cost << "\n";
        }
#endif  // TEST_POSTGRES
        
      } else {
        std::cout << "unknown number of parameters\n";
      }
}

