/*PGR-GNU*****************************************************************
File: points_input.c

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

#include "postgres.h"
#include "utils/lsyscache.h"
#include "catalog/pg_type.h"
#include "utils/array.h"
#include "executor/spi.h"


#define DEBUG
#include "./debug_macro.h"
#include "pgr_types.h"
#include "postgres_connection.h"
#include "points_input.h"




static
void fetch_points_column_info(
     int (*columns)[4], 
     int (*types)[4]) { 

  PGR_DBG("Entering Fetch_points_column_info\n");

  pgr_fetch_column_info(&(*columns)[0], &(*types)[0], "pid");
  pgr_check_any_integer_type("pid", (*types)[0]);

  pgr_fetch_column_info(&(*columns)[0], &(*types)[0], "edge_id");
  pgr_check_any_integer_type("edge_id", (*types)[0]);

  pgr_fetch_column_info(&(*columns)[1], &(*types)[1], "fraction");
  pgr_check_any_numerical_type("fraction", (*types)[1]);

  pgr_fetch_column_info(&(*columns)[2], &(*types)[2], "side");
  pgr_check_text_type("side", (*types)[2]);

}

static
void fetch_point(
   HeapTuple *tuple,
   TupleDesc *tupdesc, 
   int (*columns)[4],
   int (*types)[4],
   Point_on_edge_t *point) {

  point->pid = pgr_SPI_getBigInt(tuple, tupdesc, (*columns)[0], (*types)[0]);
  point->edge_id = pgr_SPI_getBigInt(tuple, tupdesc, (*columns)[1], (*types)[1]);
  point->fraction = pgr_SPI_getFloat8(tuple, tupdesc, (*columns)[2], (*types)[2]);
  point->side = (char)pgr_SPI_getBigInt(tuple, tupdesc, (*columns)[3], (*types)[3]);

}


// pid, edge_id, fraction, [side] 
void
pgr_get_points(
        char *points_sql,
        Point_on_edge_t **points,
        int64_t *total_points) {

  const int tuple_limit = 1000000;

  PGR_DBG("Entering pgr_get_restriction_data");

  int64_t ntuples;
  int64_t total_tuples;

  int columns[4];
  int types[4];
  int i;
  for (i = 0; i < 4; ++i) columns[i] = -1;
  for (i = 0; i < 4; ++i) types[i] = -1;

        
  void *SPIplan;
  SPIplan = pgr_SPI_prepare(points_sql);

  Portal SPIportal;
  SPIportal = pgr_SPI_cursor_open(SPIplan);

  bool moredata = TRUE;
  (*total_points) = total_tuples = 0;

  /*  on the first tuple get the column numbers */

  PGR_DBG("Starting Cycle");
  while (moredata == TRUE) {
      SPI_cursor_fetch(SPIportal, TRUE, tuple_limit);
      if (total_tuples == 0)
         fetch_points_column_info(&columns, &types);

      ntuples = SPI_processed;
      total_tuples += ntuples;

      if (ntuples > 0) {
          PGR_DBG("Getting Memory");
          if ((*points) == NULL)
            (*points) = (Point_on_edge_t *)palloc0(total_tuples * sizeof(Point_on_edge_t));
          else
            (*points) = (Point_on_edge_t *)repalloc((*points), total_tuples * sizeof(Point_on_edge_t));
          PGR_DBG("Got Memory");

          if ((*points) == NULL) {
           elog(ERROR, "Out of memory"); 
          }

          int64_t t;
          SPITupleTable *tuptable = SPI_tuptable;
          TupleDesc tupdesc = SPI_tuptable->tupdesc;
          PGR_DBG("processing %ld", ntuples);
          for (t = 0; t < ntuples; t++) {
              PGR_DBG("   processing %ld", t);
              HeapTuple tuple = tuptable->vals[t];
              fetch_point(&tuple, &tupdesc, &columns, &types,
                         &(*points)[total_tuples - ntuples + t]);
          }
          SPI_freetuptable(tuptable);
      } else {
          moredata = FALSE;
      }
  }

  if (total_tuples == 0) {
    (*total_points) = 0;
    PGR_DBG("NO points");
    PGR_DBG("closed");
    return;
  }
  

  (*total_points) = total_tuples;
  PGR_DBG("Finish reading %ld data, %ld", total_tuples, (*total_points));
  PGR_DBG("closed");
}


