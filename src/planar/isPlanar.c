/*PGR-GNU*****************************************************************
File: isPlanar.c

Generated with Template by:
Copyright (c) 2020 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2020 Himanshu Raj
Mail: raj.himanshu2@gmail.com


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

#include <stdbool.h>
#include "c_common/postgres_connection.h"

#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
#include "drivers/planar/isPlanar_driver.h"

PGDLLEXPORT Datum _pgr_isplanar(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_isplanar);

static bool
process(
    char *edges_sql
  ) {
    bool planarity = false;
    pgr_SPI_connect();
    char* log_msg = NULL;
    char* notice_msg = NULL;
    char* err_msg = NULL;

    clock_t start_t = clock();
    planarity = pgr_do_isPlanar(
        edges_sql,

        &log_msg,
        &notice_msg,
        &err_msg);
    time_msg(" processing pgr_isPlanar", start_t, clock());

    pgr_global_report(&log_msg, &notice_msg, &err_msg);

    pgr_SPI_finish();

    return planarity;
}

PGDLLEXPORT Datum _pgr_isplanar(PG_FUNCTION_ARGS) {
        PG_RETURN_BOOL(process(text_to_cstring(PG_GETARG_TEXT_P(0))));
}
