///////////////////////////////////////////////////////////////////////////////
///  YenTopKShortestPathsAlg.cpp
///  The implementation of Yen's algorithm to get the top k shortest paths 
///  connecting a pair of vertices in a graph. 
///
///  @remarks <TODO: insert remarks here>
///
///  @author Yan Qi @date 7/10/2010
///
///  $Id: YenTopKShortestPathsAlg.cpp 65 2010-09-08 06:48:36Z yan.qi.asu $
///
///////////////////////////////////////////////////////////////////////////////

//#include <set>
//#include <map>
//#include <algorithm>
#include <deque>
#include "GraphElements.h"
#include "Graph.h"
#include "DijkstraShortestPathAlg.h"
#include "YenTopKShortestPathsAlg.h"

using namespace std;

void YenTopKShortestPathsAlg::clear()
{
	//m_nGeneratedPathNum = 0;
	//m_mpDerivationVertexIndex.clear();
	m_ResultList.clear();
	//m_PathCandidates.clear();
        m_Heap.clear();
}

void YenTopKShortestPathsAlg::_init()
{
	clear();
        Graph graph(m_Graph);
	BasePath shortestPath = graph.Dijkstra(m_Source_id, m_Target_id);
 	if ( !shortestPath.isEmpty()) {   //to be empty has no edges
	    m_ResultList.push_back(shortestPath);
        }
}

BasePath* YenTopKShortestPathsAlg::get_shortest_path( BaseVertex* pSource, BaseVertex* pTarget )
{
	DijkstraShortestPathAlg dijkstra_alg(m_Graph);
	return dijkstra_alg.get_shortest_path(pSource, pTarget);
}
/*
bool YenTopKShortestPathsAlg::has_next()
{
	return !m_quPathCandidates.empty();
}
*/
void YenTopKShortestPathsAlg::next()
{
        Graph graph(m_Graph);

	//1. Prepare for removing vertices and arcs
	//BasePath* cur_path = *(m_quPathCandidates.begin());
	//m_quPathCandidates.erase(m_quPathCandidates.begin());
	//m_vResultList.push_back(cur_path);

	//POS count = m_vResultList.size();
	
	//BaseVertex* cur_derivation_pt = m_mpDerivationVertexIndex.find(cur_path)->second; 
	//std::deque<BaseEdge*> sub_path_of_derivation_pt;
	//cur_path->SubPath(sub_path_of_derivation_pt, cur_derivation_pt);
	//POS sub_path_length = sub_path_of_derivation_pt.size();
        
        // Spur node is retrieved from the previous k-shortest path, k − 1.  currPathId= k-1 
        //   spurNode = A[k-1].node(i);    
        // The sequence of nodes from the source to the spur node of the previous k-shortest path.
        //   rootPath = A[k-1].nodes(0, i);
       
        POS currPathId =  m_ResultList.size()-1 ;
	BasePath cur_result_path = m_ResultList[ currPathId ];
        BaseEdge* spurEdge = cur_result_path[currPathId];
        //POS spurEdgeId = spurEdge->getID();
        POS spurNode = spurEdge->getStart() ;

        BasePath rootPath(m_Source_id);
        cur_result_path.subPath(rootPath, currPathId);    //con (0,0)  regresa un path vacio pero el source debe ser el primer vertice del primer edge del path);

	for (POS i=0; i<m_ResultList.size()-1; ++i)
	{
		
               //for each path p in A:
               //   if rootPath == p.nodes(0, i):
               //     Remove the links that are part of the previous shortest paths which share the same root path.
               //     remove p.edge(i, i + 1) from Graph;

               BasePath workingPath = m_ResultList[i]; // p
 	       if (rootPath.isEqual(workingPath) ) {  //this isequal compares up to the size of rootPath
	            int edgeToBeRemoved = workingPath[i]->getID(); 
		    graph.remove_edge( edgeToBeRemoved );
	       }
        
	       //2.1 remove vertices and edges along the current result
               // for each node  in rootPath except spurNode:
               //       remove node from Graph;
               graph.removeNodes( rootPath ); //should remove the nodes that are in rootpath except the last node (that belongs to the spureEdge

               // Calculate the spur path from the spur node to the sink.
               BasePath spurPath( spurNode );
               spurPath = graph.Dijkstra(spurNode, m_Target_id);
  
               // Entire path is made up of the root path and spur path.
               rootPath.append(spurPath);  //this should also update the cost of the totalPath

               // Add the potential k-shortest path to the heap.
               if (rootPath.FromTo(m_Source_id, m_Target_id))
                   m_Heap.push_back(rootPath); 
	}
}

/*
bool OrderByCost (const BasePath &p1,const BasePath &p2) { 
       return p1.Weight() < p2.Weight()
              || p1.size() < p2.size()
              || p1.EdgesLessComapre(p2);
}
*/
void YenTopKShortestPathsAlg::get_shortest_paths( BaseVertex* pSource, 
	BaseVertex* pTarget, int top_k, std::deque<BasePath>& result_list)
{
        if (pSource != NULL && pTarget != NULL)
        {
	  m_Source_id = pSource->getNID();
	  m_Target_id = pTarget->getNID();

	  _init();
	  int count = 0; 
	  while ( count < top_k)
	  {
		next();
                if ( m_Heap.empty() ) break;
//                std::sort( m_Heap.begin() , m_Heap.end() );  // , OrderByCost );
                m_ResultList.push_back( m_Heap[0] );
                m_Heap.erase( m_Heap.begin() );
		++count;
	  }
	  result_list = m_ResultList;
         }
}
