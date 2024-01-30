/*PGR-GNU*****************************************************************
File: trsp_pgget.h

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2023 Celia Virginia Vergara Castillo
mail: vicky at erosion.dev
Copyright (c) 2020 Mahmoud SAKR and Esteban ZIMANYI
mail: m_attia_sakrcw at yahoo.com, estebanzimanyicw at gmail.com
Copyright (c) 2016 Rohith Reddy
Copyright (c) 2016 Andrea Nardelli
mail: nrd.nardelli at gmail.com
Copyright (c) 2015 Celia Virginia Vergara Castillo
mail: vicky at erosion.dev

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
/** @file

This is a copy of pgdata_getters.h

The old version of TRSP
- Is the only function that has its own edge reader within its code,
  hence the name of the file.
- The old version of TRSP will exist during 3.x series


@note
- The only function that remains is `fetch_edge`
- The old version of TRSP will be removed on V4

 @todo: TODO V4 Delete this file
*/
#ifndef INCLUDE_C_COMMON_TRSP_PGGET_H_
#define INCLUDE_C_COMMON_TRSP_PGGET_H_
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <postgres.h>
#include <executor/spi.h>
#include <funcapi.h>
#include <utils/builtins.h>
#include <access/htup_details.h>
#include <fmgr.h>
#include <utils/array.h>
#include <catalog/pg_type.h>
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#else
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#endif

#include "cpp_common/undefPostgresDefine.hpp"

#ifdef __cplusplus
using Edge_t = struct Edge_t;
#else
typedef struct Edge_t Edge_t;
#endif


#ifdef __cplusplus
extern "C" {
#endif

/** @brief Read edges called from C file*/
void pgr_get_edges(char*, Edge_t**, size_t*, bool, bool, char**);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_C_COMMON_TRSP_PGGET_H_
