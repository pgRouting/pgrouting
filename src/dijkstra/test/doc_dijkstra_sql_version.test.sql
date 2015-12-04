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

\echo -- q1
SELECT * from pgr_dijkstra(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    1,
    'SELECT id from edge_table_vertices_pgr where id > 3 and id < 6');

\echo -- q2

SELECT * from pgr_dijkstra(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT id from edge_table_vertices_pgr where id > 3 and id < 6',
    1);

\echo -- q3

SELECT * from pgr_dijkstra(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT id from edge_table_vertices_pgr where id < 3',
    'SELECT id from edge_table_vertices_pgr where id > 3 and id < 6'
    );
