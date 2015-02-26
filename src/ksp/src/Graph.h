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
#ifndef GRAPH_H
#define GRAPH_H


//using namespace std;
#if 0
class Path : public BasePath
{
public: 
	Path(const std::deque<BaseEdge*>& edges_list, double weight):BasePath(edges_list,weight){}
};
#endif
class Graph
{
public: // members

	const static double DISCONNECT; 
	//typedef set<BaseEdge*>::iterator EdgesPtSetIterator;
	//typedef std::deque< deque<BaseEdge*> > BaseVertexMap;

protected: // members

        std::deque<BaseVertex> m_vtVertices;
        std::deque<BaseEdge> m_vtEdges;
public:

	// Constructors and Destructor
	Graph(const std::string &file_name);
	Graph(const Graph& rGraph);
	Graph();
	~Graph(void);

	void clear();

        //functions used to insert the data from postgres into the graph
        void insertNewEdge( int id, POS startPos, POS endPos, double edge_weight);
        POS getNewVertex( int node_id );
         
	BaseVertex GetVertex(int node_id);
	BaseVertex* get_vertex(int node_id);
	double NodeWeight(POS node_id) const { return m_vtVertices[node_id].Weight(); };
	double EdgeWeight(POS edge_id) const { return m_vtEdges[edge_id].Weight(); };
        BasePath Dijkstra( POS source_id, POS target_id);
	
	int get_edge_code(const BaseVertex* start_vertex_pt, const BaseVertex* end_vertex_pt) const;
	//set<BaseVertex*>* get_vertex_set_pt(BaseVertex* vertex_, map<BaseVertex*, set<BaseVertex*>*>& vertex_container_index);

	double get_original_edge_weight(const BaseVertex* source, const BaseVertex* sink);

        void insertEdge(int source, int target, double weight);

	double get_edge_weight(const BaseEdge &edge) const; 
	double get_edge_weight(int edge_id) const;

        void  get_adjacent_edges ( POS vertex_id,std::deque<POS> &edges_set ) const;
        void  get_precedent_edges ( POS vertex_id, std::deque<BaseEdge*> &edges_set );

        POS find_vertex (int vertex_id) const {
            POS i;
            if (m_vtVertices.size() ==0) return 0;
	    for ( i=0; i<m_vtVertices.size() && m_vtVertices[i].getID()!=vertex_id; i++) {};
            if ( i>=m_vtVertices.size()) return 0;
            else return i;
        }
        
        POS exist_vertex (int vertex_id) const {
            POS i;
            if (m_vtVertices.size() ==0) return false;
	    for ( i=0; i<m_vtVertices.size() && m_vtVertices[i].getID()!=vertex_id; i++) {};
            if ( i>=m_vtVertices.size()) return false;
            else return true;
        }
        bool checkVertexIDs( POS nodeNID, int nodeID) {
            if ( !(nodeNID < m_vtVertices.size()) ) return false;
            if ( m_vtVertices[nodeNID].getID()  != nodeID ) return false;
            return true;
        }

	void remove_edge(POS edge_id)
	{
		m_vtEdges[edge_id].remove();
        }

        void removeNodes ( const BasePath &path ){
#if 0          
std::cout<<"about to remove the nodes\n";
#endif

            if ( path.size() == 0) return;
            for (POS i = 0 ; i < path.size() ; i++){
               m_vtVertices[ path[i]->getStart() ].remove();
            } 
#if 0
std::cout<<"finished remove the nodes\n";
#endif         

            //m_vtVertices[ path[ path.size()-1  ]->getEnd() ].remove();
        }

        void PrintOut(std::ostream &out_stream) const;
        void PrintPath(std::ostream &out_stream, const BasePath &path, int start) const;
        private:
	void _import_from_file(const std::string &file_name);

};

#endif
