#include <deque>
#include <sstream>

#include "KSPGraph.h"
#include "YenTopKShortestPathsAlg.h"
extern "C" {
#include "ksp.h"
}

#include "KSPDriver.h"
static  void dpPrint(const KSPGraph &theGraph,
                     const BasePath &thePath,
                     ksp_path_element_t *path,
                     int &sequence, int route_id);
static  ksp_path_element_t * noPathFound(int start_id);

int  doKpaths(ksp_edge_t  * edges, int total_tuples,
                       int  start_vertex, int  end_vertex,
                       int no_paths, bool has_reverse_cost,
                       ksp_path_element_t **path, int *path_count,
                       char ** err_msg) {
        KSPGraph theGraph = KSPGraph();
        std::ostringstream log;

        log << "Step 0: Loading the graph\n";
        theGraph.StartLoad();
        theGraph.AddData(edges, total_tuples, has_reverse_cost);
        theGraph.EndLoad();
        (*path_count) = 0;

        log << "Step 1: checking Sarting and Ending Vertex\n";

        if ( !theGraph.exist_vertex(start_vertex)) {
            *err_msg = strdup( "Starting vertex not found on any edge" );
            (*path_count) = 1;
            *path = noPathFound(start_vertex);
            return 0;
        }

        if (!theGraph.exist_vertex(end_vertex)) {
            *err_msg = strdup( "Ending vertex not found on any edge" );
            (*path_count) = 1;
            *path = noPathFound(start_vertex);
            return 0;
        }

        if (start_vertex == end_vertex) {
            *err_msg = strdup( "Starting and Ending vertices are the same" );
            (*path_count) = 1;
            *path = noPathFound(start_vertex);
            return 0;
        }
        log << "Step 2: Starting Yen graph \n";

        YenTopKShortestPathsAlg yenGraph(theGraph);

        log << "Step 3: Getting the paths \n";
        std::deque<BasePath> paths;
        paths = yenGraph.Yen(start_vertex, end_vertex, no_paths);
*err_msg = strdup( log.str().c_str());
(*path_count) = 1;
*path = noPathFound(start_vertex);
return 0;

        if (paths.size() == 0) {
            *err_msg = strdup( "No path found between Starting and Ending vertices" );
            (*path_count) = 1;
            *path = noPathFound(start_vertex);
            return 0;
        }




        log << "Step 3: Calculating the number of tuples \n";
        int count = 0;
        int seq = 0;
        for (unsigned int i = 0; i < paths.size(); ++i ) {
           if (paths[i].size() > 0)  // don't count empty routes
              count += paths[i].size() + 1;   // add final vertex
           for (unsigned int j = 0; j < paths[i].size(); ++j ) {
             log << seq << "\t" <<   paths[i][j]->getStart() << "\t" << yenGraph.getVertex(paths[i][j]->getStart()) 
                 << "\t" << paths[i][j]->getID() 
                 << "\t" <<  paths[i][j]->Weight() << "\n";
 
             seq ++;
           }
           log << seq << "\t" <<  paths[i][ paths[i].size()-1 ]->getEnd() << "\t0\t -1\n";
           seq++;
        }
        log << "Count: " << count << "\n";
*err_msg = strdup( log.str().c_str());
(*path_count) = 1;
*path = noPathFound(start_vertex);
return 0;

        // get the space required to store all the paths
        ksp_path_element_t *ksp_path;
        ksp_path = 0;
        ksp_path = get_ksp_memory(count, ksp_path);

        int sequence = 0;
        for (unsigned int route_id = 0; route_id < paths.size(); route_id++) {
          if (paths[route_id].size() > 0)
               dpPrint(theGraph, paths[route_id], ksp_path, sequence, route_id);
        }

        log << "Sequence: " << sequence << "\n";
        if (count != sequence) {
            log << "Internal count and real count are different. \nThis should not happen: Please report in GitHub: pgrouting issues.";
            *err_msg = strdup( log.str().c_str());
            return -1;
        }
        #if 0
        *err_msg = strdup("OK");
        #else
        *err_msg = strdup( log.str().c_str());
        #endif
        *path = ksp_path;
        *path_count = count;
        return 1;
}

static  void dpPrint(const KSPGraph &theGraph,
                     const BasePath &thePath,
                     ksp_path_element_t *path,
                     int &sequence, int route_id) {
        // the row data:  seq, route, nodeid, edgeId, cost
        int nodeId, edgeId, lastNodeId;
        double cost;

        for (unsigned int i = 0; i < thePath.size(); i++) {
                edgeId = thePath.getID(i);
                nodeId = theGraph.getVertex(thePath[i]->getStart());
                cost = thePath[i]->Weight();
                if (i == thePath.size()-1)
                      lastNodeId = theGraph.getVertex(thePath[i]->getEnd());

               path[sequence].route_id = route_id;
               path[sequence].vertex_id = nodeId;
               path[sequence].cost = cost;
               path[sequence].edge_id = edgeId;
               sequence++;
                if (i == thePath.size()-1) {
                      path[sequence].route_id = route_id;
                      path[sequence].vertex_id = lastNodeId;
                      path[sequence].cost = 0;
                      path[sequence].edge_id = -1;
                      sequence++;
               }
        }
}

static  ksp_path_element_t * noPathFound(int start_id) {
        ksp_path_element_t *no_path;
        no_path = get_ksp_memory(1, no_path);
        no_path[0].route_id  = 0;
        no_path[0].vertex_id = start_id;
        no_path[0].cost = 0;
        no_path[0].edge_id = -1;
        return no_path;
}

