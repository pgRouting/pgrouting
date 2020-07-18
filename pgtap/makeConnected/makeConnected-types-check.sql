\i setup.sql

SELECT plan(5);

SELECT has_function('pgr_makeconnected');

SELECT function_returns('pgr_makeconnected', ARRAY['text'], 'setof record');


-- flags
-- error

SELECT lives_ok(
    'SELECT * FROM pgr_makeConnected(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table''
    )',
    '3: Documentation says works with no flags');

SELECT throws_ok(
    'SELECT * FROM pgr_makeConnected(
        ''SELECT id, source, target, cost, reverse_cost FROM edge_table id < 17'',
        3
    )','42883','function pgr_makeConnected(unknown, integer) does not exist',
    '4: Documentation says it does not work with 1 flags');


-- prepare for testing return types

PREPARE all_return AS
SELECT
    'bigint'::text AS t1,
    'bigint'::text AS t2,
    'bigint'::text AS t3;

PREPARE q5 AS
SELECT pg_typeof(seq)::text AS t1,
       pg_typeof(node_from)::text AS t2,
       pg_typeof(node_to)::text AS t3
    FROM (
        SELECT * FROM pgr_makeConnected(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id < 17'
        ) ) AS a LIMIT 1;


SELECT set_eq('q5', 'all_return', 'Expected returning, columns names & types');

SELECT * FROM finish();
ROLLBACK;
