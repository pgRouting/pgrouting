#include <vector>

#define DEBUG 0

using namespace std;

//-------------------------------------------------------------------------
//Weight Map Data Structure


typedef struct weight_map_element
{
		int edge_id;
		double start_time;
		double travel_time;
} 
weight_map_element_t;

class weight_map_t
{
	
	vector<weight_map_element_t> weight_map_set;
		
	public:
	void insert(weight_map_element_t element);
	double get_travel_time(int edge_id, double start_time);

};

void weight_map_t::insert(weight_map_element_t element)
{
	weight_map_set.push_back(element);
}

double weight_map_t::get_travel_time(int edge_id, double start_time)
{
	#if DEBUG
	cout<<endl<<"Query for travel-time for edge "<<edge_id<<endl;//" and start-time: "<<start_time;
	#endif
	
	vector<weight_map_element_t>::iterator it, it_next;
	
	it_next=weight_map_set.begin();
	it_next++;
	
	for (it=weight_map_set.begin(); it!=weight_map_set.end(); it++,it_next++)
	{
		if(it_next != weight_map_set.end())
		{
			if(it_next->edge_id == edge_id)
			{
				if(it->edge_id == edge_id && it->start_time <= start_time && it_next->start_time >start_time)
				{
					#if DEBUG
					cout<<"Edge ID passed: "<<edge_id<<endl;
					cout<<"Arrival Time: "<<start_time<<endl;
					cout<<"it->edge_id: "<<it->edge_id<<endl;
					cout<<"start_time: "<<it->start_time<<endl;
					cout<<"travel_time: "<<it->travel_time<<endl;
					#endif
					return it->travel_time;
				}
			}	
			else if(it->edge_id == edge_id && it->start_time <= start_time)
			{
				#if DEBUG
				cout << 2 << endl;
				cout<<"Edge ID passed: "<<edge_id<<endl;
					cout<<"Arrival Time: "<<start_time<<endl;
					cout<<"it->edge_id: "<<it->edge_id<<endl;
					cout<<"start_time: "<<it->start_time<<endl;
					cout<<"travel_time: "<<it->travel_time<<endl;
				#endif
				return it->travel_time;
			}	
		}
		else
		{
			if(it->edge_id == edge_id && it->start_time <= start_time )
			{
				#if DEBUG
					cout << 3 << endl;
					cout<<"Edge ID passed: "<<edge_id<<endl;
					cout<<"Arrival Time: "<<start_time<<endl;
					cout<<"it->edge_id: "<<it->edge_id<<endl;
					cout<<"start_time: "<<it->start_time<<endl;
					cout<<"travel_time: "<<it->travel_time<<endl;
				#endif
				return it->travel_time;
			
			}
		}	
	}	
	return -1;
	
}



/*
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

*/
