#ifndef SRC_KSP_SRC_GRAPH_H_
#define SRC_KSP_SRC_GRAPH_H_


#include <deque>
#include <string>

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
     const static double DISCONNECT;

 protected:
     std::deque <BaseVertex> m_Vertices; //<! Vertices
     std::deque <BaseEdge> m_Edges; //<! Edges

 public:
     Graph();
     explicit Graph(const Graph &rGraph);
     explicit Graph(const std::string &file_name);
     ~Graph(void);

     /** @name accessors */
     ///@{

     //! Edges set returns a deque containing the Ids of the fanOut edges of vertex_id;
     void getFanOutActiveEdgesIds(UINT vertex_id, std::deque< UINT> &edges_set) const;
     //! Edges set returns a deque containing a copy of the fanOut edges of vertex_id;
     void getFanOutActiveEdges(UINT vertex_id, std::deque< BaseEdge> &edges_set) const;
     //! Edges set returns a deque containing a copy of the fanIn edges of vertex_id;
     void getFanInActiveEdges(UINT vertex_id, std::deque< BaseEdge> &edges_set) const;
     //! Returns the weight of vertex_id
     double vertexWeight(UINT vertex_id) const;
     //! Returns the weight of the edge identified by edge_id
     double edgeWeight(UINT edge_id) const;
     //! Returns the a copy of the vertex identified by vertex_id
     BaseVertex getVertex(UINT vertex_id) const;
     //! Returns the original ID of vertex_id
     int getVertexOriginalID(UINT vertex_id) const;
     //! Returns the Id of the vertex originaly identified by vertex_id (0 when nor found);
     UINT find_vertex(int vertex_id) const;
     //! True when the identified originaly by vertex_id is in the graph
     UINT exist_vertex(int vertex_id) const;
     //! Returns a pointer to vertex_id
     BaseVertex* getVertexPt(UINT vertex_id) ;
     ///@}

     /** @name mutators */
     ///@{

     //! clears the edges and vertices of the graph
     void clear();
     //! Inserts an edge to the graph (original id, startId, endId, weight)
     UINT insertNewEdge(int id, UINT startId, UINT endId, double edge_weight);
     //! Returns the Id a the newly created vertex or the Id if it already exists
     UINT getNewVertex(int vertex_id);
     //! logically removes the edge from the graph
     void remove_edge(UINT edge_id);
     //! logically removes the vertices from the graph that are starting vertices of the edges in path
     void removeVertices(const BasePath &path);
     ///@}

     /** @name debugging */
     ///@{
     void PrintOut(std::ostream &out_stream) const;
     void PrintPath(std::ostream &out_stream, const BasePath &path, int start) const;
     ///@}

 private:
     void import_from_file(const std::string &file_name);
};

#endif  // SRC_KSP_SRC_GRAPH_H_
