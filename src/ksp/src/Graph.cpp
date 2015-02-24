///////////////////////////////////////////////////////////////////////////////
///  Graph.cpp
///  <TODO: insert file description here>
///
///  @remarks <TODO: insert remarks here>
///
///  @author Yan Qi @date 8/18/2010
/// 
///  $Id: Graph.cpp 65 2010-09-08 06:48:36Z yan.qi.asu $
///////////////////////////////////////////////////////////////////////////////

#include <limits>
#include <set>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "GraphElements.h"
#include "Graph.h"
#include "DijkstraShortestPathAlg.h"

//#include "ksp.h"

const double Graph::DISCONNECT = (numeric_limits<double>::max)();

#if 0
Graph::Graph( const string& file_name )
{
	_import_from_file(file_name);
}
#endif

Graph::Graph( const Graph& graph )
{
	m_vtVertices.assign(graph.m_vtVertices.begin(),graph.m_vtVertices.end());
	m_vtEdges.assign(graph.m_vtEdges.begin(),graph.m_vtEdges.end());
}
Graph::Graph(void){
}

Graph::~Graph(void)
{
	clear();
}

BasePath Graph::Dijkstra( POS source_id, POS target_id) {
   BasePath shortestPath(source_id,target_id);
//TODO  
   return shortestPath;
}

POS Graph::getNewVertex( int node_id )
{
   POS nodePos = find_vertex(node_id);
   if (nodePos==0 && ( m_vtVertices.size()==0 || m_vtVertices[0].getNID()!=nodePos)) {
       nodePos = m_vtVertices.size();
       BaseVertex vertex(nodePos,node_id);
       m_vtVertices.push_back(vertex);
    }
    return nodePos;
}


void Graph::insertNewEdge(int id,  POS startPos, POS endPos, double edge_weight)
{
    POS edgePos=m_vtEdges.size();
    BaseEdge edge(edgePos,id,startPos,endPos,edge_weight);
    m_vtEdges.push_back(edge);

    BaseEdge* edge_pt= &m_vtEdges[edgePos];
    m_vtVertices[startPos].push_FanOut(edge_pt);
    m_vtVertices[endPos].push_FanIn(edge_pt);
}



BaseVertex Graph::GetVertex( int node_id )
{
        BaseVertex vertex_pt = m_vtVertices[node_id];
        return vertex_pt;
}


BaseVertex* Graph::get_vertex( int node_id )
{
        BaseVertex * vertex_pt = &m_vtVertices[node_id];
	if (vertex_pt->isRemoved() )
		return NULL;
	return vertex_pt;	
}

void Graph::clear()
{
	m_vtEdges.clear();
        for (POS pos=0; pos<m_vtVertices.size(); pos++)
             m_vtVertices[pos].clear();
	m_vtVertices.clear();
}



double Graph::get_edge_weight( const BaseEdge &edge) const
{
        BaseVertex source = m_vtVertices[edge.getStart()];
        BaseVertex sink = m_vtVertices[edge.getEnd()];

        if (   source.isRemoved()
            || sink.isRemoved()
            || edge.isRemoved() )
        {
                return DISCONNECT;
        } else {
                return edge.Weight();
        }
}

double Graph::get_edge_weight( int edge_id) const
{
        return get_edge_weight( m_vtEdges[edge_id] );
}



void Graph::get_adjacent_edges ( POS vertex_id, deque<BaseEdge*> &edges_set ) const
{
        BaseEdge *edge;

        if (m_vtVertices[vertex_id].isActive())
        {
           deque<BaseEdge *> FanOut = m_vtVertices[vertex_id].getFanOut();
           for (POS i=0; i< FanOut.size();i ++) {
               edge = FanOut[i];
               if ( m_vtVertices[ edge->getEnd() ].isActive()
                   && edge->isActive() )
                       edges_set.push_back(edge);
           }
        }
}

void Graph::get_precedent_edges ( POS vertex_id, deque<BaseEdge*> &edges_set ) const
{
        BaseEdge *edge;

        if (m_vtVertices[vertex_id].isActive())
        {
           deque<BaseEdge *> FanIn = m_vtVertices[vertex_id].getFanIn();
           for (POS i=0; i<FanIn.size();i ++) {
               edge = FanIn[i] ;
               if ( m_vtVertices[ edge->getStart() ].isActive()
                   && edge->isActive() )
                       edges_set.push_back(edge);
           }
        }
}


#if 0
///////////////////////////////////////////////////////////////////////////////
///  public  _import_from_file
///  Construct the graph by importing the edges from the input file. 
///
///  @param [in]       file_name const std::string &    The input graph file
///
///  This function doesn't return a value
///
///  @remarks The format of the file is as follows:
///   1. The first line has an integer as the number of vertices of the graph
///   2. Each line afterwards contains a directed edge in the graph:
///		     starting point, ending point and the weight of the edge. 
///		 These values are separated by 'white space'.
///
///  @see <TODO: insert text here>
///
///  @author Yan Qi @date 5/29/2010
///////////////////////////////////////////////////////////////////////////////
void Graph::_import_from_file( const string& input_file_name )
{
	const char* file_name = input_file_name.c_str();
        POS m_nVertexNum;

	//1. Check the validity of the file
	ifstream ifs(file_name);
	if (!ifs)
	{
		cerr << "The file " << file_name << " can not be opened!" << endl;
		exit(1);
	}

	//2. Reset the members of the class
	clear();

	//3. Start to read information from the input file. 
	/// Note the format of the data in the graph file.
	//3.1 The first line has an integer as the number of vertices of the graph
	ifs >> m_nVertexNum;

	//3.2 In the following lines, each line contains a directed edge in the graph:
	///   the id of starting point, the id of ending point, the weight of the edge. 
	///   These values are separated by 'white space'. 
	int start_id, end_id;
	double edge_weight;
        POS startPos;
        POS endPos;
        POS edgePos;

	while(ifs >> start_id)
	{
		if (start_id == -1)
		{
			break;
		}
		ifs >> end_id;
		ifs >> edge_weight;

		///3.2.1 construct the vertices
                  /// look for existing vertex or create
                startPos=find_vertex(start_id);
                if (startPos==0 && ( m_vtVertices.size()==0 ||   m_vtVertices[0].getNID()!=startPos)) {
                   startPos = m_vtVertices.size();
                   BaseVertex start_vertex(startPos,start_id);
                   m_vtVertices.push_back(start_vertex);
                }
                endPos=find_vertex(end_id);
                if (endPos==0 && m_vtVertices[0].getNID()!=endPos) {
                   endPos=m_vtVertices.size();
                   BaseVertex end_vertex(endPos,end_id);
                   m_vtVertices.push_back(end_vertex);
                }

//kspDBG("==============================>>>>>>>Start Vertex %ld end Vertex %ld\n",start_id,end_id);
		///3.2.2 add the edge weight
		//// note that the duplicate edge would overwrite the one occurring before. 
                /////  not anymore
                edgePos=m_vtEdges.size();
                BaseEdge edge(edgePos,startPos,endPos,edge_weight); 
                m_vtEdges.push_back(edge);
                BaseEdge* edge_pt= &m_vtEdges[edgePos];

		///3.2.3 update the fan-in or fan-out variables
		m_vtVertices[startPos].push_FanOut(edge_pt);
		m_vtVertices[endPos].push_FanIn(edge_pt);
	}	

	if(m_nVertexNum != m_vtVertices.size())
	{
		cerr << "The number of nodes in the graph is "<<  m_vtVertices.size() << " instead of " << m_nVertexNum << endl;
		exit(1);
	}

	//m_nVertexNum = m_vtVertices.size();
	//m_nEdgeNum = m_mpEdgeCodeWeight.size();

	ifs.close();	
}
#endif
