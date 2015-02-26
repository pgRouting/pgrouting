///////////////////////////////////////////////////////////////////////////////
///  BaseGraph.h
///  <TODO: insert file description here>
///
///  @remarks <TODO: insert remarks here>
///
///  @author Yan Qi @date 6/6/2010
///
///  $Id: BaseGraph.h 57 2010-08-16 07:03:32Z yan.qi.asu $
///
///////////////////////////////////////////////////////////////////////////////

#ifndef __BaseGraph_h
#define __BaseGraph_h

#include <set>
#include <vector>
#include <map>
#include "GraphElements.h"

class BaseGraph
{
public: // members

  const static double DISCONNECT;

  typedef std::set<BaseVertex*>::iterator VertexPtSetIterator;
  typedef std::map<BaseVertex*, std::set<BaseVertex*>*>::iterator BaseVertexPt2SetMapIterator;

protected: // members

  std::map<BaseVertex*, std::set<BaseVertex*>*> m_mpFanoutVertices;
  std::map<BaseVertex*, std::set<BaseVertex*>*> m_mpFaninVertices;
  std::map<int, double> m_mpEdgeCodeWeight;
  std::vector<BaseVertex*> m_vtVertices;
  int m_nEdgeNum;
  int m_nVertexNum;


public: // methods

  BaseGraph(){}; //default constructor
  BaseGraph(const BaseGraph& graph) /// copy constructor
  {
    m_nVertexNum = graph.m_nVertexNum;
    m_nEdgeNum = graph.m_nEdgeNum;
    m_vtVertices.assign(graph.m_vtVertices.begin(),graph.m_vtVertices.end());
    m_mpFaninVertices.insert(graph.m_mpFaninVertices.begin(),graph.m_mpFaninVertices.end());
    m_mpFanoutVertices.insert(graph.m_mpFanoutVertices.begin(),graph.m_mpFanoutVertices.end());
    m_mpEdgeCodeWeight.insert(graph.m_mpEdgeCodeWeight.begin(),graph.m_mpEdgeCodeWeight.end());
  }
  virtual ~BaseGraph(void){clear();}

  void clear();

protected: // methods

  int get_edge_code(const BaseVertex* start_vertex_pt, const BaseVertex* end_vertex_pt) const
  {
    /// Note that the computation below works only if
    /// the result is smaller than the maximum of an integer!
    return start_vertex_pt->getID()*m_nVertexNum+end_vertex_pt->getID();
  }

public:

  virtual double get_edge_weight(const BaseVertex* start_vertex_pt, const BaseVertex* end_vertex_pt) const
  {
    std::map<int, double>::const_iterator pos =
      m_mpEdgeCodeWeight.find(get_edge_code(start_vertex_pt, end_vertex_pt));

    if (pos != m_mpEdgeCodeWeight.end())
    {
      return pos->second;
    }

    return DISCONNECT;
  }

  virtual std::set<BaseVertex*>* get_precedent_vertex_set(BaseVertex* vertex)
  {
    return get_vertex_set_pt(vertex, m_mpFaninVertices);
  }

  virtual std::set<BaseVertex*>* get_adjacent_vertex_set(BaseVertex* vertex)
  {
    return get_vertex_set_pt(vertex, m_mpFanoutVertices);
  }

  //////////////////////////////////////////////////////////////////////////
  // Non-virtual methods
  //////////////////////////////////////////////////////////////////////////
  BaseVertex* get_vertex_by_ID(size_t id) const
  {
    return m_vtVertices.at(id);
  }

  std::set<BaseVertex*>* get_vertex_set_pt(BaseVertex* vertex_,
    std::map<BaseVertex*, std::set<BaseVertex*>*>& vertex_container_index)
  {
    BaseVertexPt2SetMapIterator pos = vertex_container_index.find(vertex_);

    if(pos == vertex_container_index.end())
    {
      std::set<BaseVertex*>* vertex_set = new std::set<BaseVertex*>();
      std::pair<BaseVertexPt2SetMapIterator,bool> ins_pos =
        vertex_container_index.insert(std::make_pair(vertex_, vertex_set));

      pos = ins_pos.first;
    }

    return pos->second;
  }
};

#endif
