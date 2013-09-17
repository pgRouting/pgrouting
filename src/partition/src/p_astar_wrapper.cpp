


#include <exception>
#include "P_AStar.h"
#include "p_astar.h"

int p_astar_wrapper(int start_vertex, int end_vertex,int s_pid ,int t_pid,bool has_reverse_cost, path_element_t **path , int *path_count,
		char **err_msg);

{
	int res;

	try {
		P_AStar Pastar;
		res = Pastar.p_astar(start_vertex,end_vertex,s_pid,t_pid,has_reverse_cost,path,path_count,err_msg);     
	}
	catch(std::exception& e) {
		*err_msg = (char *) e.what();
		return -1;
	}
	catch(...) {
		*err_msg = (char *) "Caught unknown exception!";
		return -1;
	}

	if (res < 0)
		return res;
	else
		return EXIT_SUCCESS;
}

