#include <iostream>
#include <vector>
#include <utility>
#include <tdsp.h>

#define DEBUG_CONSOLE 0

using namespace std;

//-------------------------------------------------------------------------
//A vertex structure, mainly to be used in distance map.

struct Vertex
{
	int vertex_id;
	float8 reach_time;
};




/*
 * Binary heap class. Provides binary heap implementation for Vertex as elements.
 * TODO: Maybe make it generic??
 * TODO: Can we use fionacci heap instead??
 * 
 */

class binary_heap
{
	public:
	vector<Vertex> heap;
	
	void heapify(int index);
	
	
	
	void insert(Vertex v);
	float8 decrease_key(int index, float8 delta);
	Vertex delete_min();
	bool is_empty()
	{
		if(heap.size() > 0)
			return false;
		else
			return true;
	}
	
	int parent(int i)
	{
		if(i%2 == 0)
			return ((i/2) - 1);
		else 
			return ((i-1)/2);
	}
	
	#if DEBUG_CONSOLE
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
					i = 2*i+2;
				}
				else
				return;
			}
			else
			{
				if(heap[i].reach_time > heap[2*i+1].reach_time)
				{
					Vertex temp = heap[i];
					heap[i] = heap[2*i+1];
					heap[2*i+1] = temp;
					i = 2*i+1;
				}
				else
				return;
			}
		}
		else
		{
			if(heap[i].reach_time > heap[2*i+1].reach_time)
			{
				Vertex temp = heap[i];
				heap[i] = heap[2*i+1];
				heap[2*i+1] = temp;
				i = 2*i+1;
			}
			else
			return;
			
		}

	}
}

void binary_heap::insert(Vertex v)
{
	#if DEBUG_CONSOLE
	cout<<endl<<"Inserting "<<v.vertex_id<<" into heap, with reach time: "<<v.reach_time;
	#endif
	
	heap.push_back(v);
	
	for(int i = heap.size() - 1 ; i > 0 ;)
	{
		int p = parent(i);
		
		if(heap[i].reach_time < heap[p].reach_time)
		{
			Vertex temp = heap[i];
			heap[i] = heap[p];
			heap[p] = temp;
			i = p;
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
float8 binary_heap::decrease_key(int vertex_id, float8 delta)
{
	#if DEBUG_CONSOLE
	cout<<endl<<"Decreasing key of "<<vertex_id<<" by "<<delta;
	#endif
	
	//First search for vertex
	int index = 0;
	for(; index < heap.size() ; index++)
		if(heap[index].vertex_id == vertex_id)
			break;
	
	if(index == heap.size())  //Vertex not found
		return -1; 
	
	float8 new_key = heap[index].reach_time - delta;
	if( new_key >= 0)
		heap[index].reach_time = new_key;
	//else                  //This case will never be called anyways
		//return -1;       //Decrease not possible, will go negative
		
	for(int i = index ; i > 0 ; )
	{
		int p = parent(i);
		if(heap[i].reach_time < heap[p].reach_time)
		{
			Vertex temp = heap[i];
			heap[i] = heap[p];
			heap[p] = temp;
			i = p;
		}
		else
			break;
	}	
	
	return new_key; //success
}


Vertex binary_heap::delete_min()
{
	#if DEBUG_CONSOLE
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

