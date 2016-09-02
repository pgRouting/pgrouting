/*PGR-GNU*****************************************************************
File: error_report.c

Function's developer:
Copyright (c) 2016 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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

#include <postgres.h>
#include "postgres_connection.h"
#include "debug_macro.h"
#include "error_report.h"

/**
 * Report notice & error to postgreSQL
 *
 *  The char* messages are cleared.
 *  New messages are made with palloc
 *
 *  when there is ERROR then postgreSQL takes over control
 *
 *  @warning Free all data not created with palloc before calling
 *  ~~~~{.c}
 *  if (err_msg) {
 *      pfree(<data>);
 *  }
 *  pgr_error_report(log_msg, notice_msg, error_msg);
 *  ~~~~
 */

void
pgr_error_report(
    char **log_msg,
    char **notice_msg,
    char **err_msg) {

    PGR_DBG("Returned log message = %s", *log_msg);
    PGR_DBG("Returned notice message = %s", *notice_msg);
    PGR_DBG("Returned error message = %s", *err_msg);

    char *notice = NULL;
    if (*notice_msg){
    notice = pgr_cstring2char(*notice_msg);
        free(*notice_msg);
    }

    char *error = NULL;
    error = pgr_cstring2char(*err_msg);
    if (*err_msg) {
    error = pgr_cstring2char(*err_msg);
        free(*err_msg);
    }

    char *log = NULL;
    log = pgr_cstring2char(*log_msg);
    if (*log_msg){
    notice = pgr_cstring2char(*notice_msg);
        free(*log_msg);
    }

    if (notice) {
        ereport(NOTICE,
                (errmsg("%s", notice),
                 errhint("%s", log)));
    }

    if (error) {
        ereport(ERROR,
                (errmsg_internal("%s", error),
                 errhint("%s", log)));
    }
}
