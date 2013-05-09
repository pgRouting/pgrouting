#include <map>
#include "KSPGraph.h"
#include "YenTopKShortestPathsAlg.h"
#include "ksp.h"
#include "KSPDriver.h"
static ksp_path_element_t * dpPrint(KSPGraph *theGraph,BasePath *thePath, ksp_path_element_t *ksp_path, int *path_count,int,int *);

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
	// Only include routes who have a cost great or equal zero
        // represent one way values
  	for(i=0; i<total_tuples; i++) {
    		// count the total number of vertexs
		if(edges[i].cost >=0 ){
    			vertexs[edges[i].source]=1;
    			vertexs[edges[i].target]=1;
		}
	}



	theGraph.StartLoad(vertexs.size());
	

	for(i=0;i<total_tuples;i++){
		if(edges[i].cost >= 0 ){
			theGraph.AddData(edges[i].source,edges[i].target,edges[i].cost,edges[i].id);
			if( has_reverse_cost && edges[i].reverse_cost >= 0){
				theGraph.AddData(edges[i].target,edges[i].source,edges[i].reverse_cost,edges[i].id);
				no_reverse=true;
			}
		}
		
	}
	// avoid a nasty bug, if no reverse paths are define and reverse mode
        // is set, an endless loop is entered.
	if(no_reverse == false && has_reverse_cost){
		*err_msg = (char *) "No reverse paths defined and reverse cost selected";
		return -1;
	}
	theGraph.EndLoad();
        YenTopKShortestPathsAlg yenAlg(theGraph, theGraph.get_vertex(start_vertex),theGraph.get_vertex(end_vertex));

        i=0;

	ksp_path=0;
	mem_offset=MEM_OSET;
	ksp_path=get_ksp_memory(mem_offset,ksp_path);

	*path_count=0;
        if( no_paths <= 0 ) { // print all paths

		while(yenAlg.has_next()) {
      			i++;
			ksp_path=dpPrint(&theGraph,yenAlg.next(),ksp_path,path_count,i,&mem_offset);
    		}
  	} else {
  		std::vector< BasePath* > paths;
  		yenAlg.get_shortest_paths( theGraph.get_vertex(start_vertex), theGraph.get_vertex(end_vertex), no_paths, paths );
  		std::vector< BasePath* >::const_iterator it = paths.begin();
		int i=0;
  		while( it != paths.end() ) {
    		
			ksp_path=dpPrint(&theGraph,*it,ksp_path,path_count,i,&mem_offset);
    			++it;
			i++;
    		}

  	}

	*path=ksp_path;
	return 1;
}

static ksp_path_element_t* dpPrint(KSPGraph *theGraph,BasePath *thePath, ksp_path_element_t *path, int *path_count,int cnt,int *memCnt){

	int i;
	for(i=0;i<thePath->length();i++){

		if((*path_count)+1 >= *memCnt){
			(*memCnt) += MEM_OSET;
			path=get_ksp_memory(*memCnt,path);
		}
		BaseVertex  bv= *thePath->GetVertex(i);
		path[*path_count].route_id= cnt;
		path[*path_count].cost= 0.0;
		path[*path_count].edge_id= 0;
		path[*path_count].vertex_id= bv.getID();
		if((i+1 )< thePath->length()){
			BaseVertex  destbv= *thePath->GetVertex(i+1);
			(path)[*path_count].cost= theGraph->get_original_edge_weight(&bv, &destbv);
			(path)[*path_count].edge_id= theGraph->get_edge_value(&bv, &destbv);

		}
		(*path_count)++;
	}
	return path;
}
