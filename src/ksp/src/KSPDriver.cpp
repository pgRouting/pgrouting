#include <vector>
#include <set>
#include <map>
#include "KSPGraph.h"
#include "YenTopKShortestPathsAlg.h"
#include "ksp.h"
#include "KSPDriver.h"
static  void dpPrint(const KSPGraph &theGraph,const BasePath &thePath, ksp_path_element_t *path, int &sequence, int route_id);


int  doKpaths(ksp_edge_t  * edges,int total_tuples,int  start_vertex,int  end_vertex,
                       int no_paths, bool has_reverse_cost,
                       ksp_path_element_t **path, int *path_count,
			char ** err_msg){

  	std::map<int,int>vertexs;
	KSPGraph theGraph = KSPGraph();

        theGraph.StartLoad();
        theGraph.AddData( edges, total_tuples, has_reverse_cost );
        theGraph.EndLoad();

        POS start_id = theGraph.find_vertex(start_vertex);
        POS end_id = theGraph.find_vertex(start_vertex);
        
        if ( !theGraph.checkVertexIDs( start_id, start_vertex)  
            || !theGraph.checkVertexIDs( end_id, end_vertex) ) {
             //TODO  insert a message that says could not find the start or the ending vertex
            return 0;
        };
        

        YenTopKShortestPathsAlg yenAlg(theGraph, start_id, end_id, no_paths);
        std::deque<BasePath> paths;
//        yenAlg.execute( paths );


        int count=0;
	for( unsigned int i = 0; i < paths.size(); ++i )
           if ( paths[i].size() > 1 )  // don't count empty routes
              count += paths[i].size() + 1;   // add final vertex

        //get the space required to store all the paths
        ksp_path_element_t *ksp_path;
        ksp_path=0;
	ksp_path = get_ksp_memory(count, ksp_path );

        int sequence = 0;
        for (unsigned int route_id = 0; route_id < paths.size(); route_id++) {
		dpPrint(theGraph, paths[route_id], ksp_path, sequence, route_id);
  	}

        if (count != sequence-1) {
            // message we got a terrible error
            return 0;
        }
	*path=ksp_path;
        *path_count = count;
	return 1;
}

static  void dpPrint(const KSPGraph &theGraph,const BasePath &thePath, ksp_path_element_t *path, int &sequence, int route_id){

        // the row data:  seq, route, nodeid, edgeId, cost
        int nodeId, edgeId, lastNodeId;
        double cost;

	for(unsigned int i = 0; i < thePath.size(); i++){
                edgeId = thePath.getID(i); 
                nodeId = thePath[i]->getStart(); 
                cost = thePath[i]->Weight();
                if (i == thePath.size()-1)
                      lastNodeId=  thePath[i]->getEnd();

		path[sequence].route_id = route_id;
		path[sequence].vertex_id = nodeId;
		path[sequence].cost = cost;
		path[sequence].edge_id = edgeId;
                if (i == thePath.size()-1) {
		        path[sequence].route_id = route_id;
		        path[sequence].vertex_id = lastNodeId;
			path[sequence].cost = 0;
			path[sequence].edge_id=-1;
		}
		sequence++;
	}
}

