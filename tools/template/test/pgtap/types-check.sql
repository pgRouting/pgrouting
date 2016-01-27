
\i setup.sql


SELECT plan(17);

PREPARE q1 AS
SELECT * FROM pgr_MY_FUNCTION_NAME(
    'SELECT id::BIGINT, source::SMALLINT, target::BIGINT, cost::INTEGER, reverse_cost::SMALLINT FROM edge_table',
    2, ARRAY[5,3]);

PREPARE q2 AS
SELECT * FROM pgr_MY_FUNCTION_NAME(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2::BIGINT, ARRAY[5,3]);

PREPARE q3 AS
SELECT * FROM pgr_MY_FUNCTION_NAME(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2::SMALLINT, ARRAY[5,3]);

PREPARE q4 AS
SELECT * FROM pgr_MY_FUNCTION_NAME(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]::INTEGER[]);

PREPARE q5 AS
SELECT * FROM pgr_MY_FUNCTION_NAME(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]::SMALLINT[]);

PREPARE q6 AS
SELECT * FROM pgr_MY_FUNCTION_NAME(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]::BIGINT[]);

SELECT lives_ok('q1', 'edges query accepts ANY-INTEGER & ANY NUMERICAL');
SELECT lives_ok('q2', 'start_vid accepts BIGINT');
SELECT lives_ok('q3', 'start_vid accepts SMALLINT');
SELECT lives_ok('q4', 'end_vids accepts array of INTEGER');
SELECT lives_ok('q4', 'end_vids accepts array of SMALLINT');
SELECT lives_ok('q5', 'end_vids accepts array of BIGINT');
SELECT lives_ok('q6', 'end_vids accepts array of BIGINT');


PREPARE q10 AS
SELECT * FROM pgr_MY_FUNCTION_NAME(
    'SELECT id::FLOAT, source, target, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]);

PREPARE q11 AS
SELECT * FROM pgr_MY_FUNCTION_NAME(
    'SELECT id::REAL, source, target, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]);

SELECT throws_ok('q10', 'XX000', 'Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'Throws because id is FLOAT');
SELECT throws_ok('q11', 'XX000', 'Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'Throws because id is REAL');

PREPARE q12 AS
SELECT * FROM pgr_MY_FUNCTION_NAME(
    'SELECT id, source::FLOAT, target, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]);

PREPARE q13 AS
SELECT * FROM pgr_MY_FUNCTION_NAME(
    'SELECT id, source::REAL, target, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]);

SELECT throws_ok('q12', 'XX000', 'Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'Throws because source is FLOAT');
SELECT throws_ok('q13', 'XX000', 'Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'Throws because source is REAL');

PREPARE q14 AS
SELECT * FROM pgr_MY_FUNCTION_NAME(
    'SELECT id, source, target::FLOAT, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]);

PREPARE q15 AS
SELECT * FROM pgr_MY_FUNCTION_NAME(
    'SELECT id, source, target::REAL, cost, reverse_cost FROM edge_table',
    2, ARRAY[5,3]);

SELECT throws_ok('q14', 'XX000', 'Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'Throws because source is FLOAT');
SELECT throws_ok('q15', 'XX000', 'Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'Throws because source is REAL');


SELECT throws_ok('
SELECT * FROM pgr_MY_FUNCTION_NAME(
    ''SELECT id, source, target, cost, reverse_cost FROM edge_table'',
    2::FLOAT, ARRAY[5,3]);',
    '42883', 'function pgr_MY_FUNCTION_NAME(unknown, double precision, integer[]) does not exist',
    'Throws because start_vid is FLOAT');

SELECT throws_ok('
SELECT * FROM pgr_MY_FUNCTION_NAME(
    ''SELECT id, source, target, cost, reverse_cost FROM edge_table'',
    2::REAL, ARRAY[5,3]);',
    '42883', 'function pgr_MY_FUNCTION_NAME(unknown, real, integer[]) does not exist',
    'Throws because start_vid is REAL');


SELECT throws_ok('
SELECT * FROM pgr_MY_FUNCTION_NAME(
    ''SELECT id, source, target, cost, reverse_cost FROM edge_table'',
    2, ARRAY[5,3]::FLOAT[]);',
    'XX000','Expected array of ANY-INTEGER',
    'Throws because end_vids array is of FLOAT');

SELECT throws_ok('
SELECT * FROM pgr_MY_FUNCTION_NAME(
    ''SELECT id, source, target, cost, reverse_cost FROM edge_table'',
    2, ARRAY[5,3]::REAL[]);',
    'XX000','Expected array of ANY-INTEGER',
    'Throws because end_vids array is of REAL');





SELECT finish();
ROLLBACK;
