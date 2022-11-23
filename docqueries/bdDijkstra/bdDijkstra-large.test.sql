-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
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

/* TODO move to pgtap */

/* --q1 */
SELECT seq, node, edge, round(cost::numeric, 6)  FROM
  pgr_bdDijkstra('SELECT * FROM bdd_ways', 5700, 6733, true);

/* --q2 */
SELECT seq, node, edge, round(cost::numeric, 6)  FROM
  pgr_bdDijkstra('SELECT * FROM bdd_ways', 6585, 8247, true);

/* --q3 */
SELECT seq, node, edge, round(cost::numeric, 6)  FROM
  pgr_bdDijkstra('SELECT * FROM bdd_ways', 9426, 3606, true);

/* --q4 */
SELECT seq, node, edge, round(cost::numeric, 6)  FROM
  pgr_bdDijkstra('SELECT * FROM bdd_ways', 3606, 9426, true);

/* --q5 */
SELECT seq, node, edge, round(cost::numeric, 6)  FROM
  pgr_bdDijkstra('SELECT * FROM bdd_ways', 6733, 5700, true);

/* --q6 */
SELECT seq, node, edge, round(cost::numeric, 6)  FROM
  pgr_bdDijkstra('SELECT * FROM bdd_ways', 8247, 6585, true);

