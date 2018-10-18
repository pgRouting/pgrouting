\i setup.sql

SELECT plan(5);

SELECT has_function('pgr_kruskal', ARRAY['text']);

SELECT function_returns('pgr_kruskal', ARRAY['text'], 'setof record');

-- flags
-- error

SELECT lives_ok(
    'SELECT * FROM pgr_kruskal(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table''
    )',
    '4: Documentation says works with no flags');

SELECT throws_ok(
    'SELECT * FROM pgr_kruskal(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'',
        3
    )','42883','function pgr_kruskal(unknown, integer) does not exist',
    '6: Documentation says it does not work with 1 flags');


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
       pg_typeof(component)::text AS t2,
       pg_typeof(edge)::text AS t3,
       pg_typeof(cost)::text AS t4,
       pg_typeof(tree_cost)::text AS t5
    FROM (
        SELECT * FROM pgr_kruskal(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table'
        ) ) AS a
    limit 1;


SELECT set_eq('q1', 'all_return', 'Expected returning, columns names & types');

SELECT * FROM finish();
ROLLBACK;
