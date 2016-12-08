/*PGR-GNU*****************************************************************

Copyright (c) 2016 pgRouting developers
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

\echo --q1
SELECT seq, node, edge, round(cost::numeric, 6)  FROM
  pgr_bdDijkstra('SELECT * FROM bdd_ways', 5700, 6733, true);

\echo --q2
SELECT seq, node, edge, round(cost::numeric, 6)  FROM
  pgr_bdDijkstra('SELECT * FROM bdd_ways', 6585, 8247, true);

\echo --q3
SELECT seq, node, edge, round(cost::numeric, 6)  FROM
  pgr_bdDijkstra('SELECT * FROM bdd_ways', 9426, 3606, true);

\echo --q4
SELECT seq, node, edge, round(cost::numeric, 6)  FROM
  pgr_bdDijkstra('SELECT * FROM bdd_ways', 3606, 9426, true);

\echo --q5
SELECT seq, node, edge, round(cost::numeric, 6)  FROM
  pgr_bdDijkstra('SELECT * FROM bdd_ways', 6733, 5700, true);

\echo --q6
SELECT seq, node, edge, round(cost::numeric, 6)  FROM
  pgr_bdDijkstra('SELECT * FROM bdd_ways', 8247, 6585, true);

