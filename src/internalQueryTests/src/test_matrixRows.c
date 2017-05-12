/*PGR-GNU*****************************************************************
File: testXYedges.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: 

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

#include "c_common/postgres_connection.h"


#include "c_common/matrixRows_input.h"

#include "c_common/debug_macro.h"
#include "./test_matrixRows_driver.h"

PGDLLEXPORT Datum test_matrixRows(PG_FUNCTION_ARGS);


/*******************************************************************************/
/*                          MODIFY AS NEEDED                                   */
static
void
process(char* matrix_rows_sql,
        bool *result_bool) {
    pgr_SPI_connect();

    PGR_DBG("Load data");
    Matrix_cell_t *matrix_rows = NULL;
    size_t total_rows = 0;
    pgr_get_matrixRows(matrix_rows_sql, &matrix_rows, &total_rows);

    if (total_rows == 0) {
        PGR_DBG("No rows found");
        (*result_bool) = true;
        pfree(matrix_rows);
        pgr_SPI_finish();
        return;
    }

    PGR_DBG("Total %ld rows in query:", total_rows);
    size_t i;
    for (i = 0; i < total_rows; ++i) {
        PGR_DBG("start_vid = %li \t end_vid = %li agg_cost = %lf",
               matrix_rows[i].from_vid, matrix_rows[i].to_vid, matrix_rows[i].cost);
    }


    PGR_DBG("Starting processing");
    char *err_msg = NULL;
    char *log_msg = NULL;
    (*result_bool) = do_pgr_test_matrixRows(
            matrix_rows,
            total_rows,
            &log_msg,
            &err_msg);

    pfree(matrix_rows);
    PGR_DBG("Returned log message = %s\n", log_msg);
    if (log_msg) {
        elog(DEBUG1, "%s", log_msg);
        free(log_msg);
    }
    PGR_DBG("Returned error message = %s\n", err_msg);

    if (err_msg) {
        pgr_SPI_finish();
        elog(ERROR, "%s", err_msg);
        free(err_msg);
    }

    pgr_SPI_finish();
}


PG_FUNCTION_INFO_V1(test_matrixRows);
PGDLLEXPORT Datum
test_matrixRows(PG_FUNCTION_ARGS) {
    bool  result_bool = false;

    process(
            text_to_cstring(PG_GETARG_TEXT_P(0)),
            &result_bool);

    PG_RETURN_BOOL(result_bool);
}
