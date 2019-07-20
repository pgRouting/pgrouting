/*PGR-GNU*****************************************************************

File: debug_macro.h

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

#ifndef INCLUDE_C_COMMON_DEBUG_MACRO_H_
#define INCLUDE_C_COMMON_DEBUG_MACRO_H_
#pragma once

#ifndef NDEBUG

#include "c_common/postgres_connection.h"

#define PGR_DBG(...) \
    elog(DEBUG3, __VA_ARGS__)
#else
#define PGR_DBG(...) do { ; } while (0)
#endif

#endif  // INCLUDE_C_COMMON_DEBUG_MACRO_H_
