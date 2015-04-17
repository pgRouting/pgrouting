/************************************************************************/
/* $Id: MainP.cpp 65 2010-09-08 06:48:36Z yan.qi.asu $                                                                 */
/************************************************************************/
#include <boost/config.hpp>
#include <string>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/graph/adjacency_list.hpp>

#include <iostream>
#include <deque>


#include "postgres.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/basePath_SSEC.hpp"
#include "./../../dijkstra/src/pgr_dijkstra.hpp"
#include "./pgr_ksp.hpp"



/****************************************
SIMULATES  THE C CODE THAT LOADS THE DATA
****************************************/
void import_from_file(const std::string &input_file_name, pgr_edge_t *edges, unsigned int *count, int64_t start_vertex, int64_t end_vertex, bool &found) {
    const char* file_name = input_file_name.c_str();

    std::ifstream ifs(file_name);
    if (!ifs) {
            std::cerr << "The file " << file_name << " can not be opened!" << std::endl;
            exit(1);
    }

        ifs >> (*count);

        long edge_id; // , start_id, end_id;
        //double edge_weight, reverse_weight;
        bool s_found = false;
        bool t_found = false;

        unsigned int i = 0;
        while (i < (*count) && ifs >> edge_id) {
                if (edge_id == -1)  break;
                edges[i].id = edge_id;
                ifs >> edges[i].source;
                ifs >> edges[i].target;
                ifs >> edges[i].cost;
                ifs >> edges[i].reverse_cost;
 edges[i].reverse_cost=-1;
                // when loading checking if start and end are found
                if (!s_found)
                   s_found = ((edges[i].source == start_vertex) || (edges[i].target == end_vertex));
                if (!t_found)
                   t_found = ((edges[i].source == end_vertex) || (edges[i].target == end_vertex));
                i++;
        }
  ifs.close();
  found = s_found && t_found;
}

int main(int ac, char* av[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("file,f", po::value<std::string>(), "set file_name")
        ("source,s", po::value<int64_t>(), "set source")
        ("target,t", po::value<int64_t>(), "set target")
        ("directed,d", po::value<bool>()->default_value(true), "True when directed graph")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 0;
    }

    if (vm.count("file") & vm.count("source") & vm.count("target")) {
        std::cout << "File "
             << vm["file"].as<std::string>() << "\t"
             << vm["source"].as<int64_t>() << "\t"
             << vm["target"].as<int64_t>() << ".\n";
    } else {
        std::cout << "Missing parameter.\n";
        std::cout << desc << "\n";
        return 1;
    }

    pgr_edge_t data_edges[100];
    unsigned int count = 0;

    std::string fileName(vm["file"].as<std::string>());
    int start_vertex(vm["source"].as<int64_t>());
    int end_vertex(vm["target"].as<int64_t>());
    int directedFlag(vm["directed"].as<bool>());
    graphType gType = directedFlag? DIRECTED: UNDIRECTED;
    bool found = false;
//    int result_size;
//    float8 total_cost;

    Path path;
    import_from_file(fileName, data_edges, &count, start_vertex, end_vertex, found);

    if ( found == false) {
         path.empty_path(start_vertex);
         path.print_path();       
         return 0;
    }


    typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::undirectedS,
        boost_vertex_t, boost_edge_t > UndirectedGraph;
    typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::bidirectionalS,
        boost_vertex_t, boost_edge_t > DirectedGraph;

    const int initial_size = 1;

#if 0
    // no size overhead because the graph is empty
    // have them available thru out the code
    Pgr_dijkstra< DirectedGraph > digraph(gType, initial_size);
    Pgr_dijkstra< UndirectedGraph > undigraph(gType, initial_size);

    if (directedFlag) {

      std::cout << "DIRECTED GRAPH DEMO\n";
      digraph.initialize_graph(data_edges, count);
      digraph.process_dijkstra(path, start_vertex, end_vertex);
      std::cout << "THE GRAPH \n";
      digraph.print_graph();


   } else {
      std::cout << "UNDIRECTED GRAPH DEMO\n";
      undigraph.initialize_graph(data_edges, count);
      undigraph.process_dijkstra(path, start_vertex, end_vertex);

      std::cout << "THE GRAPH \n";
      undigraph.print_graph();

   }
#endif   

   // the output is independent of the graph
   std::cout << "THE OPUTPUT ---->  total cost: " << path.cost << "\n";
   path.print_path();
   path.clear();
   Pgr_ksp < DirectedGraph > kdigraph(gType, initial_size);
   Pgr_ksp < UndirectedGraph > kundigraph(gType, initial_size);

   std::deque< Path > ksp_paths;

   if (directedFlag) {

      std::cout << "KSP DIRECTED GRAPH DEMO 1 path\n";
      kdigraph.initialize_graph(data_edges, count);
      ksp_paths = kdigraph.Yen(start_vertex, end_vertex, 1);

      std::cout << "THE GRAPH \n";
      kdigraph.print_graph();
   } 

   // the outputs are independent of the graph
   std::cout << "THE OPUTPUTS ---->  total outputs: " << ksp_paths.size() << "\n";
   for (unsigned int i = 0; i < ksp_paths.size(); ++i) {
       if (sizeof(ksp_paths[i]) == 0) continue; //no solution found
       std::cout << "Path #" << i << "\n";
       ksp_paths[i].print_path();
   }


   Pgr_ksp < DirectedGraph > k2digraph(gType, initial_size);
   Pgr_ksp < UndirectedGraph > k2undigraph(gType, initial_size);

   if (directedFlag) {

      std::cout << "KSP DIRECTED GRAPH DEMO 2 path\n";
      k2digraph.initialize_graph(data_edges, count);
      ksp_paths = k2digraph.Yen(start_vertex, end_vertex, 2);

      std::cout << "THE GRAPH \n";
      k2digraph.print_graph();
   } else {
      std::cout << "KSP UNDIRECTED GRAPH DEMO 2 path\n";
      k2undigraph.initialize_graph(data_edges, count);
      ksp_paths = k2undigraph.Yen(start_vertex, end_vertex, 2);

      std::cout << "THE GRAPH \n";
      k2undigraph.print_graph();
   }

   // the outputs are independent of the graph
   std::cout << "THE OPUTPUTS ---->  total outputs: " << ksp_paths.size() << "\n";
   for (unsigned int i = 0; i < ksp_paths.size(); ++i) {
       if (sizeof(ksp_paths[i]) == 0) continue; //no solution found
       std::cout << "Path #" << i << " cost: " << ksp_paths[i].cost << "\n";
       ksp_paths[i].print_path();
   }
}
