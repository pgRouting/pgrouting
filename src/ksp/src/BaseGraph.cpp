///////////////////////////////////////////////////////////////////////////////
///  BaseGraph.cpp
///  <TODO: insert file description here>
///
///  @remarks <TODO: insert remarks here>
///
///  @author Yan Qi @date 6/6/2010
///
///  $Id: BaseGraph.cpp 52 2010-06-14 06:53:06Z yan.qi.asu $
///
///////////////////////////////////////////////////////////////////////////////

#include <limits>
#include <string>
#include <algorithm>
#include "BaseGraph.h"

const double BaseGraph::DISCONNECT = (std::numeric_limits<double>::max)();

void BaseGraph::clear()
{
  m_nEdgeNum = 0;
  m_nVertexNum = 0;

  for(std::map<BaseVertex*, std::set<BaseVertex*>*>::const_iterator pos=m_mpFaninVertices.begin();
    pos!=m_mpFaninVertices.end(); ++pos)
  {
    delete pos->second;
  }
  m_mpFaninVertices.clear();

  for(std::map<BaseVertex*, std::set<BaseVertex*>*>::const_iterator pos=m_mpFanoutVertices.begin();
    pos!=m_mpFanoutVertices.end(); ++pos)
  {
    delete pos->second;
  }
  m_mpFanoutVertices.clear();


  m_mpEdgeCodeWeight.clear();

  /*m_stVertices.clear();*/
  /*m_mpVertexIndex.clear();*/

  //clear the list of vertices objects
  for_each(m_vtVertices.begin(), m_vtVertices.end(), DeleteFunc<BaseVertex>());
  m_vtVertices.clear();
}
