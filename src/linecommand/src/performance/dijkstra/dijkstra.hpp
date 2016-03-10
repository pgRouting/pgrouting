/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

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

#include <ctime>
#include <chrono>


template <typename G> 
void process_dijkstra(G &graph, const std::vector<std::string> &tokens) {

    std::string::size_type sz;
    if (tokens[1].compare("from") != 0) {
        std::cout << "missing 'from' kewyword\n";
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
        std::cout << "dijkstra: 'dist' kewyword not found\n";
        return;
    }

    if (sources.size() == 0) {
        std::cout << "dijkstra: No start value found\n";
        return;
    }

    ++i_ptr;
    if (i_ptr == tokens.size()) {
        std::cout << "dijkstra: No 'to' values found\n";
        return;
    }

    std::vector< int64_t > targets; 
    for ( ; i_ptr < tokens.size(); ++i_ptr) {
        auto end_vertex(stol(tokens[i_ptr], &sz));
        targets.push_back(end_vertex);
    }


    if (sources.size() == 1 && targets.size() == 1) {
        // one to one
        Path path;

        // TIMING DIJKSTRA
        std::cout << "**************\n\n";
        clock_t begin = clock();
        std::time_t start_t = std::time(NULL);
        std::cout << "Execution starts at: " << std::ctime(&start_t) << "\n";
        std::chrono::steady_clock::time_point begin_elapsed = std::chrono::steady_clock::now();


        pgr_dijkstra(graph, path, sources[0], targets[0]);
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / static_cast<double>(CLOCKS_PER_SEC);

        std::time_t end_t = std::time(NULL);
        std::chrono::steady_clock::time_point end_elapsed = std::chrono::steady_clock::now();

        typedef std::chrono::duration<int,std::milli> millisecs_t ;
        millisecs_t duration = std::chrono::duration_cast<millisecs_t>(end_elapsed - begin_elapsed);

        std::cout << "Execution started at: " << std::ctime(&start_t);
        std::cout << "Execution ended at:   " << std::ctime(&end_t);
        std::cout << "Elapsed time: " << (double)duration.count()/(double)1000 << " Seconds.\n" ;
        std::cout << "User CPU time: -> " << elapsed_secs << " seconds\n";
        std::cout << "**************\n\n";
        // END TIMING DIJKSTRA

        std::cout << "THE OPUTPUT ---->  total cost: " << path.tot_cost() << "\n"
            << path;
    } else if (sources.size() == 1 && targets.size() > 1){
        std::deque<Path> paths;
        // one to many
        pgr_dijkstra(graph, paths, sources[0], targets);

        std::cout << "THE OPUTPUTS ---->  total outputs: " << paths.size() << "\n";
        for (unsigned int i = 0; i < paths.size(); ++i) {
            if (sizeof(paths[i]) == 0) continue; //no solution found
            std::cout << "Path #" << i << " cost: " << paths[i].tot_cost() << "\n"
                << paths[i];
        }
    } else if (sources.size() > 1 && targets.size() == 1){
        // many to 1
        std::deque<Path> paths;
        pgr_dijkstra(graph, paths, sources, targets[0]);


        std::cout << "THE OPUTPUTS ---->  total outputs: " << paths.size() << "\n";
        for (unsigned int i = 0; i < paths.size(); ++i) {
            if (sizeof(paths[i]) == 0) continue; //no solution found
            std::cout << "Path #" << i << " cost: " << paths[i].tot_cost() << "\n"
                << paths[i];
        }
    } else {
        //many to many
        std::deque<Path> paths;
        pgr_dijkstra(graph, paths, sources, targets);

        std::cout << "THE OPUTPUTS ---->  total outputs: " << paths.size() << "\n";
        for (unsigned int i = 0; i < paths.size(); ++i) {
            if (sizeof(paths[i]) == 0) continue; //no solution found
            std::cout << "Path #" << i << " cost: " << paths[i].tot_cost() << "\n"
                << paths[i];
        }
    }
}
