#include <iostream>
#include <vector>
#include <utility>

#define DEBUG 0

using namespace std;

//-------------------------------------------------------------------------
//A vertex structure, mainly to be used in distance map.

struct Vertex
{
	int vertex_id;
	double reach_time;
};




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

