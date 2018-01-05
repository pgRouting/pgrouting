\i setup.sql


SELECT plan(6);

SELECT can(ARRAY['pgr_bdastar']);


--V2.4+
SELECT has_function('pgr_bdastar',
    ARRAY['text','bigint','bigint','boolean', 'integer', 'numeric', 'numeric']);
SELECT function_returns('pgr_bdastar',
    ARRAY['text','bigint','bigint','boolean', 'integer', 'numeric', 'numeric'],
    'setof record');

-- testing for the signature that they return the correct names & columns

PREPARE v21q00 AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(path_seq)::text AS t2,
    pg_typeof(node)::text AS t5, pg_typeof(edge)::text AS t6,
    pg_typeof(cost)::text AS t7, pg_typeof(agg_cost)::TEXT AS t8
    FROM (
        SELECT * FROM pgr_bdastar(
            'SELECT id, source, target, cost, reverse_cost, x1,y1,x2,y2 FROM edge_table',
            2, 3, true) ) AS a
    limit 1
;

PREPARE v21q01 AS
SELECT  'integer'::text AS t1,'integer'::text AS t2,
    'bigint'::text AS t5, 'bigint'::text AS t6,
    'double precision'::text AS t7, 'double precision'::text AS t8;

SELECT set_eq('v21q00', 'v21q01','Expected columns names & types in version 2.5');


-- CHECKING WORKS WITH & WITOUT REVERSE COST

PREPARE v20q1 AS
SELECT * FROM pgr_bdastar(
    'SELECT id, source, target, cost, reverse_cost, x1,y1,x2,y2 FROM edge_table',
    2, 3, true);

PREPARE v20q3 AS
SELECT * FROM pgr_bdastar(
    'SELECT id, source, target, cost, x1,y1,x2,y2 FROM edge_table',
    2, 3, false);

SELECT lives_ok('v20q1','with reverse cost');
SELECT lives_ok('v20q3','with NO reverse cost');


SELECT finish();
ROLLBACK;
