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

\echo --Examples for queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns
\echo ----------------------------------------------------------------------------------------

        SELECT * FROM pgr_dijkstraViaVertices(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[1, 3, 11]
                );
\echo ------

        SELECT * FROM pgr_dijkstraViaVertices(
                  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                  array[1, 3, 11],
                  true
                );



\echo -- Examples for queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns
\echo ----------------------------------------------------------------------------------------


        SELECT * FROM pgr_dijkstraViaVertices(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[1, 3, 11],
			 false
                );



\echo -- Examples for queries marked as ``directed`` with ``cost`` column
\echo ----------------------------------------------------------------------------------------

        SELECT * FROM pgr_dijkstraViaVertices(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[1, 3, 11]
                );
\echo ------

        SELECT * FROM pgr_dijkstraViaVertices(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[1, 3, 11],
			 true
                );




\echo -- Examples for queries marked as ``undirected`` with ``cost`` column
\echo ----------------------------------------------------------------------------------------

        SELECT * FROM pgr_dijkstraViaVertices(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[1, 3, 11],
                        false
		);

\echo -- Equvalences between pgr_dijkstraViaVertices and pgr_dijkstra
\echo ------------------------------------------------------------

        SELECT * FROM pgr_dijkstra(
               'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                array[1], array[3]
        );
\echo ------
        SELECT * FROM pgr_dijkstra(
               'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                array[3], array[11]
        );
\echo ------
        SELECT * FROM pgr_dijkstra(
               'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                array[11], array[1]
        );
\echo ------
        SELECT * FROM pgr_dijkstraViaVertices(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[1, 3, 11, 1],
                        false
        );

