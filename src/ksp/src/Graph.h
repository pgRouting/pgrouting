#ifndef SRC_KSP_SRC_GRAPH_H_
#define SRC_KSP_SRC_GRAPH_H_


#include <set>
#include <deque>
#include <string>
#include "GraphElements.h"

/**************************************************************************/
//!  Graph(Vertices,Edges)
/*!
\author Dave Potts
\author Vicky Vergara
\date Feb/2015
\copyright GNU General Public License, version 2
\details  Class to store a Graph(V,E)
A graph is defined as a set of Vertices and a set of Edges
Vertices and Edges are stored in this implementation with deque
********************************************************************** */
class Graph {
 public:
     typedef std::deque<BaseVertex> vList;
     typedef std::deque<BaseVertex>::iterator vListIt;
     typedef std::set<BaseVertex*, BaseVertex::compBaseVertexID> vSetPt;
     typedef std::set<BaseVertex*, BaseVertex::compBaseVertexID>::iterator vSetIt;

     typedef std::deque<BaseEdge> eList;
     typedef std::deque<BaseEdge>::iterator eListIt;

 protected:
     vList m_Vertices;    //<! Vertices
     vSetPt m_VerticesPt; //<! Vertices pointers ordered by original Id
     eList m_Edges;       //<! Edges
     std::set<BaseEdge*, BaseEdge::compBestEdge> m_BestEdgesPt;  //<! set of pointers to best edges between 2 nodes

 public:
     Graph();
     explicit Graph(const std::string &file_name);

     /** @name accessors */
     ///@{

     //! Returns the original ID of vertex_id
     long getVertexOriginalID(UINT vertex_id) const;
     //! Returns a pointer to the vertex originaly identified by vertex_id (NULL when nor found);
     BaseVertex* find_vertex(long vertex_id) const;
     ///@}

     /** @name mutators */
     ///@{

     //! clears the edges and vertices of the graph
     void clear();
     //! Inserts an edge to the graph (original id, startId, endId, weight)
     BaseEdge* insertNewEdge(long id, UINT startId, UINT endId, double edge_weight);
     //! Returns the Id a the newly created vertex or the Id if it already exists
     BaseVertex* getNewVertex(long vertex_id);
     ///@}

     /** @name debugging */
     ///@{
     void PrintOut(std::ostream &out_stream) const;
     void PrintPath(std::ostream &out_stream, const BasePath &path, long start) const;
     ///@}

 private:
     void import_from_file(const std::string &file_name);
     void updateBestEdgesSet(BaseEdge *edgePt);
};

#endif  // SRC_KSP_SRC_GRAPH_H_
