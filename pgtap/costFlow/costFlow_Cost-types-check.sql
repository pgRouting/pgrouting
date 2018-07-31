\i setup.sql

SELECT plan(21);

SELECT has_function('pgr_mincostmaxflow_cost');

SELECT has_function('pgr_mincostmaxflow_cost', ARRAY['text', 'bigint', 'bigint']);
SELECT has_function('pgr_mincostmaxflow_cost', ARRAY['text', 'anyarray', 'bigint']);
SELECT has_function('pgr_mincostmaxflow_cost', ARRAY['text', 'bigint', 'anyarray']);
SELECT has_function('pgr_mincostmaxflow_cost', ARRAY['text', 'anyarray', 'anyarray']);

SELECT function_returns('pgr_mincostmaxflow_cost', ARRAY['text', 'bigint', 'bigint'], 'double precision');
SELECT function_returns('pgr_mincostmaxflow_cost', ARRAY['text', 'anyarray', 'bigint'], 'double precision');
SELECT function_returns('pgr_mincostmaxflow_cost', ARRAY['text', 'bigint', 'anyarray'], 'double precision');
SELECT function_returns('pgr_mincostmaxflow_cost', ARRAY['text', 'anyarray', 'anyarray'], 'double precision');

-- column names
SELECT bag_has(
    $$SELECT proargnames from pg_proc where proname = 'pgr_mincostmaxflow_cost'$$,
    $$SELECT '{"edges_sql", "sources", "targets"}'::TEXT[] $$
);
SELECT bag_has(
    $$SELECT proargnames from pg_proc where proname = 'pgr_mincostmaxflow_cost'$$,
    $$SELECT '{"edges_sql","sources","targets"}'::TEXT[] $$
);
SELECT bag_has(
    $$SELECT proargnames from pg_proc where proname = 'pgr_mincostmaxflow_cost'$$,
    $$SELECT '{"edges_sql","sources","targets"}'::TEXT[] $$
);
SELECT bag_has(
    $$SELECT proargnames from pg_proc where proname = 'pgr_mincostmaxflow_cost'$$,
    $$SELECT '{"edges_sql","sources","targets"}'::TEXT[] $$
);

-- pgr_mincostmaxflow_cost works
PREPARE t1 AS
SELECT * FROM pgr_mincostmaxflow_cost(
    'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table',
    2, 3
);
PREPARE t2 AS
SELECT * FROM pgr_mincostmaxflow_cost(
    'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table',
    ARRAY[2], 3
);
PREPARE t3 AS
SELECT * FROM pgr_mincostmaxflow_cost(
    'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table',
    2, ARRAY[3]
);
PREPARE t4 AS
SELECT * FROM pgr_mincostmaxflow_cost(
    'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table',
    ARRAY[2], ARRAY[3]
);

SELECT lives_ok('t1', 'pgr_mincostmaxflow_cost(one to one)');
SELECT lives_ok('t2', 'pgr_mincostmaxflow_cost(many to one)');
SELECT lives_ok('t3', 'pgr_mincostmaxflow_cost(one to many)');
SELECT lives_ok('t4', 'pgr_mincostmaxflow_cost(many to many)');

-- prepare for testing return types
PREPARE all_return AS
SELECT
    'double precision'::text AS t1;

PREPARE q1 AS
SELECT pg_typeof(pgr_minCostMaxFlow_cost)::text AS t1
    FROM (
        SELECT * FROM pgr_mincostmaxflow_cost(
            'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table',
            2, 3
        ) ) AS a
    limit 1;

PREPARE q2 AS
SELECT pg_typeof(pgr_minCostMaxFlow_cost)::text AS t1
    FROM (
        SELECT * FROM pgr_mincostmaxflow_cost(
            'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table',
            ARRAY[2], 3
        ) ) AS a
    limit 1;

PREPARE q3 AS
SELECT pg_typeof(pgr_minCostMaxFlow_cost)::text AS t1
    FROM (
        SELECT * FROM pgr_mincostmaxflow_cost(
            'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table',
            2, ARRAY[3]
        ) ) AS a
    limit 1;

PREPARE q4 AS
SELECT pg_typeof(pgr_minCostMaxFlow_cost)::text AS t1
    FROM (
        SELECT * FROM pgr_mincostmaxflow_cost(
            'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table',
            ARRAY[2], ARRAY[3]
        ) ) AS a
    limit 1;

-- test return types
SELECT set_eq('q1', 'all_return', '1 to 1: Expected returning, columns names & types');
SELECT set_eq('q2', 'all_return', 'many to 1: Expected returning, columns names & types');
SELECT set_eq('q3', 'all_return', '1 to many: Expected returning, columns names & types');
SELECT set_eq('q4', 'all_return', 'many to many: Expected returning, columns names & types');

SELECT * FROM finish();
ROLLBACK;
