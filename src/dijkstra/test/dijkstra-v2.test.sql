/*PGR-GNU*****************************************************************

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
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              PGR_dijkstra  V.2.0
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
SET client_min_messages TO notice;

\echo --q1
SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    2,3, true, false);
\echo --q2

SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
    2,3, true, true);
\echo --q3

SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
     2, 3, false, false);
\echo --q4

SELECT * FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
     2, 3, false, true);
\echo --q5
