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
#include <cassert>
#include "GraphElements.h"
#include "Graph.h"
#include "DijkstraShortestPathAlg.h"
#include "YenTopKShortestPathsAlg.h"

using namespace std;

void YenTopKShortestPathsAlg::clear()
{
        DijkstraShortestPathAlg::clear();
	m_ResultList.clear();
        m_Heap.clear();
}

void YenTopKShortestPathsAlg::_init()
{
	clear();
	BasePath shortestPath = Dijkstra(m_Source_id, m_Target_id);
        DijkstraShortestPathAlg::clear();
#if 0
shortestPath.PrintOut(std::cout);
#endif
 	if ( !shortestPath.isEmpty()) {   //to be empty has no edges
	    m_ResultList.push_back(shortestPath);
        }
}


std::deque<BasePath> YenTopKShortestPathsAlg::Yen( int  source, int  sink, int K ) {
	clear();
        if (  source !=sink && K > 0 && exist_vertex(source) && exist_vertex(sink) ) {

	     m_Source_id = source;
	     m_Target_id = sink;
             sourceID = find_vertex(source);
             targetID = find_vertex(sink);
             get_shortest_paths( find_vertex(source), find_vertex(sink), K );
#if 0
std::cout<<"paths found "<<m_ResultList.size()<<"\n";
#endif
        }
        return m_ResultList;
}


BasePath YenTopKShortestPathsAlg::get_shortest_path( POS  sourcePos, POS  targetPos )
{
	int source_id = m_vtVertices[ sourcePos ].getID();
	int target_id = m_vtVertices[ targetPos ].getID();
	return Dijkstra( source_id,  target_id );
}

/*
bool YenTopKShortestPathsAlg::has_next()
{
	return !m_quPathCandidates.empty();
}
*/
void YenTopKShortestPathsAlg::next()
{
#if 0
std::cout<<"Entering YenTopKShortestPathsAlg::next\n";
#endif
        
        POS currPathId =  m_ResultList.size()-1 ;
	BasePath curr_result_path = m_ResultList[ currPathId ];
#if 0
std::cout<<"current path\n";
curr_result_path.PrintOut(std::cout);
std::cout<<"size is "<<curr_result_path.size()<<"\n";
#endif
//        POS currPathSize= curr_result_path.size();


        BaseEdge* spurEdge;
        POS spurNode;
        BasePath rootPath;
	for (POS i = 0; i < curr_result_path.size(); ++i)
	{
#if 0
std::cout<<"Entering cycle\n";
#endif
            spurEdge = curr_result_path[i];
#if 0
std::cout<<"\nspur Edge\n";
spurEdge->PrintOut(std::cout);
#endif
            spurNode = spurEdge->getStart();
#if 0
std::cout<<"\nspur node\n";
m_vtVertices[spurNode].PrintOut(std::cout, m_vtEdges);
#endif
            curr_result_path.subPath(rootPath, i);  
#if 0
//std::cout<<"SubPath results:\ncurrent path\n";
//curr_result_path.PrintOut(std::cout);
std::cout<<"root path\n";
rootPath.PrintOut(std::cout);
#endif

		
               //for each path p in A:
               //   if rootPath == p.nodes(0, i):
               //     Remove the links that are part of the previous shortest paths which share the same root path.
               //     remove p.edge(i, i + 1) from Graph;

#if 0
std::cout<<"\nGraph status before removed edges\n";
PrintOut(std::cout);
#endif
            for ( POS j=0; j < m_ResultList.size(); j++) {
               BasePath workingPath = m_ResultList[j]; // p (luego lo arreglo para que no se copie
#if 0
std::cout<<"\nworkingPath #"<<j<<"\n";
workingPath.PrintOut(std::cout);
#endif
 	       if (rootPath.isEqual(workingPath) ) {  //this isequal compares up to the size of rootPath
	            int edgeToBeRemoved = workingPath[i]->getNID(); 
		    remove_edge( edgeToBeRemoved );
#if 0
std::cout<<"\nremoved Edge for processing\n";
m_vtEdges[ edgeToBeRemoved ].PrintOut(std::cout);
#endif
	       }
             }

        

               removeNodes( rootPath ); 

               BasePath spurPath;
#if 0
std::cout<<"\n !!!!!!!!!going to Dijstra" <<  m_vtVertices[ spurNode ].getID() <<" to "<< m_Target_id <<"\n";
std::cout<<"aka" <<  spurNode  <<" to "<< targetID <<"\n";
if (spurNode ==1 ) PrintOut(std::cout);
#endif
               //spurPath = DijkstraShortestPathAlg::Dijkstra( m_vtVertices[ spurNode ].getID(), m_Target_id);
               spurPath = DijkstraShortestPathAlg::Dijkstra(  spurNode , targetID, true);
               DijkstraShortestPathAlg::clear();
#if 0
std::cout<<"\nGraph status after Dij clear\n";
PrintOut(std::cout);
std::cout<<"\n*****************\n";
#endif

  
#if 0
std::cout<<"root path\n";
rootPath.PrintOut(std::cout);
std::cout<<"spur path\n";
spurPath.PrintOut(std::cout);
#endif
               // Entire path is made up of the root path and spur path.
               rootPath.append(spurPath);  //this should also update the cost of the totalPath
#if 0
std::cout<<"total path\n";
rootPath.PrintOut(std::cout);
#endif

               // Add the potential k-shortest path to the heap.
               if (rootPath.FromTo(sourceID, targetID)) {
                   m_Heap.push_back(rootPath); 
#if 0
std::cout<<"\npath added to HEAP\n";
rootPath.PrintOut(std::cout);
//assert(true==false);
#endif
             }
	}
}

/*
bool OrderByCost (const BasePath &p1,const BasePath &p2) { 
       return p1.Weight() < p2.Weight()
              || p1.size() < p2.size()
              || p1.EdgesLessComapre(p2);
}
*/
void YenTopKShortestPathsAlg::get_shortest_paths( POS source_id, POS target_id, int K)
{

	  _init(); // get the best using Dijkstra
	  while ( m_ResultList.size() < (unsigned int) K )
	  {
		next();
                if ( m_Heap.empty() ) break;
//                std::sort( m_Heap.begin() , m_Heap.end() );  // , OrderByCost );
                m_ResultList.push_back( m_Heap[0] );
#if 0
std::cout<<"\npath added to results\n";
m_ResultList[ m_ResultList.size()-1 ].PrintOut(std::cout);
//assert(true==false);
#endif
                m_Heap.erase( m_Heap.begin() );
	  }
}
