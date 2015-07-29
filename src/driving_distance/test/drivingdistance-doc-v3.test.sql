------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              PGR_drivingDistance  V3
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------


SELECT node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        2, 3
      ) order by node;

SELECT node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        13, 3
      ) order by node;

SELECT node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        array[2,13], 3
      ) order by from_v, node;

SELECT node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        array[2,13], 3, equicost:=true
      ) order by from_v, node;
--------------------------------------------------------------
SELECT node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        2, 3, false
      ) order by node;

SELECT node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        13, 3, false
      ) order by node;

SELECT from_v, node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        array[2,13], 3, false
      ) order by from_v, node;

SELECT from_v, node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        array[2,13], 3, false, equicost:=true
      ) order by from_v, node;

---------------------------------------------------------------

SELECT node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        2, 3
      ) order by node;

SELECT node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        13, 3
      ) order by node;

SELECT from_v, node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        array[2,13], 3
      ) order by from_v, node;

SELECT from_v, node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        array[2,13], 3, equicost:=true
      ) order by from_v, node;
--------------------------------------------------------------
SELECT node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        2, 3, false
      ) order by node;

SELECT node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        13, 3, false
      ) order by node;

SELECT from_v, node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        array[2,13], 3, false
      ) order by from_v, node;

SELECT from_v, node, edge, cost, agg_cost FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        array[2,13], 3, false, equicost:=true
      ) order by from_v, node;

---------------------------------------------------------------

