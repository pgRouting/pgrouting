///////////////////////////////////////////////////////////////////////////////
///  Graph.h
///   < TODO: insert file description here>
///
///  @remarks  < TODO: insert remarks here>
///
///  @author Yan Qi @date 8/18/2010
///
///  $Id: Graph.h 65 2010-09-08 06:48:36Z yan.qi.asu $
///////////////////////////////////////////////////////////////////////////////
#ifndef SRC_KSP_SRC_GRAPH_H_
#define SRC_KSP_SRC_GRAPH_H_


#include <deque>
#include <string>

class Graph {
 public:
     const static double DISCONNECT;

 protected:
     std::deque < BaseVertex> m_vtVertices;
     std::deque < BaseEdge> m_vtEdges;

 public:
     Graph();
     explicit Graph(const Graph& rGraph);
     explicit Graph(const std::string &file_name);
     ~Graph(void);

     void clear();

     // functions used to insert the data from postgres into the graph
     void insertNewEdge(int id, POS startPos, POS endPos, double edge_weight);
     POS getNewVertex(int node_id);

     BaseVertex GetVertex(int node_id);
     BaseVertex* get_vertex(int node_id);
     double NodeWeight(POS node_id) const { return m_vtVertices[node_id].Weight();}
     double EdgeWeight(POS edge_id) const { return m_vtEdges[edge_id].Weight();}

     void insertEdge(int source, int target, double weight);

     void get_adjacent_edges(POS vertex_id, std::deque< POS> &edges_set) const;
     void get_precedent_edges(POS vertex_id, std::deque< BaseEdge> &edges_set);

     POS find_vertex(int vertex_id) const;
     POS exist_vertex(int vertex_id) const;
     int getVertexOriginalID(POS vertex_id) const {return m_vtVertices[vertex_id].getNID();}
     bool checkVertexIDs(POS nodeNID, int nodeID);

     void remove_edge(POS edge_id);
     void removeNodes(const BasePath &path);

     void PrintOut(std::ostream &out_stream) const;
     void PrintPath(std::ostream &out_stream, const BasePath &path, int start) const;

 private:
     void _import_from_file(const std::string &file_name);
};

#endif  // SRC_KSP_SRC_GRAPH_H_
