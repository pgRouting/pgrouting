
      
template <typename G> 
void process_warshall(G &graph) {
      std::vector< std::vector<double>> Dmatrix;
      graph.warshall(Dmatrix);
      std::cout << "THE OPUTPUT \n";
}
