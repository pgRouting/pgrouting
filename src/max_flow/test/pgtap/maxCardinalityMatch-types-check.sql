\i setup.sql
SET client_min_messages TO WARNING;

SELECT plan(7);


SELECT has_function('pgr_maxcardinalitymatch');

SELECT has_function('pgr_maxcardinalitymatch', ARRAY['text', 'boolean']);

SELECT function_returns('pgr_maxcardinalitymatch', ARRAY['text', 'boolean'], 'setof record');

-- Maximum cardinality matching
PREPARE mcm_dir AS
SELECT * FROM pgr_maxCardinalityMatch(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table'
);

PREPARE mcm_undir AS
SELECT * FROM pgr_maxCardinalityMatch(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
    directed := false
);

SELECT lives_ok('mcm_dir','directed');
SELECT lives_ok('mcm_undir','undirected');

PREPARE mcm_q AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(edge)::text AS t2,
    pg_typeof(source)::text AS t3, pg_typeof(target)::text AS t4
    FROM (
        SELECT * FROM pgr_maxCardinalityMatch(
        'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table'
        ) ) AS a
    limit 1
;

PREPARE mcm_v AS
SELECT  'integer'::text AS t1,'bigint'::text AS t2,
    'bigint'::text AS t3, 'bigint'::text AS t4;

SELECT set_eq('mcm_q', 'mcm_v','Expected returning, columns names & types');

-- testing column names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_maxcardinalitymatch'$$,
    $$SELECT  '{"edges_sql","directed","seq","edge","source","target"}'::TEXT[] $$
);


SELECT * FROM finish();
ROLLBACK;
