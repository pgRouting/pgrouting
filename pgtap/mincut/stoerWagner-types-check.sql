\i setup.sql

SELECT plan(5);

SELECT has_function('pgr_stoerwagner');

SELECT function_returns('pgr_stoerwagner', ARRAY['text'], 'setof record');


-- flags
-- error

SELECT lives_ok(
    'SELECT * FROM pgr_stoerWagner(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table''
    )',
    '4: Documentation says works with no flags');

SELECT throws_ok(
    'SELECT * FROM pgr_stoerWagner(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table id < 17'',
        3
    )','42883','function pgr_stoerwagner(unknown, integer) does not exist',
    '6: Documentation says it does not work with 1 flags');


-- prepare for testing return types

PREPARE all_return AS
SELECT
    'integer'::text AS t1,
    'bigint'::text AS t2,
    'double precision'::text AS t3,
    'double precision'::text AS t4;

PREPARE q1 AS
SELECT pg_typeof(seq)::text AS t1,
       pg_typeof(edge)::text AS t2,
       pg_typeof(cost)::text AS t3,
       pg_typeof(mincut)::text AS t4
    FROM (
        SELECT * FROM pgr_stoerWagner(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id < 17'
        ) ) AS a
    limit 1;


SELECT set_eq('q1', 'all_return', 'Expected returning, columns names & types');

SELECT * FROM finish();
ROLLBACK;
