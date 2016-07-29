/*PGR-GNU*****************************************************************
File: restrictions_input.h

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

#ifndef SRC_COMMON_SRC_RESTRICTIONS_INPUT_H_
#define SRC_COMMON_SRC_RESTRICTIONS_INPUT_H_

#pragma once

#include "./pgr_types.h"

void pgr_get_restriction_data(
        char *restrictions_sql,
        Restrict_t **restructions,
        size_t *total_restrictions);

#endif  // SRC_COMMON_SRC_RESTRICTIONS_INPUT_H_
