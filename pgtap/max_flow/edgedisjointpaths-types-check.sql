\i setup.sql

SELECT plan(10);


-- Edge disjoint paths
PREPARE edp_dir AS
SELECT * FROM pgr_edgeDisjointPaths(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    3, 5
);
PREPARE edp_undir AS
SELECT * FROM pgr_edgeDisjointPaths(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    3, 5,
    directed := false
);

SELECT lives_ok('edp_dir','directed');
SELECT lives_ok('edp_undir','undirected');


-- preparing return types pgr_edgeDisjointPaths(One to One)
PREPARE edp_v_o2o AS
SELECT
    'integer'::text AS t1,
    'integer'::text AS t2,
    'bigint'::text AS t3,
    'bigint'::text AS t4,
    'double precision'::text AS t5,
    'double precision'::text AS t6,
    'integer'::text AS t10;

PREPARE edp_q_o2o AS
SELECT pg_typeof(seq)::text AS t1,
    pg_typeof(path_seq)::text AS t2,
    pg_typeof(node)::text AS t3,
    pg_typeof(edge)::text AS t4,
    pg_typeof(cost)::text AS t5,
    pg_typeof(agg_cost)::text AS t6,
    pg_typeof(path_id)::text AS t10
    FROM (
        SELECT * FROM pgr_edgeDisjointPaths(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        3, 5
        ) ) AS a
    limit 1;



-- preparing return types for:
--    pgr_edgeDisjointPaths(One to Many)
--    pgr_edgeDisjointPaths(Many to One)
PREPARE edp_v_o2m AS
SELECT
    'integer'::text AS t1,
    'integer'::text AS t2,
    'bigint'::text AS t3,
    'bigint'::text AS t4,
    'bigint'::text AS t5,
    'double precision'::text AS t6,
    'double precision'::text AS t7,
    'integer'::text AS t10;

PREPARE edp_q_o2m AS
SELECT pg_typeof(seq)::text AS t1,
    pg_typeof(path_seq)::text AS t2,
    pg_typeof(end_vid)::text as t3,
    pg_typeof(node)::text AS t4,
    pg_typeof(edge)::text AS t5,
    pg_typeof(cost)::text AS t6,
    pg_typeof(agg_cost)::text AS t7,
    pg_typeof(path_id)::text AS t10
    FROM (
        SELECT * FROM pgr_edgeDisjointPaths(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        3, ARRAY[5]
        ) ) AS a
    limit 1
;

PREPARE edp_q_m2o AS
SELECT pg_typeof(seq)::text AS t1,
    pg_typeof(path_seq)::text AS t2,
    pg_typeof(start_vid)::text as t3,
    pg_typeof(node)::text AS t4,
    pg_typeof(edge)::text AS t5,
    pg_typeof(cost)::text AS t6,
    pg_typeof(agg_cost)::text AS t7,
    pg_typeof(path_id)::text AS t10
    FROM (
        SELECT * FROM pgr_edgeDisjointPaths(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        ARRAY[3], 5
        ) ) AS a
    limit 1;

-- preparing return types pgr_edgeDisjointPaths(Many to Many)
PREPARE edp_v_m2m AS
SELECT
    'integer'::text AS t1,
    'integer'::text AS t2,
    'bigint'::text AS t3,
    'bigint'::text AS t4,
    'bigint'::text AS t5,
    'bigint'::text AS t6,
    'double precision'::text AS t7,
    'double precision'::text AS t8,
    'integer'::text AS t10;

PREPARE edp_q_m2m AS
SELECT pg_typeof(seq)::text AS t1,
    pg_typeof(path_seq)::text AS t2,
    pg_typeof(start_vid)::text as t3,
    pg_typeof(end_vid)::text as t4,
    pg_typeof(node)::text AS t5,
    pg_typeof(edge)::text AS t6,
    pg_typeof(cost)::text AS t7,
    pg_typeof(agg_cost)::text AS t8,
    pg_typeof(path_id)::text AS t10
    FROM (
        SELECT * FROM pgr_edgeDisjointPaths(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        ARRAY[3], ARRAY[5]
        ) ) AS a
    limit 1
;

-- testing return types
SELECT set_eq('edp_v_o2o', 'edp_q_o2o','1 to 1: Expected returning, columns names & types');
SELECT set_eq('edp_v_o2m', 'edp_q_o2m','1 to many: Expected returning, columns names & types');
SELECT set_eq('edp_v_o2m', 'edp_q_m2o','many to 1: Expected returning, columns names & types');
SELECT set_eq('edp_v_m2m', 'edp_q_m2m','many to many: Expected returning, columns names & types');


-- testing column names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_edgedisjointpaths'$$,
    $$SELECT  '{"","","","directed","seq","path_id","path_seq","node","edge","cost","agg_cost"}'::TEXT[] $$
);
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_edgedisjointpaths'$$,
    $$SELECT  '{"","","","directed","seq","path_id","path_seq","end_vid","node","edge","cost","agg_cost"}'::TEXT[] $$
);
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_edgedisjointpaths'$$,
    $$SELECT  '{"","","","directed","seq","path_id","path_seq","start_vid","node","edge","cost","agg_cost"}'::TEXT[] $$
);
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_edgedisjointpaths'$$,
    $$SELECT  '{"","","","directed","seq","path_id","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[] $$
);

SELECT * FROM finish();
ROLLBACK;
