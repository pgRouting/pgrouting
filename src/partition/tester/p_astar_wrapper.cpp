


#include <exception>
#include "P_AStar.h"
#include "p_astar.h"

int p_astar_wrapper(int source_vertex_id, int target_vertex_id, char **err_msg)
{
	int res;

	try {
		P_AStar Pastar;
		res = Pastar.p_astar( source_vertex_id, target_vertex_id, err_msg);     
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

