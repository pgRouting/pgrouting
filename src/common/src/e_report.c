/*PGR-GNU*****************************************************************
File: e_report.c

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

#include "c_common/postgres_connection.h"
#include "c_common/debug_macro.h"
#include "c_common/e_report.h"

void
pgr_notice(
        char* notice) {
    PGR_DBG("Returned notice message = %s", notice);

    if (notice) {
        ereport(NOTICE,
                (errmsg("%s", notice)));
    }
}

void
pgr_notice2(
        char* log,
        char* notice) {
    PGR_DBG("Returned log message = %s", log);
    PGR_DBG("Returned notice message = %s", notice);

    if (log) {
        pgr_notice(notice);
        return;
    }

    if (notice) {
        ereport(NOTICE,
                (errmsg("%s", notice),
                 errhint("%s", log)));
    }
}

void
pgr_error(char* err) {
    PGR_DBG("Returned error message = %s", err);

#if 0
    char *error = NULL;
    if (*err_msg) {
        error = pgr_cstring2char(*err_msg);
        free(*err_msg);
    }
#endif
    if (err) {
        ereport(ERROR,
                (errmsg_internal("Unexpected"),
                 errhint("%s", err)));
    }
}

void
pgr_error2(
        char* log,
        char* err) {
    PGR_DBG("Returned log message = %s", log);
    PGR_DBG("Returned error message = %s", err);

    if (err) {
        ereport(ERROR,
                (errmsg_internal("%s", err),
                 errhint("%s", log)));
    }
}

void
pgr_global_report(
        char* log,
        char* notice,
        char* err) {
    if (!notice && log) {
        ereport(DEBUG1,
                (errmsg_internal("%s", log)));
    }

    if (notice) {
        if (log) {
            ereport(NOTICE,
                    (errmsg_internal("%s", notice),
                     errhint("%s", log)));
        } else {
            ereport(NOTICE,
                    (errmsg_internal("%s", notice)));
        }
    }

    if (err) {
        if (log) {
            ereport(ERROR,
                    (errmsg_internal("%s", err),
                     errhint("%s", log)));
        } else {
            ereport(ERROR,
                    (errmsg_internal("%s", err)));
        }
    }
}
