/*PGR-GNU*****************************************************************
File: geom_text_rt.h

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

#ifndef INCLUDE_C_TYPES_GEOM_TEXT_RT_H_
#define INCLUDE_C_TYPES_GEOM_TEXT_RT_H_
#pragma once

typedef struct {
#if 0
    size_t pid;
    double x;
    double y;
#else
    char *geom;
#endif
} GeomText_t;

#endif  // INCLUDE_C_TYPES_GEOM_TEXT_RT_H_
