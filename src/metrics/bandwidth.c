/*PGR-GNU*****************************************************************
File: bandwidth.c

Generated with Template by:
Copyright (c) 2015-2026 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2025 Saloni Kumari
Mail: chaudharysaloni2510 at gmail.com

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
#include "process/metrics_process.h"

PGDLLEXPORT Datum _pgr_bandwidth(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_bandwidth);


PGDLLEXPORT Datum
_pgr_bandwidth(PG_FUNCTION_ARGS) {
    uint64_t result = pgr_process_metrics(
        text_to_cstring(PG_GETARG_TEXT_P(0)),
        BANDWIDTH);
    PG_RETURN_UINT64(result);
}
