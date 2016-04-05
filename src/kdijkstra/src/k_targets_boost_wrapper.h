/*PGR-GNU*****************************************************************

 * Bi Directional Shortest path algorithm for PostgreSQL
 *
 * Copyright (c) 2005 Sylvain Pasche
Mail: project@pgrouting.org

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/

#ifndef _TOMANYSP_H
#define _TOMANYSP_H

//#include "MyWrapper.h"

#include "postgres.h"
// #include "../../common/src/pgr_types.h"

typedef struct {
    int id;
    int source;
    int target;
    double cost;
    double reverse_cost;
} edge_t;


typedef struct
{
    int seq;
    int id1;
    int id2;
    float8 cost;
} pgr_cost_t;

typedef struct
{
    int seq;
    int id1;
    int id2;
    int id3;
    float8 cost;
} pgr_cost3_t;


typedef struct 
{
    int vertex_id_source;
    int edge_id_source;
    int vertex_id_target;
    int edge_id_target;
    float8 cost;
} dist_fromto_t;


typedef struct 
{
    int vertex_id_source;
    int edge_id_source;
    int vertex_id_target;
    int edge_id_target;
    float8 cost;
    char* the_way;
} path_fromto_t;

#ifdef __cplusplus
extern "C" {
#endif


    int onetomany_dijkstra_boostdist(
            edge_t *edges, unsigned int count, int start_vertex,
            int *end_vertices, int nb_targets,
            bool directed, bool has_reverse_cost,
            pgr_cost_t **dists,
            char **err_msg );


#ifdef __cplusplus
}
#endif

#endif // _TOMANYSP_H
