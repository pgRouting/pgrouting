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

#ifndef __YenTopKShortestPathsAlg_h
#define __YenTopKShortestPathsAlg_h

#include <vector>
#include <map>
#include <set>

class Graph;
class BasePath;
template < class T > class WeightLess;

class YenTopKShortestPathsAlg
{
  Graph* m_pGraph;

  std::vector<BasePath*> m_vResultList;
  std::map<BasePath*, BaseVertex*> m_mpDerivationVertexIndex;
  std::multiset<BasePath*, WeightLess<BasePath> > m_quPathCandidates;

  BaseVertex* m_pSourceVertex;
  BaseVertex* m_pTargetVertex;

  int m_nGeneratedPathNum;

private:

  void _init();

public:

  YenTopKShortestPathsAlg(const Graph& graph)
  {
    YenTopKShortestPathsAlg(graph, NULL, NULL);
  }

  YenTopKShortestPathsAlg(const Graph& graph, BaseVertex* pSource, BaseVertex* pTarget)
    :m_pSourceVertex(pSource), m_pTargetVertex(pTarget)
  {
    m_pGraph = new Graph(graph);
    _init();
  }

  ~YenTopKShortestPathsAlg(void){clear();}

  void clear();
  bool has_next();
  BasePath* next();

  BasePath* get_shortest_path(BaseVertex* pSource, BaseVertex* pTarget);
  void get_shortest_paths(BaseVertex* pSource, BaseVertex* pTarget, int top_k,
    std::vector<BasePath*>&);
};

#endif
