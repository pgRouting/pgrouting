#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <utility>
#include <queue>

#include <binary_heap.h>
#include <weight_map.h>
#include <edge_wrapper.h>
#include <tdsp.h>

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



int tdsp_wrapper(

			edge_columns_t *edges, 
			unsigned int edge_count, 
			weight_map_element_t *weight_map_elements, 
			int weight_map_element_count,
			int start_vertex, 
			int end_vertex,
		    bool directed, 
		    bool has_reverse_cost,
            path_element_t **path, 
            int *path_count, 
            char **err_msg

            )
{
	cout << "edge count: "<<edge_count << endl;
	
	set<int> vertices;
	
	//Form edge wrapper
	edge_wrapper e_w;
	for(int i = 0 ; i < edge_count ; i++)
	{
		e_w.insert(edges[i]);
		if(edges[i].source < 0 || edges[i].source > 99)
		{
			cout << i << "\t";
			//cout << "OOps Source: " << edges[i].source << "\t";
		}
		if(edges[i].target < 0 || edges[i].target > 99)
		{
			cout << i << "\t";
			//cout << "OOps target"<< edges[i].target << "\t";
		}
		vertices.insert(edges[i].source);
		vertices.insert(edges[i].target);
	}

	cout << endl << vertices.size() << endl;
	const int num_nodes = vertices.size();
	graph_t g(num_nodes);
		
	for(int i = 0 ; i < edge_count ; i++)
	{	
		add_edge(vertex(edges[i].source,g) , vertex(edges[i].target,g) , g);
	}
	
	//Form weight map
	weight_map_t w_m;
	for(int i = 0 ; i < weight_map_element_count ; i++)
		w_m.insert(weight_map_elements[i]);
	
	predecessor_map_t p_m;
	distance_map_t d_m;
	for( int i = 0; i < num_nodes ; i++)
	{
		d_m[i] = numeric_limits<double>::max();
		p_m[i] = i;
	}

	vertex_desc_t s = start_vertex;
	
	cout << "calling tdsp" << endl;
	//Call the time dependent shortest path function
	tdsp(g,s,d_m,p_m,w_m,e_w);
	
	#if DEBUG
	cout << "Number of vertices: " << num_nodes<<endl;
	cout << "Number of edges: " << edge_count << endl;
	cout<<"Source vertex: "<<s<<endl;
	cout<<endl<<"---------------------------------------------------------------------------------------"<<endl;
	cout<<"Output for time dependent djkstra:"<<endl<<endl;
	cout<<endl<<"Distances: "<<endl;
	for(int i = 0; i < d_m.size() ; i++)
		cout << i << ": " << d_m[i] << "\t\t";
	cout<<endl<<"Predecessors: "<<endl;
	for(int i = 0; i < p_m.size() ; i++)
		cout << i << ": " << p_m[i] << "\t\t";
	cout<<endl;
	#endif
	
	
	//Now ectract the path
	vector<int> path_vect;
    int max = num_nodes;
    int _target = end_vertex;
    path_vect.push_back(_target);

    while (_target != start_vertex) 
    {
		if (_target == p_m[_target]) 
		{
			*err_msg = (char *) "No path found";
			return 0;
		}
		_target = p_m[_target];

		path_vect.push_back(_target);
		if (!max--) 
		{
			*err_msg = (char *) "Overflow";
			return -1;
		}
    }

    *path = (path_element_t *) malloc(sizeof(path_element_t) * (path_vect.size() + 1));
    *path_count = path_vect.size();
	
	double time = 0;
	for(int i = path_vect.size() - 1, j = 0; i >= 0; i--, j++)
    {
		(*path)[j].vertex_id = path_vect[i];

		(*path)[j].edge_id = -1;
		(*path)[j].cost = d_m[_target];
	
		if (i == 0) 
		{
			time = d_m[_target];
			continue;
		}
		
		(*path)[j].edge_id = e_w.get_edge_id(path_vect[i] , path_vect[i-1]);
		
		(*path)[j].cost = w_m.get_travel_time((*path)[j].edge_id, time);
		
		time+= (*path)[j].cost;
		
	}
	return 1;
}



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
				
    }

    std::vector<vertex_desc_t> predecessors(num_vertices(graph));

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




void tdsp_wrapper_test()
{
	//Setup the graph
	int V , E , range;
	cin >> V;
	cin >> E;
	cin >> range;
	
	cout << "V: " << V << " E: " << E << " range: " << range << endl;
	
	edge_columns_t * edges = (edge_columns_t*) malloc(E * sizeof(edge_columns_t));
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
			//edge_columns_t e;
			edges[edge_id_count].id = edge_id_count; 
			edges[edge_id_count].source = i; 
			edges[edge_id_count].target = t; 
			edge_id_count++;
			
		}
	}
	
	cout << "Edge id count while reading : "<<edge_id_count << endl;
	//Setup weight map
	weight_map_element * weight_map_elements = (weight_map_element *) malloc(sizeof(weight_map_element) * E * range);
	for(int i = 0; i < edge_id_count ; i++)
	{
		for(int j = 0 ; j < range; j++)
		{
			int id,start_time,travel_time;
			cin >> id;
			cin >> start_time;
			cin >> travel_time;
			weight_map_elements[(i*range)+ j].edge_id = id; 
			weight_map_elements[(i*range)+ j].start_time = start_time;
			weight_map_elements[(i*range)+ j].travel_time = travel_time;
			
		}
	}
	
	int src;
	cin >> src;
	
	int path_count = 0;
	path_element_t * path_elements;
	char ** err_msg;
	
	
	int end_vertex = 46;
	
	cout << "Calling tdsp_wrapper" <<endl;
	int success = tdsp_wrapper(edges , E , weight_map_elements , E*range , src , end_vertex , false , false ,
				&path_elements , &path_count , err_msg);
	
	if(success == -1)
	{
		cout << "Overflow" << endl;
		return;
	}
	if(success == 0)
	{
		cout << "Not reachable" << endl;
		return;
	}
	
	cout << "Output for source - target query: " << endl;
	int total_time = 0;
	for(int i = 0 ; i < path_count ; i++)
	{
		cout << path_elements[i].vertex_id << " " << path_elements[i].edge_id << " " <<path_elements[i].cost << endl;
		total_time += path_elements[i].cost;
		
	}
	cout << endl;
	cout << "Total time: " << total_time << endl;
}


int main()
{
	
    //tdsp_test();
    cout << "Now calling tdsp caller test" << endl;
    tdsp_wrapper_test();
    boost_dijkstra_test();
}

