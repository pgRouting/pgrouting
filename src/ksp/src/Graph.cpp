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
#include <cassert>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "GraphElements.h"
#include "Graph.h"

//#include "ksp.h"

const double Graph::DISCONNECT = (std::numeric_limits<double>::max)();

Graph::Graph( const std::string &file_name )
{
	_import_from_file(file_name);
}

Graph::Graph( const Graph &graph )
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


POS Graph::getNewVertex( int node_id )
{
   POS nodePos;
   if ( !exist_vertex(node_id)) {
       nodePos = m_vtVertices.size();
       BaseVertex vertex(nodePos,node_id);
       m_vtVertices.push_back(vertex);
       return nodePos;
    }
   nodePos = find_vertex(node_id);
   return nodePos;
}


void Graph::insertNewEdge(int id,  POS startPos, POS endPos, double edge_weight)
{
    //all edges are different regardless of sharing same start & end nodes
    POS edgePos = m_vtEdges.size();
    BaseEdge edge(edgePos,id,startPos,endPos,edge_weight);
    m_vtEdges.push_back(edge);

//    BaseEdge* edge_pt= &m_vtEdges[edgePos];
    m_vtVertices[startPos].push_FanOut(edgePos);
    m_vtVertices[endPos].push_FanIn(edgePos);
#if 0
std::cout<<"inserted edge\n";
m_vtEdges[edgePos].PrintOut(std::cout);
#endif
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



void Graph::get_adjacent_edges ( POS vertex_id, std::deque<POS> &edges_set ) const
{
#if 0
std::cout<<"*********Entering Graph::get_adjacent_edges\n";
#endif
        POS edgeId, nextNodeId;
#if 0
m_vtVertices[ vertex_id ].PrintOut(std::cout, m_vtEdges );
std::cout<<"\n";
#endif


        if (m_vtVertices[vertex_id].isActive())
        {
           std::deque<POS> FanOut = m_vtVertices[vertex_id].getFanOut();
#if 0
std::cout<<"vertex has "<<FanOut.size()<<"\n";
#endif
           for (POS i = 0; i< FanOut.size(); i++) {
               edgeId=FanOut[i];
#if 0
std::cout<<"Working with";
m_vtEdges[ edgeId ].PrintOut(std::cout);
#endif
               nextNodeId =  m_vtEdges[ edgeId ].getEnd();
#if 0
m_vtEdges[ edgeId ].PrintOut(std::cout);
std::cout<<"\n";
std::cout<<nextNodeId<<"\n";
#endif
               if ( m_vtVertices[ nextNodeId ].isActive()
                   && m_vtEdges[ edgeId ].isActive() )
                       edges_set.push_back( edgeId );
#if 0
std::cout<<edges_set.size()<<"edges found\n";
#endif

           }
        }
#if 0
std::cout<<"exiting Graph::get_adjacent_edges with "<<edges_set.size()<<"edges found\n";
#endif
}

void Graph::get_precedent_edges ( POS vertex_id, std::deque<BaseEdge*> &edges_set ) 
{
        //BaseEdge *edge;

        if (m_vtVertices[vertex_id].isActive())
        {
           std::deque<POS> FanIn = m_vtVertices[vertex_id].getFanIn();
           for (POS i=0; i<FanIn.size();i ++) {
               //edge = FanIn[i] ;
               if ( m_vtVertices[  m_vtEdges[ FanIn[i] ].getStart() ].isActive()
                   &&  m_vtEdges[ FanIn[i] ].isActive() )
                       edges_set.push_back( &m_vtEdges[ FanIn[i] ]);
           }
        }
}


void Graph::PrintOut(std::ostream &out_stream) const {
   out_stream << "Vertices \n";
   for (POS i = 0; i< m_vtVertices.size(); i++) {
       m_vtVertices[i].PrintOut(out_stream );
       out_stream << "\n";
   }
   out_stream << "Edges\n";
   for (POS i = 0; i < m_vtEdges.size(); i++) { 
       m_vtEdges[i].PrintOut(out_stream);
       out_stream << "\n";
   }
}



void Graph::PrintPath(std::ostream &out_stream, const BasePath &path, int startNode) const {
   if ( path.size()==0 ) {
       out_stream<< "Route 0\n";
       out_stream<< startNode << "\t-1 \t0\n";
       return;
   }
       
   for (POS i=0 ; i < path.size() ; i++) {
       BaseEdge edge = (*path[i]);
       int start = m_vtVertices[ edge.getStart() ].getID();
       double cost  = edge.Weight();
       int edgeId = edge.getID();
       out_stream<< "Route "<< i<< "\n";
       out_stream<< start << "\t"<<edgeId<<"\t"<<cost<<"\n";
   };
   out_stream<< m_vtVertices[ path[path.size()-1 ]->getEnd() ].getID() << "\t-1 \t0\n";
   
}   
        
///////////////////////////////////////////////////////////////////////////////
///  private  _import_from_file
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
void Graph::_import_from_file( const std::string &input_file_name )
{
	const char* file_name = input_file_name.c_str();

	//1. Check the validity of the file
	std::ifstream ifs(file_name);
	if (!ifs)
	{
		std::cerr << "The file " << file_name << " can not be opened!" << std::endl;
		exit(1);
	}

	//2. Reset the members of the class
	clear();

	//3. Start to read information from the input file. 
	//3.1 The first line indicates if it has a reverse cost
        bool has_reverse_cost;
	ifs >> has_reverse_cost;

	//3.2 In the following lines, each line contains a directed edge in the graph:
	///   id of the edge, the id of starting point, the id of ending point, the weight of the edge. 
	///   These values are separated by 'white space'. 
	int edge_id, start_id, end_id;
	double edge_weight,reverse_weight;
        POS startPos;
        POS endPos;

	while(ifs >> edge_id) {
	
		if (edge_id == -1)  break;

		ifs >> start_id;
		ifs >> end_id;
		ifs >> edge_weight;
		if ( has_reverse_cost ) ifs >> reverse_weight;

		///3.2.1 construct the vertices
                  /// look for existing vertex or create
                startPos = Graph::getNewVertex(start_id);
                endPos = Graph::getNewVertex(end_id);
                
		///3.2.2 add the edge 
                if ( edge_weight >0 ) 
	               insertNewEdge(edge_id, startPos, endPos, edge_weight);
                if ( has_reverse_cost && reverse_weight >0 ) 
	               insertNewEdge(edge_id, endPos, startPos, reverse_weight);
	}	

	ifs.close();	
}
