#ifdef __cplusplus

extern "C" {

#endif 
int  doKpaths(ksp_edge_t  * edges, int total_tuples,int  start_vertex,int  end_vertex,
                       int no_paths, bool has_reverse_cost,
                       ksp_path_element_t **ksp_path, int *path_count,
			char ** err_msg);
#ifdef __cplusplus 
}
#endif 
