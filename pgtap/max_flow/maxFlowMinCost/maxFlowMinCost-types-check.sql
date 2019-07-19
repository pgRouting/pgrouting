\i setup.sql

SELECT plan(18);

SELECT has_function('pgr_maxflowmincost');

SELECT has_function('pgr_maxflowmincost', ARRAY['text', 'bigint', 'bigint']);
SELECT has_function('pgr_maxflowmincost', ARRAY['text', 'anyarray', 'bigint']);
SELECT has_function('pgr_maxflowmincost', ARRAY['text', 'bigint', 'anyarray']);
SELECT has_function('pgr_maxflowmincost', ARRAY['text', 'anyarray', 'anyarray']);

SELECT function_returns('pgr_maxflowmincost', ARRAY['text', 'bigint', 'bigint'], 'setof record');
SELECT function_returns('pgr_maxflowmincost', ARRAY['text', 'anyarray', 'bigint'], 'setof record');
SELECT function_returns('pgr_maxflowmincost', ARRAY['text', 'bigint', 'anyarray'], 'setof record');
SELECT function_returns('pgr_maxflowmincost', ARRAY['text', 'anyarray', 'anyarray'], 'setof record');

-- column names
SELECT set_eq(
    $$SELECT proargnames from pg_proc where proname = 'pgr_maxflowmincost'$$,
    $$VALUES
        ('{"","","", "seq", "edge", "source", "target", "flow", "residual_capacity", "cost", "agg_cost"}'::TEXT[])
    $$
);

-- pgr_maxflowmincost works
PREPARE t1 AS
SELECT * FROM pgr_maxflowmincost(
    'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table',
    2, 3
);
PREPARE t2 AS
SELECT * FROM pgr_maxflowmincost(
    'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table',
    ARRAY[2], 3
);
PREPARE t3 AS
SELECT * FROM pgr_maxflowmincost(
    'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table',
    2, ARRAY[3]
);
PREPARE t4 AS
SELECT * FROM pgr_maxflowmincost(
    'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table',
    ARRAY[2], ARRAY[3]
);

SELECT lives_ok('t1', 'pgr_maxflowmincost(one to one)');
SELECT lives_ok('t2', 'pgr_maxflowmincost(many to one)');
SELECT lives_ok('t3', 'pgr_maxflowmincost(one to many)');
SELECT lives_ok('t4', 'pgr_maxflowmincost(many to many)');

-- prepare for testing return types
PREPARE all_return AS
SELECT
    'integer'::text AS t1,
    'bigint'::text AS t2,
    'bigint'::text AS t3,
    'bigint'::text AS t4,
    'bigint'::text AS t5,
    'bigint'::text AS t6,
    'double precision'::text AS t7,
    'double precision'::text AS t8;

PREPARE q1 AS
SELECT pg_typeof(seq)::text AS t1,
       pg_typeof(edge)::text AS t2,
       pg_typeof(source)::text AS t3,
       pg_typeof(target)::text AS t4,
       pg_typeof(flow)::text AS t5,
       pg_typeof(residual_capacity)::text AS t6,
       pg_typeof(cost)::text AS t7,
       pg_typeof(agg_cost)::text AS t8
    FROM (
        SELECT * FROM pgr_maxflowmincost(
            'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table',
            2, 3
        ) ) AS a
    limit 1;

PREPARE q2 AS
SELECT pg_typeof(seq)::text AS t1,
       pg_typeof(edge)::text AS t2,
       pg_typeof(source)::text AS t3,
       pg_typeof(target)::text AS t4,
       pg_typeof(flow)::text AS t5,
       pg_typeof(residual_capacity)::text AS t6,
       pg_typeof(cost)::text AS t7,
       pg_typeof(agg_cost)::text AS t8
    FROM (
        SELECT * FROM pgr_maxflowmincost(
            'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table',
            ARRAY[2], 3
        ) ) AS a
    limit 1;

PREPARE q3 AS
SELECT pg_typeof(seq)::text AS t1,
       pg_typeof(edge)::text AS t2,
       pg_typeof(source)::text AS t3,
       pg_typeof(target)::text AS t4,
       pg_typeof(flow)::text AS t5,
       pg_typeof(residual_capacity)::text AS t6,
       pg_typeof(cost)::text AS t7,
       pg_typeof(agg_cost)::text AS t8
    FROM (
        SELECT * FROM pgr_maxflowmincost(
            'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table',
            2, ARRAY[3]
        ) ) AS a
    limit 1;

PREPARE q4 AS
SELECT pg_typeof(seq)::text AS t1,
       pg_typeof(edge)::text AS t2,
       pg_typeof(source)::text AS t3,
       pg_typeof(target)::text AS t4,
       pg_typeof(flow)::text AS t5,
       pg_typeof(residual_capacity)::text AS t6,
       pg_typeof(cost)::text AS t7,
       pg_typeof(agg_cost)::text AS t8
    FROM (
        SELECT * FROM pgr_maxflowmincost(
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
