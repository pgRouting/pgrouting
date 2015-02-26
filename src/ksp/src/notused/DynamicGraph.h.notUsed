///////////////////////////////////////////////////////////////////////////////
///  VariableGraph.h
///  Depict a graph that can change dynamically!
///
///  @remarks <TODO: insert remarks here>
///
///  @author Yan Qi @date 6/13/2010
///
///  $Id: DynamicGraph.h 58 2010-08-19 07:11:58Z yan.qi.asu $
///
///////////////////////////////////////////////////////////////////////////////

#ifndef __DynamicGraph_h
#define __DynamicGraph_h

#include "Graph.h"

using namespace std;

class DynamicGraph : public Graph
{
  set<int> m_stRemovedVertexIds;
  set<pair<int,int> > m_stRemovedEdge;

public:
  /*DynamicGraph(void){}*/
  DynamicGraph(const string graph_file_name):Graph(graph_file_name){}
  DynamicGraph(const Graph& rGraph):Graph(rGraph){}
  ~DynamicGraph(void){}

  void remove_edge(const pair<int,int> edge)
  {
    //m_stRemovedEdge.erase(m_stRemovedEdge.find(edge));
    m_stRemovedEdge.insert(edge);
  }

  void remove_vertex(const int vertex_id)
  {
    m_stRemovedVertexIds.insert(vertex_id);
  }

  void recover_removed_edges()
  {
    m_stRemovedEdge.clear();
  }

  void recover_removed_vertices()
  {
    m_stRemovedVertexIds.clear();
  }

  void recover_removed_edge(const pair<int,int> edge)
  {
    m_stRemovedEdge.erase(m_stRemovedEdge.find(edge));
  }

  void recover_removed_vertex(int vertex_id)
  {
    m_stRemovedVertexIds.erase(m_stRemovedVertexIds.find(vertex_id));
  }

  double get_edge_weight_of_graph(const BaseVertex* source, const BaseVertex* sink)
  {
    return Graph::get_edge_weight(source, sink);
  }

  BaseVertex* get_vertex(int id)
  {
    if (m_stRemovedVertexIds.find(id) != m_stRemovedVertexIds.end())
    {
      return NULL;
    }else
    {
      return Graph::get_vertex(id);
    }
  }

  void get_vertex_list(vector<BaseVertex*>& vertex_list);

  virtual double get_edge_weight(const BaseVertex* source, const BaseVertex* sink);

  virtual void get_adjacent_vertices(BaseVertex* vertex, set<BaseVertex*>& vertex_set);

  virtual void get_precedent_vertices(BaseVertex* vertex, set<BaseVertex*>& vertex_set);

};

#endif
