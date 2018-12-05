\i setup.sql

SELECT plan(6);

SELECT has_function('pgr_directedchpp');

SELECT has_function('pgr_directedchpp', ARRAY['text']);
SELECT function_returns('pgr_directedchpp', ARRAY['text'], 'setof record');

-- column names
SELECT set_eq(
    $$SELECT proargnames from pg_proc where proname = 'pgr_directedchpp'$$,
    $$SELECT '{"", "seq", "node", "edge", "cost", "agg_cost"}'::TEXT[] $$
);

-- pgr_directedchpp works
PREPARE t1 AS
SELECT * FROM pgr_directedchpp(
    'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table where id < 17'
);

SELECT lives_ok('t1', 'pgr_directedchpp');

-- prepare for testing return types
PREPARE all_return AS
SELECT
    'integer'::text AS t1,
    'bigint'::text AS t2,
    'bigint'::text AS t3,
    'double precision'::text AS t4,
    'double precision'::text AS t5;

PREPARE q1 AS
SELECT pg_typeof(seq)::text AS t1,
       pg_typeof(node)::text AS t2,
       pg_typeof(edge)::text AS t3,
       pg_typeof(cost)::text AS t4,
       pg_typeof(agg_cost)::text AS t5
    FROM (
        SELECT * FROM pgr_directedchpp(
            'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table where id < 17'
        ) ) AS a
    limit 1;

-- test return types
SELECT set_eq('q1', 'all_return', 'Expected returning, columns names & types');

SELECT * FROM finish();
ROLLBACK;
