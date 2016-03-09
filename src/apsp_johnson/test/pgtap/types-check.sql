
\i setup.sql


SELECT plan(14);
SET client_min_messages TO WARNING;

PREPARE q1 AS
SELECT * FROM pgr_apspJohnson(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'
);

PREPARE q2 AS
SELECT * FROM pgr_apspJohnson(
    'SELECT source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'
);



SELECT lives_ok('q1', 'edges query accepts INTEGER & FLOAT');
SELECT lives_ok('q2', 'without id it works');
SELECT throws_ok(
    'SELECT * FROM pgr_apspJohnson(
        ''SELECT source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table'',
         false)',
    '42883','function pgr_apspjohnson(unknown, boolean) does not exist',
    'directed flag fails with false');
SELECT throws_ok(
    'SELECT * FROM pgr_apspJohnson(
        ''SELECT source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table'',
         true)',
    '42883','function pgr_apspjohnson(unknown, boolean) does not exist',
    'directed flag fails with true');


PREPARE q10 AS
SELECT * FROM pgr_apspJohnson(
    'SELECT id::FLOAT, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table'
);

PREPARE q11 AS
SELECT * FROM pgr_apspJohnson(
    'SELECT id::REAL, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table'
);

SELECT lives_ok('q10', 'id is ignored FLOAT');
SELECT lives_ok('q11', 'id is ignored REAL');


PREPARE q12 AS
SELECT * FROM pgr_apspJohnson(
    'SELECT id, source::FLOAT, target, cost, reverse_cost FROM edge_table'
);

PREPARE q13 AS
SELECT * FROM pgr_apspJohnson(
    'SELECT id, source::REAL, target, cost, reverse_cost FROM edge_table'
);

SELECT throws_ok('q12', 'XX000', 'Support for source,target columns only of type: integer. Support for Cost: double precision',
    'Throws because source is FLOAT');
SELECT throws_ok('q13', 'XX000', 'Support for source,target columns only of type: integer. Support for Cost: double precision',
    'Throws because source is REAL');

PREPARE q14 AS
SELECT * FROM pgr_apspJohnson(
    'SELECT id, source::INTEGER, target::INTEGER::FLOAT, cost, reverse_cost FROM edge_table'
);

PREPARE q15 AS
SELECT * FROM pgr_apspJohnson(
    'SELECT id, source::INTEGER, target::INTEGER::REAL, cost, reverse_cost FROM edge_table'
);

SELECT throws_ok('q14', 'XX000', 'Support for source,target columns only of type: integer. Support for Cost: double precision',
    'Throws because target is FLOAT');
SELECT throws_ok('q15', 'XX000', 'Support for source,target columns only of type: integer. Support for Cost: double precision',
    'Throws because source is REAL');

PREPARE q16 AS
SELECT * FROM pgr_apspJohnson(
    'SELECT id, source::INTEGER, target::INTEGER, cost::SMALLINT, reverse_cost FROM edge_table'
);

PREPARE q17 AS
SELECT * FROM pgr_apspJohnson(
    'SELECT id, source::INTEGER, target::INTEGER, cost::INTEGER, reverse_cost FROM edge_table'
);

PREPARE q18 AS
SELECT * FROM pgr_apspJohnson(
    'SELECT id, source::INTEGER, target::INTEGER, cost::BIGINT, reverse_cost FROM edge_table'
);

PREPARE q19 AS
SELECT * FROM pgr_apspJohnson(
    'SELECT id, source::INTEGER, target::INTEGER, cost::REAL, reverse_cost FROM edge_table'
);



SELECT throws_ok('q16', 'XX000', 'Support for source,target columns only of type: integer. Support for Cost: double precision',
    'Throws because cost is SMALLINT');
SELECT throws_ok('q17', 'XX000', 'Support for source,target columns only of type: integer. Support for Cost: double precision',
    'Throws because cost is INTEGER');
SELECT throws_ok('q18', 'XX000', 'Support for source,target columns only of type: integer. Support for Cost: double precision',
    'Throws because cost is BIGINT');
SELECT throws_ok('q19', 'XX000', 'Support for source,target columns only of type: integer. Support for Cost: double precision',
    'Throws because cost is REAL');



SELECT finish();
ROLLBACK;
