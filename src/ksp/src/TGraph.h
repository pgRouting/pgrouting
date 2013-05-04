///////////////////////////////////////////////////////////////////////////////
///  TGraph.h
///  <TODO: insert file description here>
///
///  @remarks <TODO: insert remarks here>
///
///  @author Yan Qi @date 5/29/2010
/// 
/// $Id: TGraph.h 47 2010-06-07 06:45:09Z yan.qi.asu $
///////////////////////////////////////////////////////////////////////////////

#ifndef __TGraph_h
#define __TGraph_h

using namespace std;

template<typename T>
class TGraph
{
public: // members

  const static double DISCONNECT; 
  
  typedef GVertex<T> TVertex;
  typedef typename set<TVertex*>::iterator VertexPtSetIterator;
  typedef typename map<TVertex*, set<TVertex*>*>::iterator TVertexPt2SetMapIterator;

protected: // members

  map<TVertex*, set<TVertex*>*> m_mpFanoutVertices;
  map<TVertex*, set<TVertex*>*> m_mpFaninVertices;
  map<int, double> m_mpEdgeCodeWeight; 
  //set<T> m_stVertices;
  map<T, TVertex*> m_mpVertexIndex;
  vector<TVertex*> m_vtVertices;
  int m_nEdgeNum;
  int m_nVertexNum;


public: // methods

  virtual ~TGraph(void){clear();}

  void clear()
  {
    m_nEdgeNum = 0;
    m_nVertexNum = 0;

    for(map<TVertex*, set<TVertex*>*>::const_iterator pos=m_mpFaninVertices.begin();
      pos!=m_mpFaninVertices.end(); ++pos)
    {
      delete pos->second;
    }
    m_mpFaninVertices.clear();

    for(map<TVertex*, set<TVertex*>*>::const_iterator pos=m_mpFanoutVertices.begin();
      pos!=m_mpFanoutVertices.end(); ++pos)
    {
      delete pos->second;
    }
    m_mpFanoutVertices.clear();
    

    m_mpEdgeCodeWeight.clear();

    /*m_stVertices.clear();*/
    m_mpVertexIndex.clear();

    //clear the list of vertices objects
    for_each(m_vtVertices.begin(), m_vtVertices.end(), DeleteFunc<TVertex>());
    m_vtVertices.clear();
  }
  
protected: // methods

  int get_edge_code(const TVertex* start_vertex_pt, const TVertex* end_vertex_pt) const
  {
    /// Note that the computation below works only if 
    /// the result is smaller than the maximum of an integer!
    return start_vertex_pt->getID()*m_nVertexNum+end_vertex_pt->getID();
  }

public: 

  double get_edge_weight(const TVertex* start_vertex_pt, const TVertex* end_vertex_pt) const
  {
    map<int, double>::const_iterator pos = 
      m_mpEdgeCodeWeight.find(get_edge_code(start_vertex_pt, end_vertex_pt));
    
    if (pos != m_mpEdgeCodeWeight.end())
    {
      return pos->second;
    }

    return DISCONNECT;
  }

  TVertex* get_vertex_by_ID(int id) const
  {
    return m_vtVertices.at(id);
  }

  TVertex* get_vertex(T node_)
  {
    TVertex* vertex_pt = NULL;
    const map<T, TVertex*>::iterator pos = m_mpVertexIndex.find(node_);
    if (pos == m_mpVertexIndex.end())
    {
      int vertex_id = m_vtVertices.size();
      vertex_pt = new TVertex(node_);
      vertex_pt->setID(vertex_id);
      m_vtVertices.push_back(vertex_pt);
      m_mpVertexIndex.insert(make_pair(node_, vertex_pt));
    }else
    {
      vertex_pt = m_vtVertices.at(pos->second->getID());
    }

    return vertex_pt;
  }

  set<TVertex*>* get_precedent_vertex_set(TVertex* vertex) 
  {
    return get_vertex_set_pt(vertex, m_mpFaninVertices);
  }

  set<TVertex*>* get_adjacent_vertex_set(TVertex* vertex) 
  {
    return get_vertex_set_pt(vertex, m_mpFanoutVertices);
  }

  set<TVertex*>* get_vertex_set_pt(TVertex* vertex_, 
    map<TVertex*, set<TVertex*>*>& vertex_container_index)
  {
    TVertexPt2SetMapIterator pos = vertex_container_index.find(vertex_);

    if(pos == vertex_container_index.end())
    {
      set<TVertex*>* vertex_set = new set<TVertex*>();
      pair<TVertexPt2SetMapIterator,bool> ins_pos = 
        vertex_container_index.insert(make_pair(vertex_, vertex_set));

      pos = ins_pos.first;
    }
  
    return pos->second;
  }

};

template<typename T>
const double TGraph<T>::DISCONNECT = (numeric_limits<double>::max)();

#endif
