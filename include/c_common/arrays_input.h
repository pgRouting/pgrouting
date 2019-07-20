/*PGR-GNU*****************************************************************
File: arrays_input.h

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

#ifndef INCLUDE_C_COMMON_ARRAYS_INPUT_H_
#define INCLUDE_C_COMMON_ARRAYS_INPUT_H_
#pragma once


#include <stdint.h>

#ifdef __GNUC__
#   if __GNUC__ > 5
#       pragma GCC diagnostic ignored "-Wpedantic"
#   else
#       pragma GCC diagnostic ignored "-pedantic"
#   endif
#endif

#include <postgres.h>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

// for ArrayType
#include "utils/array.h"


/** @brief enforces the input array to be @b NOT empty */
int64_t* pgr_get_bigIntArray(size_t *arrlen, ArrayType *input);

/** @brief Allows the input array to be empty */
int64_t* pgr_get_bigIntArray_allowEmpty(size_t *arrlen, ArrayType *input);

#endif  // INCLUDE_C_COMMON_ARRAYS_INPUT_H_
