#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <exception>
#include <vector>
#include <boost/graph/random_spanning_tree.hpp>
#include <boost/random/random_number_generator.hpp>

using namespace boost;

int main()
{

  typedef adjacency_list < vecS, vecS, undirectedS,
                           no_property, property < edge_weight_t, int > > 
                           Graph;
  typedef graph_traits < Graph >::edge_descriptor 
                         Edge;
  typedef graph_traits < Graph >::vertex_descriptor 
                         Vertex;
  typedef std::pair<int, int> 
                    add_E;

  const int num_nodes = 2;  // Number of node in Graph

  add_E edge_array[] = { 
                        add_E(0, 1), add_E(1, 2)
                        /*,add_E(3, 4), add_E(5, 4), add_E(6, 7), add_E(7, 8), add_E(0, 3), 
                        add_E(3, 6),  
		        add_E(1, 4), add_E(4, 7), 
                        add_E(2, 5), add_E(5, 8), add_E(9, 8),
                        add_E(6, 10), add_E(10, 11)*/ 

  };

  //int weights[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int weights[] = { 1, 1};

  std::size_t num_edges = sizeof(edge_array) / sizeof(add_E);

  Graph g(edge_array, edge_array + num_edges, weights, num_nodes);

  std::vector < graph_traits < Graph >::vertex_descriptor >
          p(num_vertices(g));

  std::mt19937_64 rng;

  try
    {
        boost::random_spanning_tree(g, 
                               rng,
                               boost::root_vertex(2)
                               .predecessor_map(&p[0])
                             );   
    }
    catch (std::exception const &exc)
    {
        std::cerr << "Exception caught " << exc.what() << "\n";
    }
    catch (...)
    {
        std::cerr << "Unknown exception caught\n";
    }  


  for (std::size_t i = 0; i != p.size(); ++i)
    if (p[i] != i){
        if(i ==2)
          std::cout << "parent[" << i<< "] = " << -1 <<std::endl;     
        else
          std::cout << "parent[" << i<< "] = " << p[i] <<std::endl;
      }
        
  return 0;
}






