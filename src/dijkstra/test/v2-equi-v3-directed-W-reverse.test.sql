\set ECHO none
\set QUIET 1
-- Turn off echo and keep things quiet.

-- Format the output for nice TAP.
\pset format unaligned
\pset tuples_only true
\pset pager
\set VERBOSITY terse

-- Revert all changes on failure.
\set ON_ERROR_ROLLBACK true
\set ON_ERROR_STOP true
\set QUIET 1

BEGIN;
     SELECT plan(11);

-- all this queries are equivalent (give the same results)
PREPARE q00 AS
SELECT  id2 FROM pgr_dijkstra(
    'SELECT id AS id, source::int4, target::int4,
    CASE WHEN cost<=0 THEN 999 ELSE cost END AS cost,
    CASE WHEN reverse_cost<=0 THEN 999 ELSE reverse_cost END AS reverse_cost
    FROM edge_table ORDER BY id',
    11, 5, true, true);

PREPARE q0 AS
SELECT  id1 FROM pgr_dijkstra(
    'SELECT id AS id, source::int4, target::int4,
    CASE WHEN cost<=0 THEN 999 ELSE cost END AS cost,
    CASE WHEN reverse_cost<=0 THEN 999 ELSE reverse_cost END AS reverse_cost
    FROM edge_table ORDER BY id',
    11, 5, true, true);

PREPARE q1 AS
SELECT seq, id1, id2, cost FROM pgr_dijkstra(
    'SELECT id AS id, source::int4, target::int4,
    CASE WHEN cost<=0 THEN 999 ELSE cost END AS cost,
    CASE WHEN reverse_cost<=0 THEN 999 ELSE reverse_cost END AS reverse_cost
    FROM edge_table ORDER BY id',
    11, 5, true, true);

PREPARE q2 AS
SELECT seq - 1, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
    'SELECT id AS id, source::int4, target::int4, 
    CASE WHEN cost<=0 THEN 999 ELSE cost END AS cost, 
    CASE WHEN reverse_cost<=0 THEN 999 ELSE reverse_cost END AS reverse_cost 
    FROM edge_table ORDER BY id', 
    11, 5, true);


PREPARE q3 AS
SELECT seq - 1, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
    'SELECT id AS id, source::int4, target::int4, 
    CASE WHEN cost<=0 THEN 999 ELSE cost END AS cost, 
    CASE WHEN reverse_cost<=0 THEN 999 ELSE reverse_cost END AS reverse_cost 
    FROM edge_table ORDER BY id', 
    11, 5);

PREPARE q4 AS
SELECT seq - 1, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
        'SELECT id, source, target, 
        cost, 
        reverse_cost 
        FROM edge_table ORDER BY id', 
        11, 5);


PREPARE q5 AS
SELECT seq - 1, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
        'SELECT * 
        FROM edge_table ORDER BY id', 
        11, 5);

PREPARE q6 AS
SELECT seq - 1, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
        'SELECT * 
        FROM edge_table ORDER BY id', 
        ARRAY[11], 5);

PREPARE q7 AS
SELECT seq - 1, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
        'SELECT * 
        FROM edge_table ORDER BY id', 
        11, ARRAY[5]);

PREPARE q8 AS
SELECT seq - 1, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
        'SELECT * 
        FROM edge_table ORDER BY id', 
        ARRAY[11], ARRAY[5]);

PREPARE q9 AS
SELECT seq, id1, id2, cost FROM pgr_dijkstra(
    'SELECT id AS id, source::int4, target::int4,
    cost, reverse_cost
    FROM edge_table ORDER BY id',
    11, 5, true, true);



    SELECT set_eq( 'q0', ARRAY[11, 12, 9, 6, 5], '1');
    SELECT set_eq( 'q00', ARRAY[13, 15, 9, 8, -1], '2');
    SELECT set_eq( 'q1', 'q2', '3');
    SELECT set_eq( 'q1', 'q3', '4');
    SELECT set_eq( 'q1', 'q4', '5');
    SELECT set_eq( 'q1', 'q5', '6');
    SELECT set_eq( 'q1', 'q6', '7');
    SELECT set_eq( 'q1', 'q7', '8');
    SELECT set_eq( 'q1', 'q8', '9');
    SELECT set_eq( 'q1', 'q9', '10');

PREPARE q50 AS
SELECT seq, id1, id2, cost FROM pgr_dijkstra(
    'SELECT id AS id, source::BIGINT, target::BIGINT,
    CASE WHEN cost<=0 THEN 999 ELSE cost END AS cost,
    CASE WHEN reverse_cost<=0 THEN 999 ELSE reverse_cost END AS reverse_cost
    FROM edge_table ORDER BY id',
    11, 5, true, true);

SELECT throws_ok('q50',
    'P0001',
    'Support for id, source, target columns only of type: integer. Support for Cost: double precision',
    '11');

    -- Finish the tests and clean up.
    SELECT * FROM finish();
    ROLLBACK;

