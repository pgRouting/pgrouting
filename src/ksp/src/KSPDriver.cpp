#include <deque>

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
static  ksp_path_element_t * noPathFound(int start_id) {
        ksp_path_element_t *no_path;
        no_path = get_ksp_memory(1, no_path);
        no_path[0].route_id  = 0;
        no_path[0].vertex_id = start_id;
        no_path[0].cost = 0;
        no_path[0].edge_id = -1;
        return no_path;
}

int  doKpaths(ksp_edge_t  * edges, int total_tuples,
                       int  start_vertex, int  end_vertex,
                       int no_paths, bool has_reverse_cost,
                       ksp_path_element_t **path, int *path_count,
                       char ** err_msg) {
        KSPGraph theGraph = KSPGraph();

        theGraph.StartLoad();
        theGraph.AddData(edges, total_tuples, has_reverse_cost);
        theGraph.EndLoad();
        (*path_count) = 0;


        if ( !theGraph.exist_vertex(start_vertex)) {
            *err_msg = strdup( "Starting vertex not found on any edge" );
            (*path_count) = 1;
            *path = noPathFound(start_vertex);
            return 0;
        }

        if (!theGraph.exist_vertex(end_vertex)) {
            *err_msg = strdup( "Ending vertex not found" );
            return -1;
        }

        if (start_vertex == end_vertex) {
            *err_msg = strdup( "Starting and Ending vertices are the same" );
            return -1;
        }

        YenTopKShortestPathsAlg yenGraph(theGraph);

        std::deque<BasePath> paths;
        paths = yenGraph.Yen(start_vertex, end_vertex, no_paths);



        int count = 0;
        for (unsigned int i = 0; i < paths.size(); ++i )
           if (paths[i].size() > 1)  // don't count empty routes
              count += paths[i].size() + 1;   // add final vertex

        // get the space required to store all the paths
        ksp_path_element_t *ksp_path;
        ksp_path = 0;
        ksp_path = get_ksp_memory(count, ksp_path);

        int sequence = 0;
        for (unsigned int route_id = 0; route_id < paths.size(); route_id++) {
               dpPrint(theGraph, paths[route_id], ksp_path, sequence, route_id);
          }

        if (count != sequence-1) {
            // message we got a terrible error
            return 0;
        }
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
                nodeId = thePath[i]->getStart();
                cost = thePath[i]->Weight();
                if (i == thePath.size()-1)
                      lastNodeId = thePath[i]->getEnd();

               path[sequence].route_id = route_id;
               path[sequence].vertex_id = nodeId;
               path[sequence].cost = cost;
               path[sequence].edge_id = edgeId;
                if (i == thePath.size()-1) {
                       path[sequence].route_id = route_id;
                       path[sequence].vertex_id = lastNodeId;
                      path[sequence].cost = 0;
                      path[sequence].edge_id = -1;
               }
               sequence++;
        }
}

