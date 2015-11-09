/*PGR

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

*/


#ifndef SRC_KSP_SRC_KSP_H_
#define SRC_KSP_SRC_KSP_H_

// #define TUPLIMIT 1000
// #define PATH_ALLOC_TOTAL 5

// #include <unistd.h>
// #include "postgres.h"

#ifdef __cplusplus
extern "C" {
#endif


#include "./../../common/src/pgr_types.h"

int compute(char* sql, int64_t start_vertex,
            int64_t end_vertex, int no_paths,
            bool has_reverse_cost, bool directedFlag,
            General_path_element_t **path, size_t *ksp_path_count);

#ifdef __cplusplus
}
#endif

#endif  // SRC_KSP_SRC_KSP_H_
