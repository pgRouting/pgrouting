\i setup.sql
SET client_min_messages = WARNING;


SELECT plan(9);

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;


-- all this queries are equivalent (give the same results)
PREPARE q00 AS
SELECT  id2 FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER,
    CASE WHEN cost<=0 THEN 999 ELSE cost END AS cost
    FROM edge_table ORDER BY id',
    7, 11, false, false);

PREPARE q0 AS
SELECT  id1 FROM pgr_dijkstra(
    'SELECT  id::INTEGER, source::INTEGER, target::INTEGER,
    CASE WHEN cost<=0 THEN 999 ELSE cost END AS cost
    FROM edge_table ORDER BY id',
    7, 11, false, false);

PREPARE q1 AS
SELECT seq, id1, id2, cost FROM pgr_dijkstra(
    'SELECT  id::INTEGER, source::INTEGER, target::INTEGER,
    CASE WHEN cost<=0 THEN 999 ELSE cost END AS cost
    FROM edge_table ORDER BY id',
    7, 11, false, false);

PREPARE q2 AS
SELECT seq - 1, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
    'SELECT  id, source, target,
    CASE WHEN cost<=0 THEN 999 ELSE cost END AS cost
    FROM edge_table ORDER BY id',
    7, 11, false);


PREPARE q3 AS
SELECT seq - 1, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
    'SELECT id, source, target,
    CASE WHEN cost<=0 THEN 999 ELSE cost END AS cost
    FROM edge_table ORDER BY id',
    7, 11, false);

PREPARE q4 AS
SELECT seq - 1, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
        'SELECT id, source, target, cost
        FROM edge_table ORDER BY id',
        7, 11, false);


PREPARE q5 AS
SELECT seq - 1, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
        'SELECT id, source, target, cost,
        -1 as reverse_cost
        FROM edge_table ORDER BY id',
        7, 11, false);

PREPARE q6 AS
SELECT seq - 1, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
        'SELECT id, source, target, cost
        FROM edge_table ORDER BY id',
        ARRAY[7], 11, false);

PREPARE q7 AS
SELECT seq - 1, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
        'SELECT id, source, target, cost
        FROM edge_table ORDER BY id',
        7, ARRAY[11], false);

PREPARE q8 AS
SELECT seq - 1, node::INTEGER, edge::INTEGER, cost FROM pgr_dijkstra(
        'SELECT id, source, target, cost
        FROM edge_table ORDER BY id',
        ARRAY[7], ARRAY[11], false);

PREPARE q9 AS
SELECT seq, id1, id2, cost FROM pgr_dijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER,
    cost
    FROM edge_table ORDER BY id',
    7, 11, false, false);



    -- SELECT set_eq( 'q0', ARRAY[7, 8, 5, 10, 11], '1');
    -- SELECT set_eq( 'q00', ARRAY[6, 7, 10, 12, -1], '2');
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
    'SELECT  id::INTEGER, source::BIGINT, target::BIGINT,
    CASE WHEN cost<=0 THEN 999 ELSE cost END AS cost
    FROM edge_table ORDER BY id',
    7, 11, false, false);

SELECT throws_ok('q50',
    'XX000',
    'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    '11');

    -- Finish the tests and clean up.
    SELECT * FROM finish();
    ROLLBACK;

