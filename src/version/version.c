/*PGR-GNU*****************************************************************

File: version.c

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
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

#define UNUSED(x) (void)(x)

PGDLLEXPORT Datum pgr_version(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(pgr_version);

/** Returns v3.0.0
 * Function called only on v3.0.0
 * Deprecated C on v3.0.1
 */
PGDLLEXPORT Datum pgr_version(PG_FUNCTION_ARGS) {
    UNUSED(fcinfo);
    char *ver = "3.0.0";
    text *result = cstring_to_text(ver);
    PG_RETURN_TEXT_P(result);
}
