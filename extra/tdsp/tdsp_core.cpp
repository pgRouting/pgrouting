#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <utility>
#include <queue>

#include <boost/config.hpp>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#define DEBUG 1

using namespace std;
using namespace boost;

//-------------------------------------------------------------------------
//A vertex structure, mainly to be used in distance map.

struct Vertex
{
	int vertex_id;
	double reach_time;
};


//For priority queue
//We want priority queue to select the minimum element first
class CompareVertex 
{
    public:
    bool operator()(Vertex& v1, Vertex& v2) // Returns true if v2 reach time is earlier than v1
    {
       if (v1.reach_time > v2.reach_time) return true;  
       return false;
    }
};

//-------------------------------------------------------------------------
//A Edge structure, mainly to may edge_id with source and target

typedef struct edge 
{
    int id;
    int source;
    int target;
    double cost;
    double reverse_cost;
} edge_t;

class edge_wrapper
{
	vector<edge_t> edges;
	
	public:
	edge_wrapper(){};
	edge_wrapper(edge_t * edges, int size)
	{
		this->edges.assign(edges, edges + size);
	}
	
	void insert(edge_t e)
	{
		edges.push_back(e);
	}
	
	int get_source(int edge_id)
	{
		vector<edge_t>::iterator it;
		for( it = edges.begin() ; it!= edges.end() ; it++)
			if(it->id == edge_id)
				return it->source;
		return -1;

	}
	
	int get_target(int edge_id)
	{
		vector<edge_t>::iterator it;
		for( it = edges.begin() ; it!= edges.end() ; it++)
			if(it->id == edge_id)
				return it->target;
		return -1;

	}
	
	int get_edge_id(int source_id, int target_id)
	{
		vector<edge_t>::iterator it;
		for( it = edges.begin() ; it!= edges.end() ; it++)
			if(it->source == source_id && it->target == target_id)
				return it->id;
		return -1;

	}
	
};


//-------------------------------------------------------------------------
//Weight Map Data Structure


typedef struct weight_map_element
{
		int edge_id;
		double start_time;
		double travel_time;
} 
weight_map_element_t;

class weight_map
{
	
	vector<weight_map_element_t> weight_map_set;
		
	public:
	void insert(weight_map_element_t element);
	double get_travel_time(int edge_id, double start_time);

};

void weight_map::insert(weight_map_element_t element)
{
	weight_map_set.push_back(element);
}

double weight_map::get_travel_time(int edge_id, double start_time)
{
	#if DEBUG
	cout<<endl<<"Query for travel-time for edge "<<edge_id<<" and start-time: "<<start_time;
	#endif
	
	vector<weight_map_element_t>::iterator it, it_next;
	
	it_next=weight_map_set.begin();
	it_next++;
	
	for (it=weight_map_set.begin(); it!=weight_map_set.end(); it++,it_next++)
		if(it->edge_id == edge_id && it->start_time <= start_time && it_next->start_time >start_time)
		{
			#if DEBUG
			cout<<"Arrival Time: "<<start_time<<endl;
			cout<<"edge_id: "<<it->edge_id<<endl;
			cout<<"start_time: "<<it->start_time<<endl;
			cout<<"travel_time: "<<it->travel_time<<endl;
			#endif
			return it->travel_time;
			
		}
		
	return -1;
	
}

/*
 * Binary heap class. Provides binary heap implementation for Vertex as elements.
 * TODO: Maybe make it generic??
 * TODO: Can we use fionacci heap instead??
 * 
 */

class binary_heap
{
	vector<Vertex> heap;
	
	void heapify(int index);
	
	public:
	
	void insert(Vertex v);
	double decrease_key(int index, double delta);
	Vertex delete_min();
	bool is_empty()
	{
		if(heap.size() > 0)
			return false;
		else
			return true;
	}
	
	#if DEBUG
	void print_heap()
	{
		cout<<endl;
		for(int i = 0;i<heap.size();i++)
			cout<<i<<": "<<heap[i].vertex_id<<" "<<heap[i].reach_time<<"\t\t";
	}
	#endif
};


void binary_heap::heapify(int index)
{
	for(int i = index ; 2*i+1 < heap.size() ;)
	{
		if(2*i+2 < heap.size())
		{
			if(heap[2*i+1].reach_time > heap[2*i+2].reach_time)
			{
				if(heap[i].reach_time > heap[2*i+2].reach_time)
				{
					Vertex temp = heap[i];
					heap[i] = heap[2*i+2];
					heap[2*i+2] = temp;
					
				}
				i = 2*i+2;
			}
			else
			{
				if(heap[i].reach_time > heap[2*i+1].reach_time)
				{
					Vertex temp = heap[i];
					heap[i] = heap[2*i+1];
					heap[2*i+1] = temp;
					
				}
				i = 2*i+1;
			}
		}
		else
		{
			if(heap[i].reach_time > heap[2*i+1].reach_time)
			{
				Vertex temp = heap[i];
				heap[i] = heap[2*i+1];
				heap[2*i+1] = temp;
				
			}
			i = 2*i+1;
		}

	}
}

void binary_heap::insert(Vertex v)
{
	#if DEBUG
	cout<<endl<<"Inserting "<<v.vertex_id<<" into heap, with reach time: "<<v.reach_time;
	#endif
	
	heap.push_back(v);
	
	for(int i = heap.size() - 1 ; i > 0 ; i/=2)
	{
		if(heap[i].reach_time < heap[i/2].reach_time)
		{
			Vertex temp = heap[i];
			heap[i] = heap[i/2];
			heap[i/2] = temp;
		}
		else
			break;
	}
}

/*
 * Here, I am assuming that we pass the vertex_id of the node to be updated.
 * This will take O(n) time to search the node in worst case.
 * Ideally this should take O(log n) time if the index of node to be 
 * updated is already known. 
 * TODO: Any alternate implementation, so that O(log n) bound is achieved?
 * 
 */
double binary_heap::decrease_key(int vertex_id, double delta)
{
	#if DEBUG
	cout<<endl<<"Decreasing key of "<<vertex_id<<" by "<<delta;
	#endif
	
	//First search for vertex
	int index = 0;
	for(; index < heap.size() ; index++)
		if(heap[index].vertex_id == vertex_id)
			break;
	
	if(index > heap.size())  //Vertex not found
		return -1; 
	
	int new_key = heap[index].reach_time - delta;
	if( new_key >= 0)
		heap[index].reach_time = new_key;
	else 
		return -1;       //Decrease not possible, will go negative
		
	for(int i = index ; i > 0 ; i/=2)
	{
		if(heap[i].reach_time < heap[i/2].reach_time)
		{
			Vertex temp = heap[i];
			heap[i] = heap[i/2];
			heap[i/2] = temp;
		}
		else
			break;
	}	
	
	return new_key; //success
}


Vertex binary_heap::delete_min()
{
	#if DEBUG
	cout<<endl<<"Deleting min element "<<heap[0].vertex_id;
	#endif
	
	//First swap min element with last
	Vertex min = heap[0];
	heap[0] = heap[heap.size() - 1];
	heap[heap.size() - 1] = min;
	
	
	//Remove the last element and return min
	heap.pop_back();
	
	//Now, heapify root.
	heapify(0);
		
	
	return min;
}



//------------------------------------------------------------------------

//Boost graph data structure types
typedef boost::adjacency_list < listS, vecS, directedS, no_property, Vertex> graph_t;
typedef boost::graph_traits < graph_t >::vertex_descriptor vertex_desc_t;
typedef boost::graph_traits < graph_t >::edge_descriptor edge_desc_t;

//To map the vertex ids to their current distances and predecessors
typedef map<int , double> distance_map;
typedef map<int , int>   predecessor_map;



/**
 * g - Input graph, adjecency list type
 * s - Source vertex
 * d_m - Distance Map. Initialized to Inf.
 * p_m - Predecessor Map. Each predecessor initialized to vertex itself.
 * w_m - Initialized with the time dependent edge weights.
 * e_w - The edge wrapper initialized with edge ids and corresponding source-vertex ids.
 */
void tdsp(graph_t g, vertex_desc_t s, distance_map d_m, predecessor_map p_m, weight_map w_m, edge_wrapper e_w)
{
	cout<<"Source: "<<s<<endl;
	
	//The visited array to keep track of the visited vertices
	//Initialize all visited to false
	vector<bool> visited( d_m.size() ); 
	for(int i = 0; i < visited.size() ; i++)
		visited[i] = false;
	
	//Initialize all distance map entries to Inf
	for( int i = 0; i < d_m.size() ; i++)
		d_m[i] = numeric_limits<double>::max();
		
	cout<< d_m.size();
	
	
	//Priority queue which will act as the min heap
	binary_heap bin_heap;
	
	//Put source vertex in priority queue and initialize visited and distancemap.
	Vertex v;
	v.vertex_id = s;
	v.reach_time = 0;  
	bin_heap.insert(v);
	d_m[s] = 0;
	visited[s] = true;
	
	
	
	//Take out vertices from priority queue until it is not empty
	while( !bin_heap.is_empty())
	{
		Vertex u = bin_heap.delete_min();
		
		vertex_desc_t u_t = vertex(u.vertex_id, g);
		
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
			if(visited[v_t] == false)
			{
				//This means that vertex is seen for first time.
				//We will put it in priority queue
				Vertex v;
				v.vertex_id = v_t;
				double travel_time = w_m.get_travel_time(ei_id , u.reach_time);
				v.reach_time = u.reach_time + travel_time;
				bin_heap.insert(v);
				
				//Also mark it as visited and update distance_map and predecessor_map
				visited[v_t] = true;
				d_m[v_t] = v.reach_time;
				p_m[v_t] = u_t;
			}
			else
			{
				//Since visited is true, the vertex was already seen
				//It is in priority queue.
				
				//Find out new reach time using current edge
				double travel_time = w_m.get_travel_time(ei_id , u.reach_time);
				double new_reach_time = u.reach_time + travel_time;
				
				//Check if new reach time is smaller than the current reach time for v
				if(d_m[v_t] > new_reach_time)  
				{
					//If so, do decrease key in heap, and update the reach time in distance map
					//and predecessor map
					bin_heap.decrease_key(v_t , d_m[v_t] - new_reach_time);
					d_m[v_t] = new_reach_time;
					p_m[v_t] = u_t;
				}
				//else do nothing, this edge does not belong to shortest path tree
				
			}
			
		}
		
	}
	
	cout<<endl<<"Distances: "<<endl;
	for(int i = 0; i < d_m.size() ; i++)
		cout << i << ": " << d_m[i] << endl;
	cout<<"Predecessors: "<<endl;
	for(int i = 0; i < p_m.size() ; i++)
		cout << i << ": " << p_m[i] << endl;
	
}





void weight_map_test()
{
	weight_map wm;
	
	weight_map_element_t wm_element;
	
	for(int i = 0;i<6;i++)
	{
		for(int j = 0; j< 5;j++)
		{
			wm_element.edge_id = i;
			wm_element.start_time = j*5;
			wm_element.travel_time = (i+j)*2;
		
			wm.insert(wm_element);
		}
	}
	
	for(int i = 0;i<6;i++)
	{
		 wm.get_travel_time(i,i*2)  ;
		 cout<<endl;
	 }
	 	
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
			edge_t e;
			e.id = edge_id_count++; e.source = i; e.target = t; e_w.insert(e);
		}
	}
	
	
	//Setup weight map
	weight_map w_m;
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
	
	predecessor_map p_m;
	distance_map d_m;
	
	for( int i = 0; i < V ; i++)
	{
		d_m[i] = numeric_limits<double>::max();
		p_m[i] = -1;
	}
	
	int src;
	cin >> src;
	vertex_desc_t s = src;
	tdsp(g,s,d_m,p_m,w_m,e_w);
	
}

/*
void tdsp_test()
{
	//Setup the graph
	int V , E;
	cin >> V;
	cin >> E;
	
	
	graph_t g(4);
	add_edge(vertex(0,g),vertex(1,g),g);
	add_edge(vertex(0,g),vertex(2,g),g);
	add_edge(vertex(0,g),vertex(3,g),g);
	add_edge(vertex(1,g),vertex(2,g),g);
	add_edge(vertex(1,g),vertex(3,g),g);
	add_edge(vertex(2,g),vertex(1,g),g);
	
	//Setup the edge_wrapper
	edge_wrapper e_w;
	edge_t e;
	e.id = 0; e.source = 0; e.target = 1; e_w.insert(e);
	e.id = 1; e.source = 0; e.target = 2; e_w.insert(e);
	e.id = 2; e.source = 0; e.target = 3; e_w.insert(e);
	e.id = 3; e.source = 1; e.target = 2; e_w.insert(e);
	e.id = 4; e.source = 1; e.target = 3; e_w.insert(e);
	e.id = 5; e.source = 2; e.target = 3; e_w.insert(e);
	
	//Setup weight map
	weight_map w_m;
	weight_map_element wme;
	wme.edge_id = 0; wme.start_time = 0; wme.travel_time = 6;
	w_m.insert(wme);
	wme.edge_id = 0; wme.start_time = 5; wme.travel_time = 3;
	w_m.insert(wme);
	wme.edge_id = 0; wme.start_time = 10; wme.travel_time = 7;
	w_m.insert(wme);

	wme.edge_id = 1; wme.start_time = 0; wme.travel_time = 20;
	w_m.insert(wme);
	wme.edge_id = 1; wme.start_time = 5; wme.travel_time = 16;
	w_m.insert(wme);
	wme.edge_id = 1; wme.start_time = 10; wme.travel_time = 6;
	w_m.insert(wme);

	wme.edge_id = 2; wme.start_time = 0; wme.travel_time = 13;
	w_m.insert(wme);
	wme.edge_id = 2; wme.start_time = 5; wme.travel_time = 9;
	w_m.insert(wme);
	wme.edge_id = 2; wme.start_time = 10; wme.travel_time = 8;
	w_m.insert(wme);

	wme.edge_id = 3; wme.start_time = 0; wme.travel_time = 2;
	w_m.insert(wme);
	wme.edge_id = 3; wme.start_time = 5; wme.travel_time = 3;
	w_m.insert(wme);
	wme.edge_id = 3; wme.start_time = 10; wme.travel_time = 2;
	w_m.insert(wme);

	wme.edge_id = 4; wme.start_time = 0; wme.travel_time = 4;
	w_m.insert(wme);
	wme.edge_id = 4; wme.start_time = 5; wme.travel_time = 5;
	w_m.insert(wme);
	wme.edge_id = 4; wme.start_time = 10; wme.travel_time = 3;
	w_m.insert(wme);

	wme.edge_id = 5; wme.start_time = 0; wme.travel_time = 7;
	w_m.insert(wme);
	wme.edge_id = 5; wme.start_time = 5; wme.travel_time = 9;
	w_m.insert(wme);
	wme.edge_id = 5; wme.start_time = 10; wme.travel_time = 6;
	w_m.insert(wme);
	
	predecessor_map p_m;
	distance_map d_m;
	
	for( int i = 0; i < 4 ; i++)
	{
		d_m[i] = numeric_limits<double>::max();
		p_m[i] = -1;
	}
	
	vertex_desc_t s = 0;
	tdsp(g,s,d_m,p_m,w_m,e_w);
}
*/
int main()
{
    tdsp_test();
}

