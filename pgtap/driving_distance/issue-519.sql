\i setup.sql

SELECT plan(3);

PREPARE q1 AS
SELECT 1 AS from_v, node, edge, cost, agg_cost FROM pgr_drivingDistance( 'select id, source, target, cost from edge_table',
    1, 3.5);

PREPARE q2 AS
SELECT 5 AS from_v, node, edge, cost, agg_cost  FROM pgr_drivingDistance( 'select id, source, target, cost from edge_table',
    5, 3.5);

PREPARE q3 AS
SELECT 25 AS from_v, node, edge, cost, agg_cost  FROM pgr_drivingDistance( 'select id, source, target, cost from edge_table',
    25, 3.5);

Prepare q4 AS
SELECT from_v, node, edge, cost, agg_cost FROM pgr_drivingDistance( 'select id, source, target, cost from edge_table',
    ARRAY[1, 5, 25], 3.5);



SELECT bag_has( 'q4', 'q1', '1: DD from [1, 5, 25] should have results of DD from 1');
SELECT bag_has( 'q4', 'q2', '2: DD from [1, 5, 25] should have results of DD from 5');
SELECT bag_has( 'q4', 'q3', '3: DD from [1, 5, 25] should have results of DD from 25');


-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;
