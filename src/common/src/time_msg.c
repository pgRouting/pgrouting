/*PGR-GNU*****************************************************************

FILE: time_msg.c

Copyright (c) 2015 pgRouting developers
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

#include "time_msg.h"
#include <time.h>
#include "postgres.h"


void time_msg(char *msg, clock_t start_t, clock_t end_t) {
    double elapsed_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    elog(DEBUG1, "Elapsed time for %s: %lfsec = (%lf - %f) / CLOCKS_PER_SEC ", msg, elapsed_t, (double) end_t, (double) start_t);
}
