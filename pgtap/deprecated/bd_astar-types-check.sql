
\i setup.sql
-- TESTING DOCUMNETATIONS INFORMATION

SELECT plan(33);

-- RECEIVES 5 PARAMETERS

SELECT has_function('pgr_bdastar',ARRAY['text', 'integer', 'integer', 'boolean', 'boolean']);
SELECT function_returns('pgr_bdastar', ARRAY['text', 'integer', 'integer', 'boolean', 'boolean'], 'setof pgr_costresult','Returns set of costResult[]');


-- CHECKING THE INNER QUERY



PREPARE q1 AS
SELECT * FROM pgr_bdastar(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table',
    2, 3, true, true);
SELECT lives_ok('q1', 'edges query accepts INTEGER & FLOAT');



SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::BIGINT, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::SMALLINT, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');

SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::BIGINT, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::SMALLINT, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');

SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::BIGINT, cost::FLOAT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::SMALLINT, cost::FLOAT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');

SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::BIGINT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::INTEGER, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::SMALLINT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::REAL, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');

SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::BIGINT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::INTEGER, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::SMALLINT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::REAL, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');

-- x1
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::BIGINT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::INTEGER, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::SMALLINT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::REAL, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');

-- x2
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x2::BIGINT, y1::FLOAT, x1::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x2::INTEGER, y1::FLOAT, x1::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x2::SMALLINT, y1::FLOAT, x1::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x2::REAL, y1::FLOAT, x1::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');


-- y1
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y1::BIGINT, x1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y1::INTEGER, x1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y1::SMALLINT, x1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y1::REAL, x1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)');

-- y2
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y2::BIGINT, y1::FLOAT, x1::FLOAT, x2::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y2::INTEGER, y1::FLOAT, x1::FLOAT, x2::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y2::SMALLINT, y1::FLOAT, x1::FLOAT, x2::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y2::REAL, y1::FLOAT, x1::FLOAT, x2::FLOAT FROM edge_table'',
        2, 3, true, true)');



SELECT finish();
ROLLBACK;
