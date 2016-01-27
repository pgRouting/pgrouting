
\i setup.sql
-- TESTING DOCUMNETATIONS INFORMATION

SELECT plan(17);

-- RECEIVES 5 PARAMETERS

SELECT has_function('pgr_bddijkstra',ARRAY['text', 'integer', 'integer', 'boolean', 'boolean']);
SELECT function_returns('pgr_bddijkstra', 'setof pgr_costresult','Returns set of costResult[]');


-- CHECKING THE INNER QUERY



PREPARE q1 AS
SELECT * FROM pgr_bddijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT  FROM edge_table',
    2, 3, true, true);
SELECT lives_ok('q1', 'edges query accepts INTEGER & FLOAT');



SELECT todo_start('issue #140 must accept ANY-INTEGER and ANY-NUMERICAL');
-- some test pass because the code is not checking

SELECT lives_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::BIGINT, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT  FROM edge_table'',
        2, 3, true, true)',
        'id accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::SMALLINT, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'id accepts SMALLINT');

SELECT lives_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::BIGINT, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'source accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::SMALLINT, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'source accepts SMALLINT');

SELECT lives_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::BIGINT, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'target accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::SMALLINT, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'target accepts SMALLINT');

SELECT lives_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::BIGINT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'cost accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::INTEGER, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'cost accepts INTEGER');
SELECT lives_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::SMALLINT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'cost accepts SMALLINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::REAL, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)',
        'cost accepts REAL');

SELECT lives_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::BIGINT FROM edge_table'',
        2, 3, true, true)',
        'reverse_cost accepts BIGINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::INTEGER FROM edge_table'',
        2, 3, true, true)',
        'reverse_cost accepts INTEGER');
SELECT lives_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::SMALLINT FROM edge_table'',
        2, 3, true, true)',
        'reverse_cost accepts SMALLINT');
SELECT lives_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::REAL FROM edge_table'',
        2, 3, true, true)',
        'reverse_cost accepts REAL');


SELECT todo_end();




SELECT finish();
ROLLBACK;
