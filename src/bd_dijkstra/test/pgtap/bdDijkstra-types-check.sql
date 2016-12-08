
\i setup.sql
-- TESTING DOCUMNETATIONS INFORMATION

SELECT plan(29);

--SET client_min_messages TO WARNING;

-- RECEIVES 5 PARAMETERS
SELECT todo_start('remove on version 3.0');
SELECT hasnt_function('pgr_bddijkstra',
    ARRAY['text','integer','integer','boolean','boolean'],
    'Remove old signature on version 3.0');
SELECT todo_end();


SELECT has_function('pgr_bddijkstra',
    ARRAY['text', 'integer', 'integer', 'boolean', 'boolean']);
SELECT function_returns('pgr_bddijkstra',
    ARRAY['text', 'integer', 'integer', 'boolean', 'boolean'],
    'setof pgr_costresult','Returns set of costResult[]');


-- CHECKING THE INNER QUERY



PREPARE q1 AS
SELECT * FROM pgr_bddijkstra(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT  FROM edge_table',
    2, 3, true, true);
SELECT lives_ok('q1', 'edges query accepts INTEGER & FLOAT');




-- id
SELECT lives_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT  FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::BIGINT, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::SMALLINT, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::REAL, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::FLOAT, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)');


-- source
SELECT lives_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::SMALLINT, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::BIGINT, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::REAL, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::FLOAT, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)');

-- target
SELECT lives_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::SMALLINT, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::BIGINT, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::REAL, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::FLOAT, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)');


-- cost
SELECT lives_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::BIGINT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::INTEGER, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::SMALLINT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::REAL, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)');

-- reverse_cost
SELECT lives_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::BIGINT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::INTEGER FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::SMALLINT FROM edge_table'',
        2, 3, true, true)');
SELECT throws_ok(
    'SELECT * FROM pgr_bddijkstra(
        ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::REAL FROM edge_table'',
        2, 3, true, true)');




SELECT finish();
ROLLBACK;
