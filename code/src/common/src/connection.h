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

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "postgres.h"
#include "executor/spi.h"
#include "utils/array.h"

#include "./structs.h"
#include "./connection.h"

#ifdef DEBUG
#define PGR_DBG(format, arg...) \
elog(NOTICE, format , ## arg)
#else
#define PGR_DBG(format, arg...) do { ; } while (0)
#endif

//tells the compiler to treat the functions as C functions without mangling
#ifdef __cplusplus
extern "C" {
#endif

  char * text2char(text *in);
  int finish(int code, int ret);
  //int64_t* pgr_get_bigIntArray(int *arrlen, ArrayType *input);


  int fetch_edge_columns(int (*edge_columns)[5],int (*edge_types)[5],
                   bool has_reverse_cost);
  int fetch_astar_edge_columns(int (*edge_columns)[9],int (*edge_types)[9], bool has_rcost);
  void fetch_edge(HeapTuple *tuple, TupleDesc *tupdesc,
           int (*edge_columns)[5], int (*edge_types)[5],Edge *target_edge,
           bool has_rcost);
void fetch_astar_edge(HeapTuple *tuple,TupleDesc *tupdesc, 
 int (*edge_columns)[9],int (*edge_types)[9],Edge *target_edge,
 bool has_rcost);
  /*!
   Signature 1:
      bigint id,
      bigint source,
      bigint target,
      float cost
      float reverse_cost
  */
  int fetch_data(
      char *sql, Edge **edges,int *count);   //!< \param [IN] end_vertex index to look for
int
fetch_astar_data(char *sql, Edge **edges,int *count);

  /* output corresponding to costResult3Big
  pgr_path_element3_t* pgr_get_memory3(int size, pgr_path_element3_t *path);
  // pgr_path_element3_t * noPathFound3(int64_t start_id);
  pgr_path_element3_t* noPathFound3(int64_t fill_value, int *count, pgr_path_element3_t *no_path);
*/

#ifdef __cplusplus
}
#endif

#endif  // CONNECTION_H_
