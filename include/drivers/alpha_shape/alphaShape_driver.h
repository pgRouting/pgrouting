/*PGR-GNU*****************************************************************
FILE: alphaShape_driver.h

Copyright (c) 2018 Vicky Vergara
Mail: project@pgrouting.org

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

#ifndef INCLUDE_DRIVERS_ALPHA_SHAPE_ALPHASHAPE_DRIVER_H_
#define INCLUDE_DRIVERS_ALPHA_SHAPE_ALPHASHAPE_DRIVER_H_
#pragma once

/* for syze_t */
#include <stddef.h>
#include "c_types/pgr_edge_xy_t.h"
#include "c_types/geom_text_rt.h"


#ifdef __cplusplus
extern "C" {
#endif

    void do_alphaShape(
            Pgr_edge_xy_t *edgesArr,
            size_t edgesTotal,

            double alpha,

            GeomText_t **return_tuples,
            size_t *return_count,
            char **log_msg,
            char **notice_msg,

            char **err_msg);

#ifdef __cplusplus
}
#endif


#endif  // INCLUDE_DRIVERS_ALPHA_SHAPE_ALPHASHAPE_DRIVER_H_
