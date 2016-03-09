/*PGR-GNU*****************************************************************
FILE: alpha.h

Copyright (c) 2006 Anton A. Patrushev, Orkney, Inc.
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

#define _ALPHA_H

#ifdef __MINGW64__
#define ELOG_H
#endif
#include "postgres.h"
#include "../../common/src/pgr_types.h"

typedef struct vertex
{
  float8 x;
  float8 y;
} vertex_t;


#ifdef __cplusplus
extern "C"
{
#endif
                                                                     
  int alpha_shape(vertex_t *vertices, size_t count, double alpha,
                  vertex_t **res, size_t *res_count, char **err_msg);

#ifdef __cplusplus
}
#endif

                                        
