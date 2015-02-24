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

#pragma once

using namespace std;

class YenTopKShortestPathsAlg
{
	Graph m_Graph;

	std::deque<BasePath> m_ResultList;
        std::deque<BasePath> m_Heap;

        POS m_Source_id;
        POS m_Target_id;
        int m_K;

	//map<BasePath*, BaseVertex*> m_mpDerivationVertexIndex;
	//multiset<BasePath*, WeightLess<BasePath> > m_quPathCandidates;

	//BaseVertex* m_pSourceVertex;
	//BaseVertex* m_pTargetVertex;

	//int m_nGeneratedPathNum;

private:
	BasePath* get_shortest_path(BaseVertex* pSource, BaseVertex* pTarget);
	void get_shortest_paths(BaseVertex* pSource, BaseVertex* pTarget, int top_k, 
		std::deque<BasePath>&);

	void _init();

public:
/*
	YenTopKShortestPathsAlg(const Graph& graph)
	{
		YenTopKShortestPathsAlg(graph, NULL, NULL);
	}
*/
	YenTopKShortestPathsAlg(const Graph& graph, POS pSource, POS pTarget, int K)
                : m_Graph(graph),
		 m_Source_id(pSource), m_Target_id(pTarget),
                 m_K(K)
	{
		_init();
	}

	~YenTopKShortestPathsAlg(void){clear();}
        void execute( std::deque<BasePath>& );
	void clear();
	//bool has_next();	
	void next();

};
