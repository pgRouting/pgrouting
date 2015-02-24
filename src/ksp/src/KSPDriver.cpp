#include <vector>
#include <set>
#include <map>
#include "KSPGraph.h"
#include "YenTopKShortestPathsAlg.h"
#include "ksp.h"
#include "KSPDriver.h"
static ksp_path_element_t * dpPrint(const KSPGraph &theGraph,const BasePath &thePath, ksp_path_element_t *ksp_path, int *path_count,int,int *);

#define MEM_OSET	10

int  doKpaths(ksp_edge_t  * edges,int total_tuples,int  start_vertex,int  end_vertex,
                       int no_paths, bool has_reverse_cost,
                       ksp_path_element_t **path, int *path_count,
			char ** err_msg){
	int i;
	int mem_offset;
        ksp_path_element_t *ksp_path;
	bool no_reverse=false;

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
        };
        

        YenTopKShortestPathsAlg yenAlg(theGraph, start_id, end_id, no_paths);
        std::deque<BasePath> paths;
        yenAlg.execute( paths );

        i=0;

	ksp_path=0;
	mem_offset = MEM_OSET;
	ksp_path = get_ksp_memory(mem_offset, ksp_path );

	*path_count=paths.size();
        for (int i = 1; i < paths.size(); i++) {
		ksp_path=dpPrint(theGraph, paths[i], ksp_path, path_count, i, &mem_offset);
  	}

	*path=ksp_path;
	return 1;
}

static ksp_path_element_t* dpPrint(const KSPGraph &theGraph,const BasePath &thePath, ksp_path_element_t *path, int *path_count,int cnt,int *memCnt){

        // the row data:  seq, route, nodeid, edgeId, cost
        int nodeId, edgeId, lastNodeId;
        double cost;

	for(int i = 0; i < thePath.size(); i++){
                edgeId = thePath.getID(i); 
                nodeId = thePath[i]->getStart(); 
                cost = thePath[i]->Weight();
                if (i == thePath.size()-1)
                      lastNodeId=  thePath[i]->getEnd();

		if((*path_count)+1 >= *memCnt){
			(*memCnt) += MEM_OSET;
			path=get_ksp_memory(*memCnt,path);
		}
		path[*path_count].route_id = cnt;
		path[*path_count].vertex_id = nodeId;
		path[*path_count].cost = cost;
		path[*path_count].edge_id = edgeId;
                if (i == thePath.size()-1) {
		        (*path_count)++;
		        path[*path_count].route_id = cnt;
		        path[*path_count].vertex_id = lastNodeId;
			path[*path_count].cost = 0;
			path[*path_count].edge_id=-1;
		}
		(*path_count)++;
	}
	return path;
}

