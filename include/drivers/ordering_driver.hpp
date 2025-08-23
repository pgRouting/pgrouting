/*PGR-GNU*****************************************************************
File: ordering_driver.hpp

Generated with Template by:
Copyright (c) 2025 pgRouting developers
Mail: project@pgrouting.org

Developer:
Copyright (c) 2025 Fan Wu
Mail: wifiblack0131 at gmail.com

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

#ifndef INCLUDE_DRIVERS_ORDERING_DRIVER_HPP_
#define INCLUDE_DRIVERS_ORDERING_DRIVER_HPP_
#pragma once

#include <cstddef>
#include <string>


void
do_ordering(
        std::string, int,
        int64_t**,  size_t*,
        char **, char **, char **);


#endif  // INCLUDE_DRIVERS_ORDERING_DRIVER_HPP_
