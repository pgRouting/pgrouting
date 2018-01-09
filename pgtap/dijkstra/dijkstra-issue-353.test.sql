\i setup.sql

SELECT plan(3);

PREPARE q1 AS
SELECT * FROM pgr_dijkstra( 'select id, source, target, cost from edge_table',
    array[2,7], array[5,6]);

PREPARE q2 AS
SELECT * FROM pgr_dijkstra( 'select id, source, target, cost from edge_table',
    array[2,7], array[17,18]);

Prepare q3 AS
(SELECT * FROM pgr_dijkstra( 'select id, source, target, cost from edge_table',
        array[2,7], array[5,6]))
UNION
(SELECT * FROM pgr_dijkstra( 'select id, source, target, cost from edge_table',
        array[2,7], array[17,18]));

PREPARE q4 AS
SELECT * FROM pgr_dijkstra( 'select id, source, target, cost from edge_table',
    array[2,7], array[5,6,17,18]);

SELECT is_empty( 'q2', '1');
SELECT set_eq( 'q1', 'q3', '2');
SELECT set_eq( 'q1', 'q4', '3');


-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;
