
\i setup.sql


SELECT plan(20);
SET client_min_messages TO WARNING;

PREPARE q1 AS
SELECT * FROM pgr_apspWarshall(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table',
    true, true
);

PREPARE q2 AS
SELECT * FROM pgr_apspWarshall(
    'SELECT source, target, cost, reverse_cost FROM edge_table',
    true, true
);



SELECT lives_ok('q1', '1: edges query accepts INTEGER & FLOAT');
SELECT throws_ok(
    'SELECT * FROM pgr_apspWarshall(
        ''SELECT source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        true, true)',
    'XX000','An expected column was not found in the query',
    'without id fails');
SELECT throws_ok(
    'SELECT * FROM pgr_apspWarshall(
        ''SELECT id::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        true, true)',
    'XX000','An expected column was not found in the query',
    'without source fails');
SELECT throws_ok(
    'SELECT * FROM pgr_apspWarshall(
        ''SELECT id::INTEGER, source::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        true, true)',
    'XX000','An expected column was not found in the query',
    'without target fails');
SELECT throws_ok(
    'SELECT * FROM pgr_apspWarshall(
        ''SELECT id, source, target, reverse_cost FROM edge_table'',
        true, true)',
    'XX000','An expected column was not found in the query',
    'without cost fails');

SELECT lives_ok(
    'SELECT * FROM pgr_apspWarshall(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table'',
         true, false)', '6: Without reverse_cost works');
SELECT lives_ok(
    'SELECT * FROM pgr_apspWarshall(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table'',
         true, false)',
    '7: Contradiction works');
SELECT throws_ok(
    'SELECT * FROM pgr_apspWarshall(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table'',
         true, true)',
    'P0001','has_rcost set to true but reverse_cost not found',
    '8: Contradiction fails');


SELECT throws_ok(
    'SELECT * FROM pgr_apspWarshall(
        ''SELECT source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table'',
         false)',
    '42883','function pgr_apspwarshall(unknown, boolean) does not exist',
    'One flag fails');
SELECT throws_ok(
    'SELECT * FROM pgr_apspWarshall(
        ''SELECT source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table'',
         true)',
    '42883','function pgr_apspwarshall(unknown, boolean) does not exist',
    'One flag fails');



PREPARE q10 AS
SELECT * FROM pgr_apspWarshall(
    'SELECT id::FLOAT, source, target, cost, reverse_cost FROM edge_table',
    true, true);
PREPARE q11 AS
SELECT * FROM pgr_apspWarshall(
    'SELECT id::REAL, source, target, cost, reverse_cost FROM edge_table',
    true, true);
SELECT throws_ok('q10', 'XX000', 'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    '11: Throws because id is FLOAT');
SELECT throws_ok('q11', 'XX000', 'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'Throws because id is REAL');

PREPARE q12 AS
SELECT * FROM pgr_apspWarshall(
    'SELECT id, source::FLOAT, target, cost, reverse_cost FROM edge_table',
    true, true
);

PREPARE q13 AS
SELECT * FROM pgr_apspWarshall(
    'SELECT id, source::REAL, target, cost, reverse_cost FROM edge_table',
    true, true
);

SELECT throws_ok('q12', 'XX000', 'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'Throws because source is FLOAT');
SELECT throws_ok('q13', 'XX000', 'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'Throws because source is REAL');

PREPARE q14 AS
SELECT * FROM pgr_apspWarshall(
    'SELECT id, source, target::FLOAT, cost, reverse_cost FROM edge_table',
    true, true
);

PREPARE q15 AS
SELECT * FROM pgr_apspWarshall(
    'SELECT id, source, target::REAL, cost, reverse_cost FROM edge_table',
    true, true
);

SELECT throws_ok('q14', 'XX000', 'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'Throws because target is FLOAT');
SELECT throws_ok('q15', 'XX000', 'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'Throws because source is REAL');

PREPARE q16 AS
SELECT * FROM pgr_apspWarshall(
    'SELECT id, source, target, cost::SMALLINT, reverse_cost FROM edge_table',
    true, true
);

PREPARE q17 AS
SELECT * FROM pgr_apspWarshall(
    'SELECT id, source, target, cost::INTEGER, reverse_cost FROM edge_table',
    true, true
);

PREPARE q18 AS
SELECT * FROM pgr_apspWarshall(
    'SELECT id, source, target, cost::BIGINT, reverse_cost FROM edge_table',
    true, true
);

PREPARE q19 AS
SELECT * FROM pgr_apspWarshall(
    'SELECT id, source, target, cost::REAL, reverse_cost FROM edge_table',
    true, true
);



SELECT throws_ok('q16', 'XX000', 'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'Throws because cost is SMALLINT');
SELECT throws_ok('q17', 'XX000', 'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'Throws because cost is INTEGER');
SELECT throws_ok('q18', 'XX000', 'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'Throws because cost is BIGINT');
SELECT throws_ok('q19', 'XX000', 'Error, columns ''source'', ''target'' must be of type int4, ''cost'' must be of type float8',
    'Throws because cost is REAL');


SELECT finish();
ROLLBACK;
