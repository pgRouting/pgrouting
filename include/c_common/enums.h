/*PGR-GNU*****************************************************************
File: ordering_process.h

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

#ifndef INCLUDE_PROCESS_ORDERING_ENUMS_H_
#define INCLUDE_PROCESS_ORDERING_ENUMS_H_

enum Which {
    /** undirected graph + results: vertex id  */
    SLOAN=0, CUTCHILL, KING,
    /** directed graph + results: vertex id  */
    TOPOSORT=11
};

#endif  // INCLUDE_PROCESS_ORDERING_ENUMS_H_
