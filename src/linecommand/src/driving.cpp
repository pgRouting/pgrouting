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

template <typename G> 
void process_drivingDistance(G &graph, const std::vector<std::string> &tokens) {

      std::string::size_type sz;
      if (tokens[1].compare("from") != 0) {
        std::cout << "missing 'from' kewyword\n";
        return;
      }

      std::vector< int64_t > sources; 
      unsigned int i_ptr = 2;

      for ( ; i_ptr < tokens.size(); ++i_ptr) {
          if (tokens[i_ptr].compare("dist") == 0) break;
          try {
            uint64_t start_vertex(stol(tokens[i_ptr], &sz));
            sources.push_back(start_vertex);
          } catch(...) {
            break;
          }
      }

      if (i_ptr == tokens.size() || tokens[i_ptr].compare("dist") != 0) {
        std::cout << "drivDist: 'dist' kewyword not found\n";
        return;
      }

      if (sources.size() == 0) {
        std::cout << "drivDist: No start value found\n";
        return;
      }

      ++i_ptr;
      if (i_ptr == tokens.size()) {
        std::cout << " 'distance' value not found\n";
        return;
      }

      double distance = stod(tokens[i_ptr], &sz);

      ++i_ptr;
      bool equiCost(false);
      if (i_ptr != tokens.size()) {
        if (tokens[i_ptr].compare("equi") != 0) {
          std::cout << " Unknown keyword '" << tokens[i_ptr] << "' found\n";
          return;
        } else {
          equiCost = true;
        }
      }

      std::cout << "found " << sources.size() << "starting locations\n";
      Pgr_dijkstra< G > fn_dijkstra;

      if (sources.size() == 1) {
        std::cout << "Performing pgr_DrivingDistance for single source\n";
        Path path;
        fn_dijkstra.dijkstra_dd(graph, path, sources[0], distance);
        std::cout << "\t\t\tTHE OPUTPUT\n";
        std::cout << "seq\tfrom\tnode\tedge\tcost\n";
        path.print_path();
      } else {
        std::deque< Path >  paths;
        fn_dijkstra.dijkstra_dd(graph, paths, sources, distance);
        if (equiCost == false) {
          std::cout << "Performing pgr_DrivingDistance for multiple sources\n";
          std::cout << "\t\t\tTHE OPUTPUT\n";
          std::cout << "seq\tfrom\tnode\tedge\tcost\n";
          for (const auto &path :  paths) {
            if (sizeof(path) == 0) return; //no solution found
            path.print_path();
          }
        } else {
          std::cout << "Performing pgr_DrivingDistance for multiple sources with equi-cost\n";
          Path path = equi_cost(paths); 
          std::cout << "\t\t\tTHE EquiCost OPUTPUT\n";
          std::cout << "seq\tfrom\tnode\tedge\tcost\n";
          path.print_path();
        }
      }
}

