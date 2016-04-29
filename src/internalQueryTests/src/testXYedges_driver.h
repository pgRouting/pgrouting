/*PGR-GNU*****************************************************************
File: testXYedges_driver.h

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

#ifndef SRC_TESTXYEDGES_SRC_TESTXYEDGES_DRIVER_H_
#define SRC_TESTXYEDGES_SRC_TESTXYEDGES_DRIVER_H_

#include "./../../common/src/pgr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

    /*********************************************************
      edges_sql TEXT
     ********************************************************/
    bool do_pgr_testXYedges(
            Pgr_edge_xy_t *edges,
            size_t total_edges,
            char ** log_msg,
            char ** err_msg);


#ifdef __cplusplus
}
#endif

#endif  // SRC_TESTXYEDGES_SRC_TESTXYEDGES_DRIVER_H_
