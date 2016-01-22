
\i setup.sql


SELECT plan(10);

PREPARE q1 AS
SELECT * FROM pgr_johnson(
    'SELECT id::BIGINT, source::SMALLINT, target::BIGINT, cost::INTEGER, reverse_cost::SMALLINT FROM edge_table'
);

PREPARE q2 AS
SELECT * FROM pgr_johnson(
    'SELECT source, target, cost, reverse_cost FROM edge_table'
);

PREPARE q3 AS
SELECT * FROM pgr_johnson(
    'SELECT source, target, cost, reverse_cost FROM edge_table',
    false
);

PREPARE q4 AS
SELECT * FROM pgr_johnson(
    'SELECT source, target, cost, reverse_cost FROM edge_table',
    true
);


SELECT lives_ok('q1', 'edges query accepts ANY-INTEGER & ANY NUMERICAL');
SELECT lives_ok('q2', 'without id it works');
SELECT lives_ok('q3', 'directed flag works ok with false');
SELECT lives_ok('q4', 'directed flag works ok with true');


PREPARE q10 AS
SELECT * FROM pgr_johnson(
    'SELECT id::FLOAT, source, target, cost, reverse_cost FROM edge_table',
    true);

PREPARE q11 AS
SELECT * FROM pgr_johnson(
    'SELECT id::REAL, source, target, cost, reverse_cost FROM edge_table',
    true);

SELECT throws_ok('q10', 'XX000', 'Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'Throws because id is FLOAT');
SELECT throws_ok('q11', 'XX000', 'Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'Throws because id is REAL');

PREPARE q12 AS
SELECT * FROM pgr_johnson(
    'SELECT id, source::FLOAT, target, cost, reverse_cost FROM edge_table',
    true);

PREPARE q13 AS
SELECT * FROM pgr_johnson(
    'SELECT id, source::REAL, target, cost, reverse_cost FROM edge_table',
    true);

SELECT throws_ok('q12', 'XX000', 'Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'Throws because source is FLOAT');
SELECT throws_ok('q13', 'XX000', 'Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'Throws because source is REAL');

PREPARE q14 AS
SELECT * FROM pgr_johnson(
    'SELECT id, source, target::FLOAT, cost, reverse_cost FROM edge_table',
    true);

PREPARE q15 AS
SELECT * FROM pgr_johnson(
    'SELECT id, source, target::REAL, cost, reverse_cost FROM edge_table',
    true);

SELECT throws_ok('q14', 'XX000', 'Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'Throws because source is FLOAT');
SELECT throws_ok('q15', 'XX000', 'Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'Throws because source is REAL');




SELECT finish();
ROLLBACK;
