\i setup.sql

SELECT plan(21);

SELECT has_function('pgr_maxflow');

SELECT has_function('pgr_maxflow', ARRAY[ 'text', 'bigint', 'bigint' ]);
SELECT has_function('pgr_maxflow', ARRAY[ 'text', 'anyarray', 'bigint' ]);
SELECT has_function('pgr_maxflow', ARRAY[ 'text', 'bigint', 'anyarray' ]);
SELECT has_function('pgr_maxflow', ARRAY[ 'text', 'anyarray', 'anyarray' ]);

SELECT function_returns('pgr_maxflow', ARRAY[ 'text', 'bigint', 'bigint' ], 'bigint');
SELECT function_returns('pgr_maxflow', ARRAY[ 'text', 'bigint', 'anyarray' ], 'bigint');
SELECT function_returns('pgr_maxflow', ARRAY[ 'text', 'anyarray', 'bigint' ], 'bigint');
SELECT function_returns('pgr_maxflow', ARRAY[ 'text', 'anyarray', 'anyarray' ], 'bigint');

-- testing column names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_maxflow'$$,
    $$SELECT  '{"edges_sql","source_vertices","sink_vertices"}'::TEXT[] $$
);
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_maxflow'$$,
    $$SELECT  '{"edges_sql","source_vertices","sink_vertices"}'::TEXT[] $$
);
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_maxflow'$$,
    $$SELECT  '{"edges_sql","source_vertices","sink_vertices"}'::TEXT[] $$
);
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_maxflow'$$,
    $$SELECT  '{"edges_sql","source_vertices","sink_vertices"}'::TEXT[] $$
);


-- pgr_maxflow works
PREPARE t1 AS
SELECT * FROM pgr_maxflow(
    'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
    3, 5
);
PREPARE t2 AS
SELECT * FROM pgr_maxflow(
    'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
    3, ARRAY[5]
);
PREPARE t3 AS
SELECT * FROM pgr_maxflow(
    'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
    ARRAY[3], 5
);
PREPARE t4 AS
SELECT * FROM pgr_maxflow(
    'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
    ARRAY[3], ARRAY[5]
);

SELECT lives_ok('t1','pgr_pgr_maxflow(one to one)');
SELECT lives_ok('t2','pgr_pgr_maxflow(one to many)');
SELECT lives_ok('t3','pgr_pgr_maxflow(many to one)');
SELECT lives_ok('t4','pgr_pgr_maxflow(many to many)');


-- preparing for testing return types
PREPARE all_return AS
SELECT
    'bigint'::text AS t1;


PREPARE q1 AS
SELECT pg_typeof(pgr_maxflow)::text AS t1
    FROM (
        SELECT * FROM pgr_maxflow(
            'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
        3, 5
        ) ) AS a
    limit 1;


PREPARE q2 AS
SELECT pg_typeof(pgr_maxflow)::text AS t1
    FROM (
        SELECT * FROM pgr_maxflow(
            'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
        3, ARRAY[5]
        ) ) AS a
    limit 1
;

PREPARE q3 AS
SELECT pg_typeof(pgr_maxflow)::text AS t1
    FROM (
        SELECT * FROM pgr_maxflow(
            'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
        ARRAY[3], 5
        ) ) AS a
    limit 1;


PREPARE q4 AS
SELECT pg_typeof(pgr_maxflow)::text AS t1
    FROM (
        SELECT * FROM pgr_maxflow(
            'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
        ARRAY[3], ARRAY[5]
        ) ) AS a
    limit 1
;

-- testing return types
SELECT set_eq('q1', 'all_return','1 to 1: Expected returning, columns names & types');
SELECT set_eq('q2', 'all_return','1 to many: Expected returning, columns names & types');
SELECT set_eq('q3', 'all_return','many to 1: Expected returning, columns names & types');
SELECT set_eq('q4', 'all_return','many to many: Expected returning, columns names & types');


SELECT * FROM finish();
ROLLBACK;
