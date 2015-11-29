/*PGR-GNU*****************************************************************
File: postgres_connection.h

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

#ifndef SRC_COMMON_SRC_POSTGRES_CONNECTION_H_
#define SRC_COMMON_SRC_POSTGRES_CONNECTION_H_

#include "postgres.h"
#include "executor/spi.h"
#include "utils/array.h"


#include "./pgr_types.h"



  void pgr_SPI_finish(void);
  void pgr_SPI_connect(void);

  int64_t* pgr_get_bigIntArray(size_t *arrlen, ArrayType *input);
/*
  int pgr_fetch_edge_columns(SPITupleTable *tuptable, int (*edge_columns)[5],
                   bool has_reverse_cost);
  void pgr_fetch_edge(HeapTuple *tuple, TupleDesc *tupdesc,
           int (*edge_columns)[5], pgr_edge_t *target_edge,
           bool has_rcost);
*/

  /*!
   Signature 1:
      bigint source,
      bigint target,
      float cost
      float reverse_cost
  */
  void pgr_get_data_4_columns(
      char *sql,           //!< \param [IN]  sql from where we get the data
      pgr_edge_t **edges,  //!< \param [OUT] edges retrieved edges
      int64_t *total_tuples);  //!< \param [OUT] total_tuples Total edges retrived

  /*!
   Signature 1:
      bigint id,
      bigint source,
      bigint target,
      float cost
      float reverse_cost
  */
  void pgr_get_data_5_columns(
      char *sql,           //!< \param [IN]  sql from where we get the data
      pgr_edge_t **edges,  //!< \param [OUT] edges retrieved edges
      int64_t *total_tuples);  //!< \param [OUT] total_tuples Total edges retrived

  void
      pgr_select_bigint(
              char *sql,
              int64 **columnValues,
              size_t *totalTuples);

  char * pgr_text2char(text *in);

  // TODO make template
  //General_path_element_t* noPathFound(size_t *count, General_path_element_t *no_path);


#endif  // SRC_COMMON_SRC_POSTGRES_CONNECTION_H_
