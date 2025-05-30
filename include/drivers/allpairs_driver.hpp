/*PGR-GNU*****************************************************************
File: allpairs_driver.hpp

Copyright (c) 2025 pgRouting developers
Mail: project@pgrouting.org

Design of one process & driver file by
Copyright (c) 2025 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

Copying this file (or a derivative) within pgRouting code add the following:

Generated with Template by:
Copyright (c) 2025 pgRouting developers
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

#ifndef INCLUDE_DRIVERS_ALLPAIRS_DRIVER_HPP_
#define INCLUDE_DRIVERS_ALLPAIRS_DRIVER_HPP_
#pragma once


#include <cstddef>
#include <string>
using IID_t_rt = struct IID_t_rt;

void
do_allpairs(
    std::string,
    bool, int,

    IID_t_rt**, size_t*,
    char**, char**);

#endif  // INCLUDE_DRIVERS_ALLPAIRS_DRIVER_HPP_
