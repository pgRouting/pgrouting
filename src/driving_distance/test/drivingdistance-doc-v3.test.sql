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
--              PGR_drivingDistance  V3
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------


SELECT 1, node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        2, 3
      ) order by node;

SELECT 2, node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        13, 3
      ) order by node;

SELECT 3, node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        array[2,13], 3
      ) order by from_v, node;

SELECT 4, node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        array[2,13], 3, equicost:=true
      ) order by from_v, node;
--------------------------------------------------------------
SELECT 5, node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        2, 3, false
      ) order by node;

SELECT 6, node, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        13, 3, false
      ) order by node;

SELECT 7, from_v, node, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        array[2,13], 3, false
      ) order by from_v, node;

SELECT 8, from_v, node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        array[2,13], 3, false, equicost:=true
      ) order by from_v, node;

---------------------------------------------------------------

SELECT 9, node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        2, 3
      ) order by node;

SELECT 10, node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        13, 3
      ) order by node;

SELECT 11, from_v, node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        array[2,13], 3
      ) order by from_v, node;

SELECT 12, from_v, node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        array[2,13], 3, equicost:=true
      ) order by from_v, node;
--------------------------------------------------------------
SELECT 13, node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        2, 3, false
      ) order by node;

SELECT 14, node, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        13, 3, false
      ) order by node;

SELECT 15, from_v, node, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        array[2,13], 3, false
      ) order by from_v, node;

SELECT 16, from_v, node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        array[2,13], 3, false, equicost:=true
      ) order by from_v, node;

---------------------------------------------------------------

