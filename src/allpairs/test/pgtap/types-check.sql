
\i setup.sql


SELECT plan(17);

PREPARE q1 AS
SELECT * FROM pgr_pgr_floydWarshall(
    'SELECT id::BIGINT, source::SMALLINT, target::BIGINT, cost::INTEGER, reverse_cost::SMALLINT FROM edge_table'
);

PREPARE q2 AS
SELECT * FROM pgr_pgr_floydWarshall(
    'SELECT source, target, cost, reverse_cost FROM edge_table',
    true
);

PREPARE q3 AS
SELECT * FROM pgr_pgr_floydWarshall(
    'SELECT source, target, cost, reverse_cost FROM edge_table',
    false
);

PREPARE q4 AS
SELECT * FROM pgr_pgr_floydWarshall(
    'SELECT source, target, cost, reverse_cost FROM edge_table',
);


SELECT lives_ok('q1', 'edges query accepts ANY-INTEGER & ANY NUMERICAL');
SELECT lives_ok('q2', 'without id it works');
SELECT lives_ok('q3', 'directed flag works ok with false');
SELECT lives_ok('q4', 'directed flag works ok with true');


PREPARE q10 AS
SELECT * FROM pgr_pgr_floydWarshall(
    'SELECT id::FLOAT, source, target, cost, reverse_cost FROM edge_table',
    true);

PREPARE q11 AS
SELECT * FROM pgr_pgr_floydWarshall(
    'SELECT id::REAL, source, target, cost, reverse_cost FROM edge_table',
    true);

SELECT throws_ok('q10', 'XX000', 'Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'Throws because id is FLOAT');
SELECT throws_ok('q11', 'XX000', 'Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'Throws because id is REAL');

PREPARE q12 AS
SELECT * FROM pgr_pgr_floydWarshall(
    'SELECT id, source::FLOAT, target, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]);

PREPARE q13 AS
SELECT * FROM pgr_pgr_floydWarshall(
    'SELECT id, source::REAL, target, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]);

SELECT throws_ok('q12', 'XX000', 'Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'Throws because source is FLOAT');
SELECT throws_ok('q13', 'XX000', 'Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'Throws because source is REAL');

PREPARE q14 AS
SELECT * FROM pgr_pgr_floydWarshall(
    'SELECT id, source, target::FLOAT, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]);

PREPARE q15 AS
SELECT * FROM pgr_pgr_floydWarshall(
    'SELECT id, source, target::REAL, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]);

SELECT throws_ok('q14', 'XX000', 'Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'Throws because source is FLOAT');
SELECT throws_ok('q15', 'XX000', 'Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'Throws because source is REAL');


SELECT throws_ok('
SELECT * FROM pgr_pgr_floydWarshall(
    ''SELECT id, source, target, cost, reverse_cost FROM edge_table'',
    2::FLOAT, ARRAY[5,3]);',
    '42883', 'function pgr_funnydijkstra(unknown, double precision, integer[]) does not exist',
    'Throws because start_vid is FLOAT');

SELECT throws_ok('
SELECT * FROM pgr_pgr_floydWarshall(
    ''SELECT id, source, target, cost, reverse_cost FROM edge_table'',
    2::REAL, ARRAY[5,3]);',
    '42883', 'function pgr_funnydijkstra(unknown, real, integer[]) does not exist',
    'Throws because start_vid is REAL');


SELECT throws_ok('
SELECT * FROM pgr_pgr_floydWarshall(
    ''SELECT id, source, target, cost, reverse_cost FROM edge_table'',
    2, ARRAY[5,3]::FLOAT[]);',
    'XX000','Expected array of ANY-INTEGER',
    'Throws because end_vids array is of FLOAT');

SELECT throws_ok('
SELECT * FROM pgr_pgr_floydWarshall(
    ''SELECT id, source, target, cost, reverse_cost FROM edge_table'',
    2, ARRAY[5,3]::REAL[]);',
    'XX000','Expected array of ANY-INTEGER',
    'Throws because end_vids array is of REAL');





SELECT finish();
ROLLBACK;
