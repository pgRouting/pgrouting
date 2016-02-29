
\i setup.sql
-- TESTING DOCUMNETATIONS INFORMATION

SELECT plan(33);

-- RECEIVES 5 PARAMETERS

SELECT has_function('pgr_bdastar',ARRAY['text', 'integer', 'integer', 'boolean', 'boolean']);
SELECT function_returns('pgr_bdastar', 'setof pgr_costresult','Returns set of costResult[]');


-- CHECKING THE INNER QUERY



PREPARE q1 AS
SELECT * FROM pgr_bdastar(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table',
    2, 3, true, true);
SELECT lives_ok('q1', 'edges query accepts INTEGER & FLOAT');



SELECT todo_start('issue #140 must accept ANY-INTEGER and ANY-NUMERICAL');
-- some test pass because the code is not checking

SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::BIGINT, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'id accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::SMALLINT, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'id accepts SMALLINT');

SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::BIGINT, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'source accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::SMALLINT, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'source accepts SMALLINT');

SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::BIGINT, cost::FLOAT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'target accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::SMALLINT, cost::FLOAT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'target accepts SMALLINT');

SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::BIGINT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'cost accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::INTEGER, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'cost accepts INTEGER');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::SMALLINT, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'cost accepts SMALLINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::REAL, reverse_cost::FLOAT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'cost accepts REAL');

SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::BIGINT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'reverse_cost accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::INTEGER, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'reverse_cost accepts INTEGER');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::SMALLINT, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'reverse_cost accepts SMALLINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::REAL, x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'reverse_cost accepts REAL');

-- x1
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::BIGINT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'x1 accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::INTEGER, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'x1 accepts INTEGER');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::SMALLINT, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'x1 accepts SMALLINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x1::REAL, y1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'x1 accepts REAL');

-- x2
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x2::BIGINT, y1::FLOAT, x1::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'x2 accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x2::INTEGER, y1::FLOAT, x1::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'x2 accepts INTEGER');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x2::SMALLINT, y1::FLOAT, x1::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'x2 accepts SMALLINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, x2::REAL, y1::FLOAT, x1::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'x2 accepts REAL');


-- y1
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y1::BIGINT, x1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'y1 accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y1::INTEGER, x1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'y1 accepts INTEGER');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y1::SMALLINT, x1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'y1 accepts SMALLINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y1::REAL, x1::FLOAT, x2::FLOAT, y2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'y1 accepts REAL');

-- y2
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y2::BIGINT, y1::FLOAT, x1::FLOAT, x2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'y2 accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y2::INTEGER, y1::FLOAT, x1::FLOAT, x2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'y2 accepts INTEGER');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y2::SMALLINT, y1::FLOAT, x1::FLOAT, x2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'y2 accepts SMALLINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bdastar(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT, y2::REAL, y1::FLOAT, x1::FLOAT, x2::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'y2 accepts REAL');

SELECT todo_end();




SELECT finish();
ROLLBACK;
