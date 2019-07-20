/*PGR-GNU*****************************************************************
File: astarOneToOne.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail:

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

#include "c_common/check_parameters.h"


void
check_parameters(
        int heuristic,
        double factor,
        double epsilon) {
    if (heuristic > 5 || heuristic < 0) {
        ereport(ERROR,
                (errmsg("Unknown heuristic"),
                 errhint("Valid values: 0~5")));
    }
    if (factor <= 0) {
        ereport(ERROR,
                (errmsg("Factor value out of range"),
                 errhint("Valid values: positive non zero")));
    }
    if (epsilon < 1) {
        ereport(ERROR,
                (errmsg("Epsilon value out of range"),
                 errhint("Valid values: 1 or greater than 1")));
    }
}

