
      
template <typename G> 
void process_ksp(G &graph, const std::vector<std::string> &tokens) {

      std::string::size_type sz;
      if (tokens[1].compare("from") != 0) {
        std::cout << "ksp: missing 'from' kewyword\n";
        return;
      }

      std::vector< int64_t > sources; 
      unsigned int i_ptr = 2;

      for ( ; i_ptr < tokens.size(); ++i_ptr) {
          if (tokens[i_ptr].compare("to") == 0) break;
          try {
            uint64_t start_vertex(stol(tokens[i_ptr], &sz));
            sources.push_back(start_vertex);
          } catch(...) {
            break;
          }
      }

      if (i_ptr == tokens.size() || tokens[i_ptr].compare("to") != 0) {
        std::cout << "ksp: 'to' kewyword not found\n";
        return;
      }

      if (sources.size() == 0) {
        std::cout << "ksp: No start value found\n";
        return;
      }

      ++i_ptr;
      if (i_ptr == tokens.size()) {
        std::cout << "ksp: No 'to' values found\n";
        return;
      }

      std::vector< int64_t > targets; 
      for ( ; i_ptr < tokens.size(); ++i_ptr) {
        auto end_vertex(stol(tokens[i_ptr], &sz));
        targets.push_back(end_vertex);
      }

      Pgr_ksp< G > ksp;
      
      if (sources.size() == 1 && targets.size() == 1) {
        // one to one
        std::deque< Path > paths;
        paths = ksp.Yen(graph, sources[0], targets[0], 3); //TODO make variable
        std::cout << "THE OPUTPUT ----" << "\n";
        for (unsigned int i = 0; i < paths.size(); ++i) {
           if (sizeof(paths[i]) == 0) continue; //no solution found
           std::cout << "Path #" << i << " cost: " << paths[i].cost << "\n";
           paths[i].print_path();
        }
      } else {
	std::cout << "ksp: unknown number of arguments\n";
      }
#if 0 // ksp is only one to 1
	else if (sources.size() == 1 && targets.size() > 1){
        // one to many
        std::deque<Path> paths;
        dijkstra.dijkstra(graph, paths, sources[0], targets);

        std::cout << "THE OPUTPUTS ---->  total outputs: " << paths.size() << "\n";
        for (unsigned int i = 0; i < paths.size(); ++i) {
           if (sizeof(paths[i]) == 0) continue; //no solution found
           std::cout << "Path #" << i << " cost: " << paths[i].cost << "\n";
           paths[i].print_path();
        }
      } else if (sources.size() > 1 && targets.size() == 1){
        // many to 1
        std::deque<Path> paths;
        dijkstra.dijkstra(graph, paths, sources, targets[0]);
        

        std::cout << "THE OPUTPUTS ---->  total outputs: " << paths.size() << "\n";
        for (unsigned int i = 0; i < paths.size(); ++i) {
           if (sizeof(paths[i]) == 0) continue; //no solution found
           std::cout << "Path #" << i << " cost: " << paths[i].cost << "\n";
           paths[i].print_path();
        }
      } else {
        //many to many
        std::deque<Path> paths;
        dijkstra.dijkstra(graph, paths, sources, targets);
        std::cout << "THE OPUTPUTS ---->  total outputs: " << paths.size() << "\n";
        for (unsigned int i = 0; i < paths.size(); ++i) {
          if (sizeof(paths[i]) == 0) continue; //no solution found
          std::cout << "Path #" << i << " cost: " << paths[i].cost << "\n";
          paths[i].print_path();
        }
      }
#endif

}
