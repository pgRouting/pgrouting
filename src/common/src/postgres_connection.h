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

#ifndef SRC_COMMON_SRC_POSTGRES_CONNECTION_H_
#define SRC_COMMON_SRC_POSTGRES_CONNECTION_H_

#include "postgres.h"
#include "executor/spi.h"
#include "utils/array.h"


#include "./pgr_types.h"
#include "./postgres_connection.h"

#ifdef DEBUG
#define PGR_DBG(format, arg...) \
elog(NOTICE, format , ## arg)
#else
#define PGR_DBG(format, arg...) do { ; } while (0)
#endif


#ifdef __cplusplus
extern "C" {
#endif

  char * pgr_text2char(text *in);
  int pgr_finish(int code, int ret);
  int64_t* pgr_get_bigIntArray(int *arrlen, ArrayType *input);
/*
  int pgr_fetch_edge_columns(SPITupleTable *tuptable, int (*edge_columns)[5],
                   bool has_reverse_cost);
  void pgr_fetch_edge(HeapTuple *tuple, TupleDesc *tupdesc,
           int (*edge_columns)[5], pgr_edge_t *target_edge,
           bool has_rcost);
*/

  /*!
   Signature 1:
      bigint id,
      bigint source,
      bigint target,
      float cost
      float reverse_cost
  */
  int pgr_get_data(
      char *sql,           //!< \param [IN]  sql from where we get the data
      pgr_edge_t **edges,  //!< \param [OUT] edges retrieved edges
      int64_t *total_tuples,  //!< \param [OUT] total_tuples Total edges retrived
      bool has_rcost,      //!< \param [IN]  has_rcost flag for reverse_cost
      int64_t start_vertex,  //!< \param [IN] start_vertex index to look for
      int64_t end_vertex);   //!< \param [IN] end_vertex index to look for


  // output corresponding to costResult3Big
  General_path_element_t* get_memory(int size, General_path_element_t *path);
  // pgr_path_element3_t * noPathFound3(int64_t start_id);
  General_path_element_t* noPathFound(size_t *count, General_path_element_t *no_path);


#ifdef __cplusplus
}
#endif

#endif  // SRC_COMMON_SRC_POSTGRES_CONNECTION_H_
