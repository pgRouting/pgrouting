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
\echo -- expected result = empty set because edge 2 cost < 0
SELECT * FROM pgr_dijkstraViaEdges(
    'SELECT id , source, target, cost FROM edge_table ',
    ARRAY[2,2]::integer[], ARRAY[0.25,0.75]::float8[], true);  

\echo -- expected result = empty set because edge 2 cost < 0
SELECT * FROM pgr_dijkstraViaEdges(
    'SELECT id , source, target, cost FROM edge_table ',
    ARRAY[2,2]::integer[], ARRAY[0.25,0.75]::float8[], false);  


\echo -- expected total agg-cost = 6.5
\echo -- vertices: -1 2 5 6 9 4 3 -2
\echo -- edges: 2 4 8 9 16 3 2 -1
 SELECT * FROM pgr_dijkstraViaEdges(
    'SELECT id , source, target, cost, reverse_cost FROM edge_table ',
    ARRAY[2,2]::integer[], ARRAY[0.25,0.75]::float8[], true);  

\echo -- expected total agg-cost = 0.5
SELECT * FROM pgr_dijkstraViaEdges(
    'SELECT id , source, target, cost FROM edge_table ',
    ARRAY[6,6]::integer[], ARRAY[0.25,0.75]::float8[], false);  

\echo -- expected total agg-cost = 0.5
SELECT * FROM pgr_dijkstraViaEdges(
    'SELECT id , source, target, cost FROM edge_table ',
    ARRAY[6,6]::integer[], ARRAY[0.25,0.75]::float8[], true);  

\echo -- expected total agg-cost = 3 
SELECT * FROM pgr_dijkstraViaEdges(
    'SELECT id , source, target, cost, reverse_cost FROM edge_table',
    ARRAY[6,10,4]::integer[], ARRAY[0,0,0]::float8[], false);


