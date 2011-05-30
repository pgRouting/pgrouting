#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <utility>
#include <queue>

#include <binary_heap.h>
#include <weight_map.h>
#include <edge_wrapper.h>

#include <boost/config.hpp>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#define DEBUG 0

using namespace std;
using namespace boost;

//------------------------------------------------------------------------

//Boost graph data structure types
typedef boost::adjacency_list < listS, vecS, directedS, no_property, Vertex> graph_t;
typedef boost::graph_traits < graph_t >::vertex_descriptor vertex_desc_t;
typedef boost::graph_traits < graph_t >::edge_descriptor edge_desc_t;

//To map the vertex ids to their current distances and predecessors
typedef map<int , double> distance_map_t;
typedef map<int , int>   predecessor_map_t;



/**
 * g - Input graph, adjecency list type
 * s - Source vertex
 * d_m - Distance Map. Initialized to Inf.
 * p_m - Predecessor Map. Each predecessor initialized to vertex itself.
 * w_m - Initialized with the time dependent edge weights.
 * e_w - The edge wrapper initialized with edge ids and corresponding source-vertex ids.
 */
void tdsp(graph_t g, vertex_desc_t s, distance_map_t &d_m, predecessor_map_t &p_m, weight_map_t w_m, edge_wrapper e_w)
{
	
	//The visited array to keep track of the visited vertices
	//0 - Not visited
	//1 - visited but in heap
	//2 - finished
	//Initialize all visited 0
	vector<int> visited( d_m.size() ); 
	for(int i = 0; i < visited.size() ; i++)
		visited[i] = 0;
		
	
	//Initialize all distance map entries to Inf
	for( int i = 0; i < d_m.size() ; i++)
	{
		d_m[i] = numeric_limits<double>::max();
		p_m[i] = i;
	}
	
	
	//Priority queue which will act as the min heap
	binary_heap bin_heap;
	
	//Put source vertex in priority queue and initialize visited and distancemap.
	Vertex v;
	v.vertex_id = s;
	v.reach_time = 0;  
	bin_heap.insert(v);
	d_m[s] = 0;
	visited[s] = 1;
	
	
	//Take out vertices from priority queue until it is not empty
	while( !bin_heap.is_empty())
	{
		Vertex u = bin_heap.delete_min();
		
		vertex_desc_t u_t = vertex(u.vertex_id, g);
		
		//Mark u as finished
		visited[u_t] = 2;
		
		//Update final distance map
		if(p_m[u_t] != -1)
			d_m[u_t] = d_m[u_t];
		
		//Visit all the adjecent vertices of v using out edge iterator
		graph_traits<graph_t>::out_edge_iterator ei, edge_end;
		
		for (boost::tie(ei,edge_end) = out_edges(u_t, g); ei != edge_end; ++ei)
		{
			
			//Get the vertex end
			vertex_desc_t v_t = target(*ei,g);
			//Get the edge id
			int ei_id = e_w.get_edge_id(u_t,v_t);
			
			#if DEBUG
			cout<<endl<<"Evaluating edge: "<<ei_id<<" ("<<u_t<<","<<v_t<<")";
			#endif
			
			//Now check if this vertex was already visited
			if(visited[v_t] == 0)
			{
				//This means that vertex is seen for first time.
				//We will put it in priority queue
				Vertex v;
				v.vertex_id = v_t;
				double travel_time = w_m.get_travel_time(ei_id , u.reach_time);
				v.reach_time = u.reach_time + travel_time;
				bin_heap.insert(v);
				
				//Also mark it as visited and update current distance and predecessor_map
				visited[v_t] = 1;
				d_m[v_t] = v.reach_time;
				p_m[v_t] = u_t;
			}
			else if(visited[v_t] == 1)
			{
				//Since visited is 1, the vertex was already seen
				//It is in priority queue.
				
				//Find out new reach time using current edge
				double travel_time = w_m.get_travel_time(ei_id , u.reach_time);
				double new_reach_time = u.reach_time + travel_time;
				
				//Check if new reach time is smaller than the current reach time for v
				if(d_m[v_t] > new_reach_time)  
				{
					//If so, do decrease key in heap, and update the reach time in current distance map
					//and predecessor map
					bin_heap.decrease_key(v_t , d_m[v_t] - new_reach_time);
					d_m[v_t] = new_reach_time;
					p_m[v_t] = u_t;
				}
				//else do nothing, this edge does not belong to shortest path tree
				
			}
			#if DEBUG
			else
			{
				cout << endl << "This vertex is already finished!";
			}
			#endif
		}
		
	}
	
}

predecessor_map_t p_m;
distance_map_t d_m;


void tdsp_test()
{
	
	//Setup the graph
	int V , E , range;
	cin >> V;
	cin >> E;
	cin >> range;
	
	//cout <<"V: "<<V<<" E:"<<E<<" R"<<range<<endl;
	
	graph_t g(V);
	//Setup the edge_wrapper
	edge_wrapper e_w;
	int edge_id_count = 0;
	
	for(int i = 0;i<V;i++)
	{
		int n;
		cin >> n;
		for(int j = 0;j<n;j++)
		{
			int t;
			cin >> t;
			//cout<<"Adding edge : "<<edge_id_count<<"("<<i<<","<<t<<")"<<endl;
			add_edge(vertex(i,g),vertex(t,g),g);
			edge_columns_t e;
			e.id = edge_id_count++; e.source = i; e.target = t; e_w.insert(e);
		}
	}
	
	
	//Setup weight map
	weight_map_t w_m;
	weight_map_element wme;
	for(int i = 0; i < edge_id_count ; i++)
	{
		for(int j = 0 ; j < range; j++)
		{
			int id,start_time,travel_time;
			cin >> id;
			cin >> start_time;
			cin >> travel_time;
			wme.edge_id = id; wme.start_time = start_time; wme.travel_time = travel_time;
			w_m.insert(wme);
		}
	}
	
	//predecessor_map_t p_m;
	//distance_map_t d_m;
	
	for( int i = 0; i < V ; i++)
	{
		d_m[i] = numeric_limits<double>::max();
		p_m[i] = i;
	}
	
	int src;
	cin >> src;
	vertex_desc_t s = src;
	tdsp(g,s,d_m,p_m,w_m,e_w);
	
	cout<<endl<<"---------------------------------------------------------------------------------------"<<endl;
	cout<<"Number of vertices in graph: "<<V<<endl;
	cout<<"Number of edges in graph: "<<E<<endl;
	cout<<"Source vertex: "<<src<<endl;
	
	cout<<endl<<"---------------------------------------------------------------------------------------"<<endl;
	cout<<"Output for time dependent djkstra:"<<endl<<endl;
	cout<<endl<<"Distances: "<<endl;
	for(int i = 0; i < d_m.size() ; i++)
		cout << i << ": " << d_m[i] << "\t\t";
	cout<<endl<<"Predecessors: "<<endl;
	for(int i = 0; i < p_m.size() ; i++)
		cout << i << ": " << p_m[i] << "\t\t";
	cout<<endl;
}


void boost_dijkstra_test()
{
	//Setup the graph
	int V , E , range;
	cin >> V;
	cin >> E;
	cin >> range;
	
	graph_t graph(E);
	//Setup the edge_wrapper
	edge_wrapper e_w;
	int edge_id_count = 0;
	
	for(int i = 0;i<V;i++)
	{
		int n;
		cin >> n;
		for(int j = 0;j<n;j++)
		{
			int t;
			cin >> t;

			//add_edge(vertex(i,g),vertex(t,g),g);
			edge_columns_t e;
			e.id = edge_id_count++; e.source = i; e.target = t; e_w.insert(e);
		}
	}
	
	
	//Setup weight map
	weight_map_t w_m;
	weight_map_element wme;
	for(int i = 0; i < edge_id_count ; i++)
	{
		for(int j = 0 ; j < range; j++)
		{
			int id,start_time,travel_time;
			cin >> id;
			cin >> start_time;
			cin >> travel_time;
			wme.edge_id = id; wme.start_time = start_time; wme.travel_time = travel_time;
			w_m.insert(wme);
		}
	}
	
	int src;
	cin >> src;
	vertex_desc_t s = src;

    property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, graph);

    for (std::size_t j = 0; j < E; ++j)
    {
		edge_desc_t e; bool inserted;
		edge_columns_t e_c = e_w.get_edge(j);
		tie(e, inserted) = add_edge(e_c.source, e_c.target, graph);

		graph[e].reach_time = w_m.get_travel_time(e_c.id, 0 );
		if(graph[e].reach_time < 0)
		{
			cout<< "Edge ID: "<< e_c.id <<"("<<e_c.source<<","<<e_c.target<<") Returned negative weight!"<<endl;
		}
		graph[e].vertex_id = e_c.id;
				
	/*if (!directed || (directed && has_reverse_cost))
	{
	    tie(e, inserted) = add_edge(edges[j].target, edges[j].source, graph);
	    graph[e].id = edges[j].id;

	    if (has_reverse_cost)
	    {
		graph[e].cost = edges[j].reverse_cost;
	    }
	    else 
	    {
		graph[e].cost = edges[j].cost;
	    }
	}*/
    }

    std::vector<vertex_desc_t> predecessors(num_vertices(graph));

    //vertex_descriptor _source = vertex(start_vertex, graph);
/*
    if (_source < 0 ) 
    {
	*err_msg = (char *) "Starting vertex not found";
	return -1;
    }

    vertex_descriptor _target = vertex(end_vertex, graph);
    if (_target < 0)
    {
	*err_msg = (char *) "Ending vertex not found";
	return -1;
    }
*/
    std::vector<double> distances(num_vertices(graph));
    // calling Boost function
    dijkstra_shortest_paths(graph, src,
			    predecessor_map(&predecessors[0]).
			    weight_map(get(&Vertex::reach_time, graph))
			    .distance_map(&distances[0]));
	
	cout<<endl<<"---------------------------------------------------------------------------------------"<<endl;
	
	cout << "Printing output of boost dijkstra: "<<endl;
	
	int success = 1;
	
	cout<<endl<<"Distances: "<<endl;
	for(int i = 0; i < V ; i++)
	{
		cout << i << ": " << distances[i] << "\t\t";
		if(distances[i] != d_m[i])
			success = 0;
	}
	cout<<endl<<"Predecessors: "<<endl;
	for(int i = 0; i < V ; i++)
	{
		cout << i << ": " << predecessors[i] << "\t\t";
		if(predecessors[i] != p_m[i])
			success = 0;
	}
	
	cout<<endl<<"---------------------------------------------------------------------------------------"<<endl;
	if(success == 1)
		cout<< endl << "Both time-dependent dijkstra and boost dijkstra gave same output \n TEST SUCCESSFULL!"<<endl<<endl<<endl;
	else
		cout<< endl << "Output mismatch. Test unsucessfull"<<endl<<endl;
		
}

int main()
{
	
    tdsp_test();
    boost_dijkstra_test();
}

