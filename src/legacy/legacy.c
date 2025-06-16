/*PGR-GNU*****************************************************************
File: legacy.c

Copyright (c) 2017 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2018-2020 Regina Obe <lr@pcorp.us>
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

/******************************************************************************
 * This file is to hold functions we no longer use,
 * but we need to keep because they were used at one time behind SQL API functions.
 * This is to ease pg_upgrade upgrades
 *
 * All functions in this file should throw an error telling the user to upgrade
 * the install
 *
 *****************************************************************************/

#include "postgres.h"
#include "utils/builtins.h"

#define PGROUTING_DEPRECATE(version, funcname) \
    Datum funcname(PG_FUNCTION_ARGS); \
    PG_FUNCTION_INFO_V1(funcname); \
    Datum funcname(__attribute__((unused)) PG_FUNCTION_ARGS) \
    { \
        ereport(ERROR, (\
            errcode(ERRCODE_FEATURE_NOT_SUPPORTED), \
            errmsg("A stored procedure tried to use deprecated C internal function '%s'", \
                   __func__), \
            errdetail("Library function '%s' was deprecated in pgRouting %s", \
                      __func__, version), \
            errhint("Consider upgrade pgRouting"))); \
        PG_RETURN_POINTER(NULL); \
    }

PGROUTING_DEPRECATE("4.0.0", _pgr_trsp)
PGROUTING_DEPRECATE("4.0.0", _v4trsp)
PGROUTING_DEPRECATE("4.0.0", _trsp)
PGROUTING_DEPRECATE("4.0.0", _pgr_alphashape)
PGROUTING_DEPRECATE("4.0.0", _pgr_withpointsdd)
PGROUTING_DEPRECATE("4.0.0", _pgr_dagshortestpath)
