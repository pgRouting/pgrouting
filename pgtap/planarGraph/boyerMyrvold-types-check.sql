\i setup.sql

SELECT plan(5);

SELECT has_function('pgr_boyermyrvold');

SELECT function_returns('pgr_boyermyrvold', ARRAY['text'], 'setof record');


-- flags
-- error

SELECT lives_ok(
    'SELECT * FROM pgr_boyerMyrvold(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table''
    )',
    '3: Documentation says works with no flags');

SELECT throws_ok(
    'SELECT * FROM pgr_boyerMyrvold(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table id < 17'',
        3
    )','42883','function pgr_boyermyrvold(unknown, integer) does not exist',
    '4: Documentation says it does not work with 1 flags');


-- prepare for testing return types

PREPARE all_return AS
SELECT
    'bigint'::text AS t1,
    'bigint'::text AS t2,
    'bigint'::text AS t3,
    'double precision'::text AS t4;

PREPARE q5 AS
SELECT pg_typeof(seq)::text AS t1,
       pg_typeof(source)::text AS t2,
       pg_typeof(target)::text AS t3,
       pg_typeof(cost)::text AS t4
    FROM (
        SELECT * FROM pgr_boyerMyrvold(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id < 17'
        ) ) AS a LIMIT 1;


SELECT set_eq('q5', 'all_return', 'Expected returning, columns names & types');

SELECT * FROM finish();
ROLLBACK;
