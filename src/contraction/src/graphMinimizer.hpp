#ifndef GRAPH_MINIMIZER_H_
#define GRAPH_MINIMIZER_H_

/*
Edge types:
0: Normal edge
1: Updated Edge
2: New Edge
*/
#include <deque>
#include <vector>
#include <set>
#include <stack>
#include <map>
#include <iostream>
#include <sstream>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include "./baseGraph.hpp"

#include "./structs.h"
using namespace std;
using namespace boost;
namespace patch
{
    template < typename T > std::string to_string( const T& n )
	{
		std::ostringstream stm ;
		stm << n ;
		return stm.str() ;
	}
}
template < class G >
class Graph_Minimizer
:public  Pgr_base_graph <G> {
public:
	Pgr_base_graph<G> *reduced_graph;
  //constrictor for this class which inherits the base graph
	explicit Graph_Minimizer(graphType gtype, const int initial_size)
	: Pgr_base_graph<G>(gtype, initial_size) {
		reduced_graph=new  Pgr_base_graph<G>(gtype,initial_size);
	}
	typedef typename boost::graph_traits < G >::vertex_descriptor V;
	typedef typename boost::graph_traits < G >::edge_descriptor E;
	typedef typename boost::graph_traits < G >::vertex_iterator V_i;

  //typedef typename std::map<V,E> removed_edges;
	typedef typename std::map<V,deque<Edge> > removed_V;
	typedef  std::deque<Edge>::iterator removed_E_i;
	typedef typename std::map< int64_t, std::vector<V> > degree_to_V;
	typedef typename std::map< int64_t, std::vector<V> >::iterator degree_to_V_i;
	typedef typename std::vector<V>::iterator Q_i;
	typedef typename boost::graph_traits < G >::out_edge_iterator EO_i;
	typedef typename std::map<V,deque<Edge> >::iterator removed_V_i;
	typedef typename std::map<int64_t , pair<int64_t,int64_t> > psuedo_E;
	typedef typename std::map<int64_t , pair<int64_t,int64_t> >::iterator psuedo_E_i;
	int64_t last_edge_id;
	typedef deque<Edge> unpackedPath;
	unpackedPath source_un_p,target_un_p;
	removed_V removedVertices;
	psuedo_E psuedoEdges;

  //initializes the graph with the given edges
	void
	initialize_graph_minimizer(Edge *data_edges, int64_t count) {
		// /cout << "Initializing......." << endl;
		this->graph_insert_data(data_edges, count);
		last_edge_id=count;
		reduced_graph->graph_insert_data(data_edges,count);
	}


	void remove_1_degree_vertices()
	{
		EO_i out,out_end;
		V front=reduced_graph->degree_to_V_map[1].front();
		int64_t frontid=-1;
		reduced_graph->get_vertex_id(front,frontid);
		std::vector<V> one_degree_vertices=reduced_graph->degree_to_V_map[1];
		degree_to_V_i it;
		cout << "1 degree vertices " << one_degree_vertices.size()  << endl;
		while (reduced_graph->degree_to_V_map[1].size()>0)
		{
			cout << "Front " << reduced_graph->graph[front].id << endl;
			for (boost::tie(out, out_end) = out_edges(front, reduced_graph->graph);
				out != out_end; ++out) 
			{
				V s=source(*out, reduced_graph->graph);
				V t=target(*out, reduced_graph->graph);
				int source_id=reduced_graph->graph[s].id;
				//int target_id=reduced_graph->graph[t].id;
				
				//cout << "Removing " << source_id  << endl;
				//cout << "Neighbor" << target_id << endl;
				reduced_graph->graph[t].contractions++;
				int prev_target_degree=reduced_graph->graph[t].degree;
				reduced_graph->graph[t].degree--;
				int final_target_degree=prev_target_degree-1;
				reduced_graph->degree_to_V_map[final_target_degree].push_back(t);
				Edge removed_edge=reduced_graph->graph[*out];
				//cout << "removing" << " (" << removed_edge.id << ", " <<  removed_edge.source<< ", " << removed_edge.target << 
				//	", " << removed_edge.cost << ", " << removed_edge.reverse_cost <<")" << endl;
				removedVertices[frontid].push_front(removed_edge);
				reduced_graph->remove_vertex(source_id);
				reduced_graph->numb_vertices--;
			}
			reduced_graph->degree_to_V_map[1].erase(reduced_graph->degree_to_V_map[1].begin(),
				reduced_graph->degree_to_V_map[1].begin()+1);
			
			front=reduced_graph->degree_to_V_map[1].front();
			reduced_graph->get_vertex_id(front,frontid);
		}

	}


	void remove_2_degree_vertices()
	{
		EO_i out,out_end;
		std::vector<V> neighbors_desc;
		std::vector<V> two_degree_vertices=reduced_graph->degree_to_V_map[2];
		std::vector<V> two_degree_vertices_0;
		for (Q_i it = two_degree_vertices.begin() ; it != two_degree_vertices.end(); ++it)
		{
			//cout << "id" << reduced_graph->graph[*it].id << endl;
			//cout << "contractions" << reduced_graph->graph[*it].contractions << endl;
			if (reduced_graph->graph[*it].contractions==0)
			{
				
				
				two_degree_vertices_0.push_back(*it);
			}
		}
		degree_to_V_i it;
		cout << "2 degree vertices " << two_degree_vertices_0.size()  << endl;
		if (two_degree_vertices_0.size()==0)
		{
			return;
		}
		
		V front=two_degree_vertices_0.front();
		int64_t front_id=-1;
		while (two_degree_vertices_0.size()>0)
		{
			//cout << "2 degree vertices " << two_degree_vertices_0.size()  << endl;
			//cout << "Edge count" << num_edges(reduced_graph->graph) << endl;
			cout << "Front " << reduced_graph->graph[front].id << endl;
			neighbors_desc.clear();
			front=two_degree_vertices_0.front();
			reduced_graph->get_vertex_id(front,front_id);
			if (reduced_graph->graph[front].contractions==0)
			{
				/* code */
				
				int count=0;
				int64_t tid1=-1,tid2=-1;
				V  tdesc1=-1,tdesc2=-1;
				float min_distance=0.00000;
				for (boost::tie(out, out_end) = out_edges(front, reduced_graph->graph);
					out != out_end; ++out) 
				{
					V t=target(*out, reduced_graph->graph);
					min_distance+=reduced_graph->graph[*out].cost;
					reduced_graph->graph[t].contractions++;
					neighbors_desc.push_back(t);
				//removedVertices[t].push(front);
					count++;
				}
				if (count==2)
				{
				/* code */
					Edge shortcut;
					tdesc1=neighbors_desc[0];
					tdesc2=neighbors_desc[1];
					reduced_graph->get_vertex_id(tdesc1,tid1);
					reduced_graph->get_vertex_id(tdesc2,tid2);
					E edesc1=edge(tdesc1, front,reduced_graph->graph).first;
					E edesc2=edge(front, tdesc2,reduced_graph->graph).first;
					int64_t eid1=reduced_graph->graph[edesc1].id;
					int64_t eid2=reduced_graph->graph[edesc2].id;
					int64_t eid;
					bool b1=edge(tdesc1, tdesc2,reduced_graph->graph).second;
					bool b2=edge(tdesc2, tdesc1,reduced_graph->graph).second;
					//cout << b1 << "," << b2 << endl;
					if ( b1 || b2)
					{
						E edesc=edge(tdesc1, tdesc2,reduced_graph->graph).first;
						eid=reduced_graph->graph[edesc].id;
						float dist=reduced_graph->graph[edesc].cost;
						//cout  << "min dist " << min_distance << ", actual dist " << dist << endl; 
						if (min_distance-dist>=0.000001)
						{
						//cout << "yes" << endl;
							min_distance=dist;
						}
						else
						{
							cout << "Updating shortcut " << "(" << eid1 << ", " << eid2 << ")" << " with " << min_distance << endl;
							reduced_graph->graph[edesc].cost=min_distance;
							reduced_graph->graph[edesc].type=1;
						}
					}
					else
					{
						//cout << "no" << endl;
						shortcut.id=++last_edge_id;
						eid=shortcut.id;
						shortcut.source=tid1;
						shortcut.target=tid2; 
						shortcut.cost=min_distance;
						shortcut.type=2;
						reduced_graph->graph_add_edge(shortcut);
					}
					psuedoEdges[eid]=make_pair(eid1,eid2);
					Edge removed_edge;
					for (int i = 0; i < 2; ++i)
					{
						V t=neighbors_desc[i];
						removed_edge=reduced_graph->graph[edge(front, t,reduced_graph->graph).first];
						removedVertices[front_id].push_front(removed_edge);
					}
					reduced_graph->remove_vertex(front_id);
					reduced_graph->numb_vertices--;
				}
			}
			two_degree_vertices_0.erase(two_degree_vertices_0.begin(),
				two_degree_vertices_0.begin()+1);
			
		}
	}



	void remove_3_degree_vertices()
	{
		EO_i out,out_end;
		std::vector<V> three_degree_vertices=reduced_graph->degree_to_V_map[3];
		std::vector<V> three_degree_vertices_0;
		std::vector<V> neighbors_desc;
		for (Q_i it = three_degree_vertices.begin() ; it != three_degree_vertices.end(); ++it)
		{
			if (reduced_graph->graph[*it].contractions==0)
			{
				//cout << reduced_graph->graph[*it].id << endl;
				three_degree_vertices_0.push_back(*it);

			}
		}
		degree_to_V_i it;
		if (three_degree_vertices_0.size()==0)
		{
			return;
		}
		cout << "3 degree vertices " << three_degree_vertices_0.size()  << endl;
		V front=three_degree_vertices_0.front();
		int64_t front_id=-1;
		while (three_degree_vertices_0.size()>0)
		{
			//cout << "2 degree vertices " << two_degree_vertices_0.size()  << endl;
			//cout << "Edge count" << num_edges(reduced_graph->graph) << endl;
			front=three_degree_vertices_0.front();
			//cout << "Front " << reduced_graph->graph[front].id << endl;
			neighbors_desc.clear();
			reduced_graph->get_vertex_id(front,front_id);
			int count=0;
			int64_t tid1=-1,tid2=-1;
			V  tdesc1=-1,tdesc2=-1;
			float min_distance=0.00000;
			for (boost::tie(out, out_end) = out_edges(front, reduced_graph->graph);
				out != out_end; ++out) 
			{
				V t=target(*out, reduced_graph->graph);
				reduced_graph->graph[t].contractions++;
				neighbors_desc.push_back(t);
				count++;
			}
			if (count==3)
			{
				/* code */
				for (int i=0;i<3; i++)
				{
					tdesc1=neighbors_desc[i];
				//cout << "tdesc1 " << reduced_graph->graph[tdesc1].id << endl;
					for (int j = i+1; j < 3; ++j)
					{
						tdesc2=neighbors_desc[j];
					//cout << "tdesc2 " << reduced_graph->graph[tdesc2].id << endl;
					//cout << tdesc1 << "," << tdesc2 << endl;
						Edge shortcut;
						E edesc1=edge(front, tdesc1,reduced_graph->graph).first;
						E edesc2=edge(front, tdesc2,reduced_graph->graph).first;
						float dist2=reduced_graph->graph[edesc2].cost;
						float dist1=reduced_graph->graph[edesc1].cost;
						int64_t eid1=reduced_graph->graph[edesc1].id;
						int64_t eid2=reduced_graph->graph[edesc2].id;
						int64_t eid;
						min_distance=dist1+dist2;
						bool b1=edge(tdesc1, tdesc2,reduced_graph->graph).second;
						bool b2=edge(tdesc2, tdesc1,reduced_graph->graph).second;
						if ( b1 || b2)
						{
							E edesc=edge(tdesc1, tdesc2,reduced_graph->graph).first;
							float dist=reduced_graph->graph[edesc].cost;
							if (fabs(min_distance-dist)>=0.000001)
							{
								min_distance=dist;
							}
							else
							{
								reduced_graph->graph[edesc].cost=min_distance;
								reduced_graph->graph[edesc].type=1;
								eid=reduced_graph->graph[edesc].id;
							}
						}
						else
						{
							reduced_graph->get_vertex_id(tdesc1,tid1);
							reduced_graph->get_vertex_id(tdesc2,tid2);
							shortcut.id=++last_edge_id;
							eid=shortcut.id;
							shortcut.source=tid1;
							shortcut.target=tid2;
							shortcut.cost=min_distance;
							shortcut.type=2;
							reduced_graph->graph_add_edge(shortcut);
						}
						psuedoEdges[eid]=make_pair(eid1,eid2);
					}
				}
				Edge removed_edge;
				for (int i = 0; i < 3; ++i)
				{
					V t=neighbors_desc[i];
					removed_edge=reduced_graph->graph[edge(front, t,reduced_graph->graph).first];
					removedVertices[front].push_front(removed_edge);
				}
				reduced_graph->remove_vertex(front_id);
			}
			three_degree_vertices_0.erase(three_degree_vertices_0.begin(),
				three_degree_vertices_0.begin()+1);
		}
	}

	int getreducedGraph(Edge **reduced_list)
	{
		int reduced_size=(int)num_edges(reduced_graph->graph);
		if ((*reduced_list)==NULL)
			(*reduced_list)=(Edge *)malloc(sizeof(Edge)*reduced_size);
		else
			(*reduced_list)=(Edge *)realloc((*reduced_list),sizeof(Edge)*reduced_size);
		V_i vi;
		EO_i out,out_end;
		int count=0;
		for (vi = vertices(reduced_graph->graph).first; vi != vertices(reduced_graph->graph).second; ++vi) {
			for (boost::tie(out, out_end) = out_edges(*vi, reduced_graph->graph);
				out != out_end; ++out)
			{
				V source_desc=source(*out, reduced_graph->graph);
				V target_desc=target(*out, reduced_graph->graph);
				int source_id=reduced_graph->graph[source_desc].id;
				int target_id=reduced_graph->graph[target_desc].id;
				
				if (target_desc>source_desc)
				{
					(*reduced_list)[count].id=reduced_graph->graph[*out].id;
					(*reduced_list)[count].source=source_id;
					(*reduced_list)[count].target=target_id;
					(*reduced_list)[count].cost=reduced_graph->graph[*out].cost;
					(*reduced_list)[count].type=reduced_graph->graph[*out].type;
					count++;
				}
				
			}
		}
		return count;
	}

	int64_t getreducedGraph_string(ostringstream& estring)
	{
		//int reduced_size=(int)num_edges(reduced_graph->graph);
		V_i vi;
		EO_i out,out_end;
		int64_t count=0;
		for (vi = vertices(reduced_graph->graph).first; vi != vertices(reduced_graph->graph).second; ++vi) {
			for (boost::tie(out, out_end) = out_edges(*vi, reduced_graph->graph);
				out != out_end; ++out)
			{
				V source_desc=source(*out, reduced_graph->graph);
				V target_desc=target(*out, reduced_graph->graph);
				int64_t source_id=reduced_graph->graph[source_desc].id;
				int64_t target_id=reduced_graph->graph[target_desc].id;
				
				if (target_desc>source_desc)
				{
					estring << reduced_graph->graph[*out].id << ","
					<< source_id << "," << target_id << ","
					<< reduced_graph->graph[*out].cost << ","
					<< reduced_graph->graph[*out].reverse_cost << "$";
					/*string id=patch::to_string(reduced_graph->graph[*out].id);
					string source=patch::to_string(source_id);
					string target=patch::to_string(target_id);
					string cost=patch::to_string(reduced_graph->graph[*out].cost);
					string reverse_cost=patch::to_string(reduced_graph->graph[*out].reverse_cost);
					restring+=id+","+source+","+target+","+cost+","+reverse_cost+"$";*/
					count++;
				}
				
			}
		}
		return count;
	}


	void contract_to_level(int level)
	{
		switch(level)
		{
			case 0 :
			this->remove_1_degree_vertices(); 
			break;
			case 1 :
			this->remove_1_degree_vertices();
			this->remove_2_degree_vertices(); 
			break;
			case 2:
			this->remove_1_degree_vertices();
			this->remove_2_degree_vertices();
			this->remove_3_degree_vertices();
			default :
		//	do nothing;
			break;
		}
	}


	void print_removed_vertices()
	{
		cout << "Printing removed vertices ......" << endl;
		for (removed_V_i iter = removedVertices.begin(); iter != removedVertices.end(); iter++)
		{
			cout << "id: " << iter->first << endl;

			for (removed_E_i edge_iter = iter->second.begin(); edge_iter != iter->second.end(); edge_iter++)
			{
				Edge temp=*edge_iter ;
				cout << "(" << temp.source<< ", " << temp.target << "), ";
			}
			cout << endl;
		}
	}

	

	void print_psuedo_edges()
	{
		cout << "Printing psuedo edges ......" << endl;
		for (psuedo_E_i iter = psuedoEdges.begin(); iter != psuedoEdges.end(); iter++)
		{
			cout << "e1: " << iter->second.first << ",e: "<< iter->first << ",e2: " << iter->second.second << endl;
		}
	}
	void print_removed_edges()
	{
		cout<< "Printing removed_edges of size " << this->reduced_graph->removed_edges.size() << endl;
		for (map<int64_t,Edge>::iterator iter = this->reduced_graph->removed_edges.begin(); iter != this->reduced_graph->removed_edges.end(); iter++)
		{
			cout << "id: " << iter->first << endl;
			Edge temp=iter->second ;
			cout << "(" << temp.source<< ", " << temp.target << "), ";
			cout << endl;
		}
	}

	void find_source_vertex(int64_t origId,int64_t &compId,unpackedPath &unpack)
	{
		//reduced_graph->get_vertex_descriptor(compId,comp);
		int64_t tempid=origId;
		while(!(removedVertices.find(tempid) == removedVertices.end()))
		{
			Edge first=removedVertices[tempid].front();
			if (first.source==tempid)
			{
				//cout << "source" << endl;
				tempid=first.target;
				//reduced_graph->get_vertex_descriptor(first.target,temp);
				unpack.push_back(first);
			}
			else if (first.target==tempid)
			{
				//cout << "target" << endl;
				tempid=first.source;
				//reduced_graph->get_vertex_descriptor(first.source,temp);
				unpack.push_back(first);
			}

		}
		//reduced_graph->get_vertex_id(temp,compId);
		compId=tempid;
		return;
	}
	void find_target_vertex(int64_t origId,int64_t &compId,unpackedPath &unpack)
	{
		int64_t tempid=origId;
		while(!(removedVertices.find(tempid) == removedVertices.end()))
		{
			//cout << "initial temp " << tempid << endl;
			Edge first=removedVertices[tempid].front();
			if (first.source==tempid)
			{
				//cout << "source" << endl;
				tempid=first.target;
				//reduced_graph->get_vertex_descriptor(first.target,temp);
				unpack.push_front(first);
			}
			else if (first.target==tempid)
			{
				//cout << "target" << endl;
				tempid=first.source;
				//reduced_graph->get_vertex_descriptor(first.source,temp);
				unpack.push_front(first);
			}
			//cout << "final temp " << tempid << endl;
		}
		//reduced_graph->get_vertex_id(temp,compId);
		compId=tempid;
		return;
	}


	

	void print_path(Edge **path,int64_t size)
	{
		cout << "Path for dijkstra......" << endl; 
		for (int i = 0; i < size; ++i)
		{
			cout << "id:- " << (*path)[i].id << " src:- " << (*path)[i].source << " dest:- " << (*path)[i].target << " cost " << (*path)[i].cost << " type " << (*path)[i].type << endl;
		}
	}


	void getGraphName(ostringstream& name,int level)
	{
		//string gname="contracted_graph_"+patch::to_string(level);
		//(*name)=strdup(gname.c_str());
		name << "contracted_graph_" << level;
	}
	void getRemovedE_string(ostringstream& estring)
	{
		//string rmstring="";
		//cout<< "Printing removed_edges of size " << this->reduced_graph->removed_edges.size() << endl;
		for (map<int64_t,Edge>::iterator iter = this->reduced_graph->removed_edges.begin(); iter != this->reduced_graph->removed_edges.end(); iter++)
		{
			//cout << "id: " << iter->first << endl;
			Edge temp=iter->second ;
			estring << temp.id << "," << temp.source << ","
			<< temp.target << "," << temp.cost << ","
			<< temp.reverse_cost << "$";
			/*string id=patch::to_string(temp.id);
			string source=patch::to_string(temp.source);
			string target=patch::to_string(temp.target);
			string cost=patch::to_string(temp.cost);
			string reverse_cost=patch::to_string(temp.reverse_cost);
			rmstring+=id+","+source+","+target+","+cost+","+reverse_cost+"$";
			cout << endl;*/
		}
	}
	void getEdgeString(Edge **edges,int num_edges,ostringstream& estring)
	{
		//string edgestring="";
		for (Edge edge:*edges)
		{
			estring << edge.id << ","
			<< edge.source << "," << edge.target << ","
			<< edge.cost << "," << edge.reverse_cost << "$";
			/*string id=patch::to_string(edge.id);
			string source=patch::to_string(edge.source);
			string target=patch::to_string(edge.target);
			string cost=patch::to_string(edge.cost);
			string reverse_cost=patch::to_string(edge.reverse_cost);
			edgestring+=id+","+source+","+target+","+cost+","+reverse_cost+"$";*/
		}
		/*for (int i = 0; i < num_edges; ++i)
		{
			string id=patch::to_string((*edges)[i].id);
			string source=patch::to_string((*edges)[i].source);
			string target=patch::to_string((*edges)[i].target);
			string cost=patch::to_string((*edges)[i].cost);
			string reverse_cost=patch::to_string((*edges)[i].reverse_cost);
			edgestring+=id+","+source+","+target+","+cost+","+reverse_cost+"$";
		}*/
		//(*estring)=strdup(edgestring.c_str());
	}


	void getRemovedV_string(ostringstream& vstring)
	{
		string vertex_string="";
		for (removed_V_i iter = removedVertices.begin(); iter != removedVertices.end(); iter++)
		{
			//cout << "id: " << iter->first << endl;
			string vid=patch::to_string(iter->first);
			for (removed_E_i edge_iter = iter->second.begin(); edge_iter != iter->second.end(); edge_iter++)
			{
				Edge temp=*edge_iter ;
				vstring << vid <<","<< temp.id << ","
				<< temp.source << "," << temp.target << ","
				<< temp.cost << "," << temp.reverse_cost << "$";
				//cout << "(" << temp.source<< ", " << temp.target << "), ";
				/*string eid=patch::to_string(temp.id);
				string source=patch::to_string(temp.source);
				string target=patch::to_string(temp.target);
				string cost=patch::to_string(temp.cost);
				string reverse_cost=patch::to_string(temp.reverse_cost);
				vertex_string+=vid+","+eid+","+source+","+target+","+cost+","+reverse_cost+"$";*/
			}
			cout << endl;
		}		
		//(*vstring)=strdup(vertex_string.c_str());

	}

	void getPsuedoE_string(ostringstream& pstring)
	{
		//string ps_string="";
		for (psuedo_E_i iter = psuedoEdges.begin(); iter != psuedoEdges.end(); iter++)
		{
			pstring << iter->first << "," << iter->second.first << ","
			<< iter->second.second << "$";
			//cout << "e1: " << iter->second.first << ",e: "<< iter->first << ",e2: " << iter->second.second << endl;
			/*string eid=patch::to_string(iter->first);
			string eid1=patch::to_string(iter->second.first);
			string eid2=patch::to_string(iter->second.second);
			ps_string+="$"+eid+","+eid1+","+eid2;*/
		}
		//(*pstring)=strdup(ps_string.c_str());
	}
};
#endif