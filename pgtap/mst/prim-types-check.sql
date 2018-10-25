\i setup.sql

SELECT plan(7);

SELECT has_function('pgr_prim', ARRAY['text', 'bigint']);

SELECT function_returns('pgr_prim', ARRAY['text', 'bigint'], 'setof record');

-- flags
-- error

SELECT lives_ok(
    'SELECT * FROM pgr_prim(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table''
    )',
    '4: Documentation says works with no flags');


SELECT lives_ok(
    'SELECT * FROM pgr_prim(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table'',
        4
    )',
    '5: Documentation says works with 1 flag');

SELECT throws_ok(
    'SELECT * FROM pgr_prim(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'',
        3, 2
    )','42883','function pgr_prim(unknown, integer, integer) does not exist',
    '6: Documentation says it does not work with 2 flags');


-- prepare for testing return types

PREPARE all_return AS
SELECT
    'bigint'::text AS t1,
    'bigint'::text AS t2,
    'bigint'::text AS t3,
    'bigint'::text AS t4,
    'double precision'::text AS t5,
    'double precision'::text AS t6,
    'double precision'::text AS t7;

PREPARE q1 AS
SELECT pg_typeof(seq)::text AS t1,
       pg_typeof(root_vertex)::text AS t2,
       pg_typeof(node)::text AS t3,
       pg_typeof(edge)::text AS t4,
       pg_typeof(cost)::text AS t5,
       pg_typeof(agg_cost)::text AS t6,
       pg_typeof(tree_cost)::text AS t7
    FROM (
        SELECT * FROM pgr_prim(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            4
        ) ) AS a
    limit 1;

PREPARE q2 AS
SELECT pg_typeof(seq)::text AS t1,
       pg_typeof(root_vertex)::text AS t2,
       pg_typeof(node)::text AS t3,
       pg_typeof(edge)::text AS t4,
       pg_typeof(cost)::text AS t5,
       pg_typeof(agg_cost)::text AS t6,
       pg_typeof(tree_cost)::text AS t7
    FROM (
        SELECT * FROM pgr_prim(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table'
        ) ) AS a
    limit 1;

SELECT set_eq('q1', 'all_return', 'Expected returning, columns names & types');
SELECT set_eq('q2', 'all_return', 'Expected returning, columns names & types');


SELECT * FROM finish();
ROLLBACK;
