\i setup.sql

SELECT plan(6);

SELECT has_function('pgr_directedchpp_cost');

SELECT has_function('pgr_directedchpp_cost', ARRAY['text']);
SELECT function_returns('pgr_directedchpp_cost', ARRAY['text'], 'double precision');

-- column names
SELECT set_eq(
    $$SELECT proargnames from pg_proc where proname = 'pgr_directedchpp_cost'$$,
    $$SELECT NULL::TEXT[] $$
);

-- pgr_directedchpp_cost works
PREPARE t1 AS
SELECT * FROM pgr_directedchpp_cost(
    'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table where id < 17'
);

SELECT lives_ok('t1', 'pgr_directedchpp_cost');

-- prepare for testing return types
PREPARE all_return AS
SELECT
    'double precision'::text AS t1;

PREPARE q1 AS
SELECT pg_typeof(pgr_directedChPP_Cost)::text AS t1
    FROM (
        SELECT * FROM pgr_directedchpp_cost(
            'SELECT id, source, target, capacity, reverse_capacity, cost, reverse_cost FROM edge_table where id < 17'
        ) ) AS a
    limit 1;

-- test return types
SELECT set_eq('q1', 'all_return', 'Expected returning, columns names & types');

SELECT * FROM finish();
ROLLBACK;
