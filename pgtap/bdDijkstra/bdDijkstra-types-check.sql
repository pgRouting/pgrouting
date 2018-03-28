\i setup.sql


SELECT plan(6);

SELECT can(ARRAY['pgr_bddijkstra']);


--V2.4+
SELECT has_function('pgr_bddijkstra',
    ARRAY['text','bigint','bigint','boolean']);
SELECT function_returns('pgr_bddijkstra',
    ARRAY['text','bigint','bigint','boolean'],
    'setof record');

-- testing for the signature that they return the correct names & columns

PREPARE v21q00 AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(path_seq)::text AS t2,
    pg_typeof(node)::text AS t5, pg_typeof(edge)::text AS t6,
    pg_typeof(cost)::text AS t7, pg_typeof(agg_cost)::TEXT AS t8
    FROM (
        SELECT * FROM pgr_bddijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            2, 3, true) ) AS a
    limit 1
;

PREPARE v21q01 AS
SELECT  'integer'::text AS t1,'integer'::text AS t2,
    'bigint'::text AS t5, 'bigint'::text AS t6,
    'double precision'::text AS t7, 'double precision'::text AS t8;

SELECT set_eq('v21q00', 'v21q01','Expected columns names & types in version 2.4');


-- CHECKING WORKS WITH & WITOUT REVERSE COST

PREPARE v20q1 AS
SELECT * FROM pgr_bddijkstra(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, 3, true);

PREPARE v20q3 AS
SELECT * FROM pgr_bddijkstra(
    'SELECT id, source, target, cost FROM edge_table',
    2, 3, false);

SELECT lives_ok('v20q1','with reverse cost');
SELECT lives_ok('v20q3','with NO reverse cost');


SELECT finish();
ROLLBACK;
