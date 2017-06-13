/*PGR-GNU*****************************************************************
File: restrict_input.h

Copyright (c) 2017 Celia Virginia Vergara Castillo
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
/*! @file */


#ifndef INCLUDE_C_COMMON_RESTRICT_INPUT_H_
#define INCLUDE_C_COMMON_RESTRICT_INPUT_H_
#pragma once

#include "c_types/restrict_t.h"

void pgr_get_restriction_data(
        char *restrictions_sql,
        Restrict_t **restrictions,
        size_t *total_restrictions);

#endif  // INCLUDE_C_COMMON_RESTRICT_INPUT_H_
