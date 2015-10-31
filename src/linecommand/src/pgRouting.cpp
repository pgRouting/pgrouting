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

/************************************************************************/
/* $Id: MainP.cpp 65 2010-09-08 06:48:36Z yan.qi.asu $                                                                 */
/************************************************************************/
#include <boost/config.hpp>
#include <string>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/graph/adjacency_list.hpp>

#include <iostream>
#include <sstream>
#include <fstream>
#include <deque>
#include <unistd.h>
#include <libpq-fe.h>


#include "postgres.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/basePath_SSEC.hpp"
#include "./../../dijkstra/src/pgr_dijkstra.hpp"
#include "./../../warshall/src/pgr_warshall.hpp"
#include "./../../ksp/src/pgr_ksp.hpp"
#include "./driving.cpp"
#include "./dijkstra.hpp"
#include "./ksp.cpp"
#include "./warshall.hpp"



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

void get_options_description(po::options_description &od_desc) {
    od_desc.add_options()
        ("help", "Produce this help message.")
        ("test,t",  po::value<bool>()->default_value(true),
            "For testing purposes.")
        ("dbname,d", po::value<std::string>()->required(), 
            "Specifies the name of the database to connect to.")
        ("host,h",  po::value<std::string>()->default_value("localhost"),
            "Specifies the host name of the machine on which the server is running.")
        ("port,p",  po::value<std::string>()->default_value("5432"),
            "Port number to connect to at the server host.")
        ("username,U",  po::value<std::string>()->default_value(getlogin()),
            "Connect to the database as the user username instead of the default.\n (You must have permission to do so, of course.)")
        ("password,W",  po::value<std::string>()->default_value(""),
            "User's the password.\n")
#if 0
        ("no-password,w",  po::value<bool>()->implicit_value(false),
            "Never issue a password prompt.\n" 
            "If the server requires password authentication and a password is not available by other means such as a .pgpass file, the connection attempt will fail.\n"
            "This option can be useful in batch jobs and scripts where no user is present to enter a password.")
#endif
    ;
}


int process_command_line(
  po::variables_map &vm, 
  po::options_description &od_desc) {

    if (vm.count("help")) {
        std::cout << od_desc << "\n";
        return 0;
    }

    if (vm.count("dbname")) 
      std::cout << "dbname = " << vm["dbname"].as<std::string>() << "\n";
    else
      std::cout << "Parameter: dbname missing\n";

    if (vm.count("host")) 
      std::cout << "host = " << vm["host"].as<std::string>() << "\n";
    else
      std::cout << "Parameter: host missing\n";

    if (vm.count("port")) 
      std::cout << "port = " << vm["port"].as<std::string>() << "\n";
    else
      std::cout << "Parameter: port missing\n";

    if (vm.count("username")) 
      std::cout << "username = " << vm["username"].as<std::string>() << "\n";
    else
      std::cout << "Parameter: username missing\n";

    if (vm.count("dbname") & vm.count("username") & vm.count("host")) {
        std::cout << "Parameters: \n"
             << vm["dbname"].as<std::string>() << "\n"
             << vm["username"].as<std::string>() << "\n"
             << vm["host"].as<std::string>() << ".\n";
        return 2;
    } else {
        std::cout << "Missing parameter.\n";
        std::cout << od_desc << "\n";
        return 1;
    }
}


static void
exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    exit(1);
}



template <class G> 
void process(G &graph, pgr_edge_t *data_edges, int row_count) {
  // grpah initialization
  graph.graph_insert_data(data_edges, row_count);
  graph.print_graph();

  // some local variables
  std::vector<int64_t> targets;
  std::string::size_type sz;

  
  // read the command and break into tokens
  std::string cmd;
  std::string buf;
  std::vector<std::string> tokens;
  while (true) {
    std::cout << "\n\n\n\n\t\t COMMANDS\n\n "
     << "\tWARSHALL\n"
     << "\twarshall\n"

     << "\n\tKSP\n"
     << "(Input the command separating with spaces)\n"
     << "\tksp from  to \n"

     << "\n\tDIJKSTRA\n"
     << "(Input the command separating with spaces)\n"
     << "\tdijkstra from  to \n"
     << "\tdijkstra from  to1 to2 to3\n\n"

     << "\n\tDRIVING DISTANCE\n"
     << "(Use kewywords)\n"
     << "\tdrivDist from <id> [<id> ...] dist <distance> [equi]\n"

     << "\n\tFINISH\n"
     << "\tend\n\n"
     << ">>>";
    tokens.clear();
    targets.clear();
    cmd = "";
    std::getline(std::cin, cmd);
    std::stringstream ss(cmd);
    while (ss >> buf) {
        tokens.push_back(buf);
    }

    if (tokens.size() == 0) {
      std::cout << "No command received\n";
      continue;
    }

    if (tokens[0].compare("end")==0) return;

    if (tokens[0].compare("dijkstra") != 0 
       && tokens[0].compare("warshall") != 0 
       && tokens[0].compare("ksp") != 0 
       && tokens[0].compare("drivDist") != 0 ) {
      std::cout << "Command: " << cmd << " not found\n";
      continue;
    }
    
    
    if (tokens[0].compare("dijkstra") == 0) {
       process_dijkstra(graph, tokens);
    } else if (tokens[0].compare("ksp") == 0) {
       process_ksp(graph, tokens);
    } else if (tokens[0].compare("warshall") == 0) {
       process_warshall(graph, tokens);
    } else {
      process_drivingDistance(graph, tokens);
    }
  }
}



int main(int ac, char* av[]) {
    po::options_description od_desc("Allowed options");
    get_options_description(od_desc);
    
    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, od_desc), vm);
    if (vm.count("help")) {
       std::cout << od_desc << "\n"; 
       return 0;
    }

    try{
      po::notify(vm);
    }
    catch(...){
	std::cout << od_desc << "\n"; 
	return 0;	
    } 

    auto ret_val = process_command_line(vm, od_desc);
    if (ret_val != 2) return ret_val;


    auto db_dbase(vm["dbname"].as<std::string>());
    db_dbase = "dbname = " + db_dbase;
    auto db_host(vm["host"].as<std::string>());
    auto db_port(vm["port"].as<std::string>());
    auto db_username(vm["username"].as<std::string>());
    auto db_pwd(vm["password"].as<std::string>());
    auto test(vm["test"].as<bool>());
    //auto db_no_pwd(vm["no-password"].as<std::string>());

    const char *conninfo = db_dbase.c_str();
    PGconn     *conn;
    PGresult   *res;
    int rec_count, col_count;


    conn = PQconnectdb(conninfo);
 /* Check to see that the backend connection was successfully made */
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s",
                PQerrorMessage(conn));
        exit_nicely(conn);
        exit(0);
    }

   std::string data_sql;
   if (test) {
     data_sql = "select id, source, target, cost, reverse_cost from edge_table order by id";
   } else {
     std::cout << "Input data query: ";
     std::getline (std::cin,data_sql);
   }
   std::cout << "\nThe data is from:" << data_sql <<"\n";

   res = PQexec(conn, data_sql.c_str());

      if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cout << "We did not get any data!\n";
        exit_nicely(conn);
        exit(1);
      }

      rec_count = PQntuples(res);
      col_count = PQnfields(res);
      if (col_count > 5 || col_count < 4) {
        std::cout << "Max number of columns 5\n";
        std::cout << "Min number of columns 4\n";
        exit_nicely(conn);
        exit(1);
      }

      auto id_fnum = PQfnumber(res, "id");
      auto source_fnum = PQfnumber(res, "source");
      auto target_fnum = PQfnumber(res, "target");
      auto cost_fnum = PQfnumber(res, "cost");
      auto reverse_fnum = PQfnumber(res, "reverse_cost");



      pgr_edge_t *data_edges;
      data_edges = (pgr_edge_t *) malloc(rec_count * sizeof(pgr_edge_t));
	

      printf("We received %d records.\n", rec_count);
      puts("==========================");

      

      std::string::size_type sz;
      std::string str;

      for (int row = 0; row < rec_count; ++row) {
        str = std::string(PQgetvalue(res, row, id_fnum));
        data_edges[row].id = stol(str, &sz);

        str = std::string(PQgetvalue(res, row, source_fnum));
        data_edges[row].source = stol(str, &sz);

        str = std::string(PQgetvalue(res, row, target_fnum));
        data_edges[row].target = stol(str, &sz);

        str = std::string(PQgetvalue(res, row, cost_fnum));
        data_edges[row].cost = stod(str, &sz);

        if (reverse_fnum != -1) {
          str = std::string(PQgetvalue(res, row, reverse_fnum));
          data_edges[row].reverse_cost = stod(str, &sz);
        } 
#if 0
        std::cout << "\tid: " << data_edges[row].id << "\t";
        std::cout << "\tsource: " << data_edges[row].source << "\t";
        std::cout << "\ttarget: " << data_edges[row].target << "\t";
        std::cout << "\tcost: " << data_edges[row].cost << "\t";
        if (reverse_fnum != -1) {
          std::cout << "\treverse: " << data_edges[row].reverse_cost << "\t";
        }
        std::cout << "\n";
#endif
      }


      puts("==========================");

      PQclear(res);

      PQfinish(conn);


//////////////////////  END READING DATA FROM DATABASE ///////////////////

    std::string directed;
    std::cout << "Is the graph directed [N,n]? default[Y]";
    std::getline(std::cin,directed);
    graphType gType =  (directed.compare("N")==0 || directed.compare("n")==0)? UNDIRECTED: DIRECTED;
    bool directedFlag =  (directed.compare("N")==0 || directed.compare("n")==0)? false: true;



    typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::undirectedS,
        boost_vertex_t, boost_edge_t > UndirectedGraph;
    typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::bidirectionalS,
        boost_vertex_t, boost_edge_t > DirectedGraph;

    const int initial_size = rec_count;

    Pgr_base_graph< DirectedGraph > digraph(gType, initial_size);
    Pgr_base_graph< UndirectedGraph > undigraph(gType, initial_size);


    if (directedFlag) {
      process(digraph, data_edges, rec_count);
    } else {
      process(undigraph, data_edges, rec_count);
    }

}


