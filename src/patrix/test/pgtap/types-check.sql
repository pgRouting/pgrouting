
\i setup.sql

SELECT plan(19);

SELECT has_function('pgr_patrix', ARRAY['text','bigint','anyarray','boolean']);
SELECT function_returns('pgr_patrix', ARRAY['text','bigint','anyarray','boolean'],'setof record');

PREPARE v21q01 AS
SELECT  'integer'::text AS t1,'integer'::text AS t2,
'bigint'::text AS t5, 'bigint'::text AS t6,
'double precision'::text AS t7, 'double precision'::text AS t8;

PREPARE v21q10 AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(path_seq)::text AS t2,
pg_typeof(end_vid)::text AS t4,
pg_typeof(node)::text AS t5, pg_typeof(edge)::text AS t6,
pg_typeof(cost)::text AS t7, pg_typeof(agg_cost)::TEXT AS t8 
FROM ( 
    SELECT * FROM pgr_patrix(
        'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table',
        2, ARRAY[3], true) ) AS a
limit 1;

SELECT set_eq('v21q01', 'v21q10','pgr_patrix 1 to many: SHOULD RETURN expected columns names & types');

PREPARE q1 AS
SELECT * FROM pgr_patrix(
    'SELECT id::BIGINT, source::SMALLINT, target::BIGINT, cost::INTEGER, reverse_cost::SMALLINT FROM edge_table',
    2, ARRAY[5,3]);

PREPARE q2 AS
SELECT * FROM pgr_patrix(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2::BIGINT, ARRAY[5,3]);

PREPARE q3 AS
SELECT * FROM pgr_patrix(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2::SMALLINT, ARRAY[5,3]);

PREPARE q4 AS
SELECT * FROM pgr_patrix(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]::INTEGER[]);

PREPARE q5 AS
SELECT * FROM pgr_patrix(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]::SMALLINT[]);

PREPARE q6 AS
SELECT * FROM pgr_patrix(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]::BIGINT[]);

SELECT lives_ok('q1', 'SHOULD LIVE because edges query accepts ANY-INTEGER & ANY NUMERICAL');
SELECT lives_ok('q2', 'SHOULD LIVE because start_vid accepts BIGINT');
SELECT lives_ok('q3', 'SHOULD LIVE because start_vid accepts SMALLINT');
SELECT lives_ok('q4', 'SHOULD LIVE because end_vids accepts array of INTEGER');
SELECT lives_ok('q5', 'SHOULD LIVE because end_vids accepts array of SMALLINT');
SELECT lives_ok('q6', 'SHOULD LIVE because end_vids accepts array of BIGINT');


PREPARE q10 AS
SELECT * FROM pgr_patrix(
    'SELECT id::FLOAT, source, target, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]);

PREPARE q11 AS
SELECT * FROM pgr_patrix(
    'SELECT id::REAL, source, target, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]);

SELECT throws_ok('q10', 'XX000', 'Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'SHOULD THROW because id is FLOAT');
SELECT throws_ok('q11', 'XX000', 'Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'SHOULD THROW because id is REAL');

PREPARE q12 AS
SELECT * FROM pgr_patrix(
    'SELECT id, source::FLOAT, target, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]);

PREPARE q13 AS
SELECT * FROM pgr_patrix(
    'SELECT id, source::REAL, target, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]);

SELECT throws_ok('q12', 'XX000', 'Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'SHOULD THROW because source is FLOAT');
SELECT throws_ok('q13', 'XX000', 'Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'SHOULD THROW because source is REAL');

PREPARE q14 AS
SELECT * FROM pgr_patrix(
    'SELECT id, source, target::FLOAT, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]);

PREPARE q15 AS
SELECT * FROM pgr_patrix(
    'SELECT id, source, target::REAL, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]);

SELECT throws_ok('q14', 'XX000', 'Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'SHOULD THROW because source is FLOAT');
SELECT throws_ok('q15', 'XX000', 'Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'SHOULD THROW because source is REAL');


SELECT throws_ok('
    SELECT * FROM pgr_patrix(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table'',
        2::FLOAT, ARRAY[5,3]);',
    '42883', 'function pgr_patrix(unknown, double precision, integer[]) does not exist',
    'SHOULD THROW because start_vid is FLOAT');

SELECT throws_ok('
    SELECT * FROM pgr_patrix(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table'',
        2::REAL, ARRAY[5,3]);',
    '42883', 'function pgr_patrix(unknown, real, integer[]) does not exist',
    'SHOULD THROW because start_vid is REAL');


SELECT throws_ok('
    SELECT * FROM pgr_patrix(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table'',
        2, ARRAY[5,3]::FLOAT[]);',
    'XX000','Expected array of ANY-INTEGER',
    'SHOULD THROW because end_vids array is of FLOAT');

SELECT throws_ok('
    SELECT * FROM pgr_patrix(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table'',
        2, ARRAY[5,3]::REAL[]);',
    'XX000','Expected array of ANY-INTEGER',
    'SHOULD THROW because end_vids array is of REAL');





SELECT finish();
ROLLBACK;
