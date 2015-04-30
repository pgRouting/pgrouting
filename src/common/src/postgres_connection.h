/*
 * kShortest path algorithm for PostgreSQL
 *
 * Copyright (c) 2011 Dave Potts
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */
#ifndef _POSTGRES_CONNECTION_H 
#define _POSTGRES_CONNECTION_H 

#include "postgres.h"
#include "executor/spi.h"

#include "pgr_types.h"
#include "postgres_connection.h"


#define DEBUG 1
#ifdef DEBUG 
#define DBG(format, arg...) \
elog(NOTICE, format , ## arg)
#else
#define DBG(format, arg...) do { ; } while (0)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

  char * pgr_text2char(text *in);
  int pgr_finish(int code, int ret);
  pgr_path_element3_t * noPathFound3(int64_t start_id);
			  
  int pgr_fetch_edge_columns(SPITupleTable *tuptable, int (*edge_columns)[5], 
                   bool has_reverse_cost);
  void pgr_fetch_edge(HeapTuple *tuple, TupleDesc *tupdesc, 
           int (*edge_columns)[5], pgr_edge_t *target_edge,
           bool has_rcost);

  pgr_path_element3_t* pgr_get_memory3(int size, pgr_path_element3_t *path);
  int pgr_get_data(
      char *sql,
      pgr_edge_t **edges,
      long *total_tuples,
      bool has_reverse_cost,
      int64_t start_Vertex,
      int64_t end_vertex,
      bool *sourceFound,
      bool *targetFound);



#ifdef __cplusplus
}
#endif

#endif  // _POSTGRES_CONNECTION_H 
