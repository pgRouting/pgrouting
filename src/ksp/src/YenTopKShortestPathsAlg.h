///////////////////////////////////////////////////////////////////////////////
///  YenTopKShortestPathsAlg.h
///  The implementation of Yen's algorithm to get the top k shortest paths 
///  connecting a pair of vertices in a graph. 
///
///  @remarks <TODO: insert remarks here>
///
///  @author Yan Qi @date 7/10/2010
/// 
///  $Id: YenTopKShortestPathsAlg.h 65 2010-09-08 06:48:36Z yan.qi.asu $
///
///////////////////////////////////////////////////////////////////////////////
#ifndef _YENTOPKSHORTESTPATHSALG_H
#define _YENTOPKSHORTESTPATHSALG_H

#include <deque>
#include "DijkstraShortestPathAlg.h"

class YenTopKShortestPathsAlg: public DijkstraShortestPathAlg
{
	std::deque<BasePath> m_ResultList;
        std::deque<BasePath> m_Heap;

        int m_Source_id;
        int m_Target_id;
        POS sourceID;
        POS targetID;
        int m_K;

	//map<BasePath*, BaseVertex*> m_mpDerivationVertexIndex;
	//multiset<BasePath*, WeightLess<BasePath> > m_quPathCandidates;

	//BaseVertex* m_pSourceVertex;
	//BaseVertex* m_pTargetVertex;

	//int m_nGeneratedPathNum;

private:
	BasePath get_shortest_path(POS sourcePos, POS targetPos);
	void get_shortest_paths(POS pSource, POS pTarget, int top_k);
	void _init();

public:
/*
	YenTopKShortestPathsAlg(const Graph& graph)
	{
		YenTopKShortestPathsAlg(graph, NULL, NULL);
	}
*/
	YenTopKShortestPathsAlg(const Graph& graph)
                : DijkstraShortestPathAlg(graph),
		 m_Source_id(0), m_Target_id(0),
                 m_K(1)
	{ }

	~YenTopKShortestPathsAlg(void){ clear(); }
        std::deque<BasePath> Yen( int source, int target, int numb_paths );
	void clear();
	//bool has_next();	
	void next();

};
#endif
