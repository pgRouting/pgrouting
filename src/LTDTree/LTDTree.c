/*PGR-GNU*****************************************************************
File: LTDTree.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2020 Prakash Tiwari
Mail: 85prakash2017@gmail.com

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


/* for macro PGR_DBG */
#include "c_common/debug_macro.h"
/* for pgr_global_report */
#include "c_common/e_report.h"
/* for time_msg & clock */
#include "c_common/time_msg.h"
/* for functions to get edges informtion */
#include "c_common/edges_input.h"

#include "drivers/LTDTree/LTDTree_driver.h"  // the link to the C++ code of the function

PGDLLEXPORT Datum _pgr_LTDTree(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_LTDTree);


/******************************************************************************/
/*                          MODIFY AS NEEDED                                  */

