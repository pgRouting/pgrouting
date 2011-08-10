/*--
-- Copyright (c) 2011 Jay Mahadeokar
--
-- This program is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation; either version 2 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program; if not, write to the Free Software
-- Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#include <vector>
#include <tdsp.h>
#define DEBUG_CONSOLE 0

using namespace std;

//Weight Map Data Structure

class weight_map_t
{
	
	vector<weight_map_element_t> weight_map_set;
	bool cyclic;
	float8 cycle_interval;
	
	public:
	weight_map_t();
	void insert(weight_map_element_t element);
	float8 get_travel_time(int edge_id, float8 start_time);

	bool is_cyclic();
	void set_cyclic(bool c);
	float8 get_cycle_interval();
	void set_cycle_interval(float8 cycle_interval);
};

//Default Consructor.
weight_map_t::weight_map_t()
{
	cyclic = false;
}


float8 weight_map_t::get_cycle_interval()
{
	return cycle_interval;
}

void weight_map_t::set_cycle_interval(float8 c_i)
{
	cycle_interval = c_i;
}

bool weight_map_t::is_cyclic()
{
	return cyclic;
}

void weight_map_t::set_cyclic(bool c)
{
	cyclic = c;
}

void weight_map_t::insert(weight_map_element_t element)
{
	weight_map_set.push_back(element);
}


/*
 * Function that returns the travel time of the edge with given id corresponding 
 * to the start time specified.
 */
float8 weight_map_t::get_travel_time(int edge_id, float8 start_time)
{
	#if DEBUG_CONSOLE
	cout<<endl<<"Query for travel-time for edge "<<edge_id<<endl;//" and start-time: "<<start_time;
	#endif
	
	//This logic needs to be refined when the cyclic behaviour is dealt with in future versions.
	//It is just a dummy now since the is_cyclic flag is false by default in constructor.
	if(cyclic)
	{
		
		start_time = (int)start_time % (int)cycle_interval;
	}
	
	
	vector<weight_map_element_t>::iterator it;
	
	for (it=weight_map_set.begin(); it!=weight_map_set.end(); it++)
	{
		if(it->edge_id == edge_id && it->start_time <= start_time && it->end_time > start_time)
		{
				return it->travel_time;
		}
	}	
	return -1;
	
}


