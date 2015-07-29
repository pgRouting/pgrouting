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

SELECT 7, from_v, edge, cost, agg_cost FROM pgr_drivingDistance(
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

