///////////////////////////////////////////////////////////////////////////////
///  Graph.h
///  <TODO: insert file description here>
///
///  @remarks <TODO: insert remarks here>
///
///  @author Yan Qi @date 8/18/2010
///
///  $Id: Graph.h 65 2010-09-08 06:48:36Z yan.qi.asu $
///////////////////////////////////////////////////////////////////////////////


#ifndef __Graph_h
#define __Graph_h

#include <set>
#include <map>
#include <vector>

#include "BasePath.h"

class BaseVertex;

class Path : public BasePath
{
public:

  Path(const std::vector<BaseVertex*>& vertex_list, double weight):BasePath(vertex_list,weight){}

  // display the content
  void PrintOut(std::ostream& out_stream) const
  {
    out_stream << "Graph Cost: " << m_dWeight << " Length: " << m_vtVertexList.size() << std::endl;
    for(std::vector<BaseVertex*>::const_iterator pos=m_vtVertexList.begin(); pos!=m_vtVertexList.end();++pos)
    {
      out_stream << (*pos)->getID() << " ";
    }
    out_stream << std::endl <<  "*********************************************" << std::endl;
  }
};

class Graph
{
public: // members

  const static double DISCONNECT;

  typedef std::set<BaseVertex*>::iterator VertexPtSetIterator;
  typedef std::map<BaseVertex*, std::set<BaseVertex*>*>::iterator BaseVertexPt2SetMapIterator;

protected: // members

  // Basic information
  std::map<BaseVertex*, std::set<BaseVertex*>*> m_mpFanoutVertices;
  std::map<BaseVertex*, std::set<BaseVertex*>*> m_mpFaninVertices;
  std::map<int, double> m_mpEdgeCodeWeight;
  std::vector<BaseVertex*> m_vtVertices;
  int m_nEdgeNum;
  int m_nVertexNum;

  std::map<int, BaseVertex*> m_mpVertexIndex;

  // Members for graph modification
  std::set<int> m_stRemovedVertexIds;
  std::set<std::pair<int,int> > m_stRemovedEdge;

public:

  // Constructors and Destructor
  Graph(const std::string& file_name);
  Graph(const Graph& rGraph);
  Graph();
  ~Graph(void);

  void clear();

  BaseVertex* get_vertex(int node_id);

  int get_edge_code(const BaseVertex* start_vertex_pt, const BaseVertex* end_vertex_pt) const;
  std::set<BaseVertex*>* get_vertex_set_pt(BaseVertex* vertex_, std::map<BaseVertex*, std::set<BaseVertex*>*>& vertex_container_index);

  double get_original_edge_weight(const BaseVertex* source, const BaseVertex* sink);

  double get_edge_weight(const BaseVertex* source, const BaseVertex* sink);
  void get_adjacent_vertices(BaseVertex* vertex, std::set<BaseVertex*>& vertex_set);
  void get_precedent_vertices(BaseVertex* vertex, std::set<BaseVertex*>& vertex_set);

  /// Methods for changing graph
  void remove_edge(const std::pair<int,int> edge)
  {
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

  void recover_removed_edge(const std::pair<int,int> edge)
  {
    m_stRemovedEdge.erase(m_stRemovedEdge.find(edge));
  }

  void recover_removed_vertex(int vertex_id)
  {
    m_stRemovedVertexIds.erase(m_stRemovedVertexIds.find(vertex_id));
  }

private:
  void _import_from_file(const std::string& file_name);

};

#endif
