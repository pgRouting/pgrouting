\i setup.sql
SET client_min_messages TO WARNING;

SELECT plan(48);
--Boykov-Kolmogorov
SELECT has_function('pgr_maxflowboykovkolmogorov');

SELECT has_function('pgr_maxflowboykovkolmogorov', ARRAY[
    'text', 'bigint', 'bigint'
    ]);
SELECT has_function('pgr_maxflowboykovkolmogorov', ARRAY[
    'text', 'anyarray', 'bigint'
    ]);
SELECT has_function('pgr_maxflowboykovkolmogorov', ARRAY[
    'text', 'bigint', 'anyarray'
    ]);
SELECT has_function('pgr_maxflowboykovkolmogorov', ARRAY[
    'text', 'anyarray', 'anyarray'
    ]);

SELECT function_returns('pgr_maxflowboykovkolmogorov', ARRAY[
    'text', 'bigint', 'bigint'
    ], 'setof record');
SELECT function_returns('pgr_maxflowboykovkolmogorov', ARRAY[
    'text', 'bigint', 'anyarray'
    ], 'setof record');
SELECT function_returns('pgr_maxflowboykovkolmogorov', ARRAY[
    'text', 'anyarray', 'bigint'
    ], 'setof record');
SELECT function_returns('pgr_maxflowboykovkolmogorov', ARRAY[
    'text', 'anyarray', 'anyarray'
    ], 'setof record');

--Edmonds-Karp
SELECT has_function('pgr_maxflowedmondskarp');

SELECT has_function('pgr_maxflowedmondskarp', ARRAY[
    'text', 'bigint', 'bigint'
    ]);
SELECT has_function('pgr_maxflowedmondskarp', ARRAY[
    'text', 'anyarray', 'bigint'
    ]);
SELECT has_function('pgr_maxflowedmondskarp', ARRAY[
    'text', 'bigint', 'anyarray'
    ]);
SELECT has_function('pgr_maxflowedmondskarp', ARRAY[
    'text', 'anyarray', 'anyarray'
    ]);

SELECT function_returns('pgr_maxflowedmondskarp', ARRAY[
    'text', 'bigint', 'bigint'
    ], 'setof record');
SELECT function_returns('pgr_maxflowedmondskarp', ARRAY[
    'text', 'bigint', 'anyarray'
    ], 'setof record');
SELECT function_returns('pgr_maxflowedmondskarp', ARRAY[
    'text', 'anyarray', 'bigint'
    ], 'setof record');
SELECT function_returns('pgr_maxflowedmondskarp', ARRAY[
    'text', 'anyarray', 'anyarray'
    ], 'setof record');

--Push-Relabel
SELECT has_function('pgr_maxflowpushrelabel');

SELECT has_function('pgr_maxflowpushrelabel', ARRAY[
    'text', 'bigint', 'bigint'
    ]);
SELECT has_function('pgr_maxflowpushrelabel', ARRAY[
    'text', 'anyarray', 'bigint'
    ]);
SELECT has_function('pgr_maxflowpushrelabel', ARRAY[
    'text', 'bigint', 'anyarray'
    ]);
SELECT has_function('pgr_maxflowpushrelabel', ARRAY[
    'text', 'anyarray', 'anyarray'
    ]);


SELECT function_returns('pgr_maxflowpushrelabel', ARRAY[
    'text', 'bigint', 'bigint'
    ], 'setof record');
SELECT function_returns('pgr_maxflowpushrelabel', ARRAY[
    'text', 'bigint', 'anyarray'
    ], 'setof record');
SELECT function_returns('pgr_maxflowpushrelabel', ARRAY[
    'text', 'anyarray', 'bigint'
    ], 'setof record');
SELECT function_returns('pgr_maxflowpushrelabel', ARRAY[
    'text', 'anyarray', 'anyarray'
    ], 'setof record');

--pgr_max_flow
SELECT has_function('pgr_maxflow');

SELECT has_function('pgr_maxflow', ARRAY[
    'text', 'bigint', 'bigint'
    ]);
SELECT has_function('pgr_maxflow', ARRAY[
    'text', 'anyarray', 'bigint'
    ]);
SELECT has_function('pgr_maxflow', ARRAY[
    'text', 'bigint', 'anyarray'
    ]);
SELECT has_function('pgr_maxflow', ARRAY[
    'text', 'anyarray', 'anyarray'
    ]);


SELECT function_returns('pgr_maxflow', ARRAY[
    'text', 'bigint', 'bigint'
    ], 'bigint');
SELECT function_returns('pgr_maxflow', ARRAY[
    'text', 'bigint', 'anyarray'
    ], 'bigint');
SELECT function_returns('pgr_maxflow', ARRAY[
    'text', 'anyarray', 'bigint'
    ], 'bigint');
SELECT function_returns('pgr_maxflow', ARRAY[
    'text', 'anyarray', 'anyarray'
    ], 'bigint');

-- Push-relabel
PREPARE pl_q_o2o AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(edge_id)::text AS t2,
    pg_typeof(source)::text AS t3, pg_typeof(target)::text AS t4,
    pg_typeof(flow)::text AS t5, pg_typeof(residual_capacity)::TEXT AS t6
    FROM (
        SELECT * FROM pgr_maxflowpushrelabel(
            'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
            2, 3) ) AS a
    limit 1
;
PREPARE pl_v_o2o AS
SELECT  'integer'::text AS t1,'bigint'::text AS t2,
    'bigint'::text AS t3, 'bigint'::text AS t4,
    'bigint'::text AS t5, 'bigint'::text AS t6;

PREPARE pl_q_o2m AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(edge_id)::text AS t2,
    pg_typeof(source)::text AS t3, pg_typeof(target)::text AS t4,
    pg_typeof(flow)::text AS t5, pg_typeof(residual_capacity)::TEXT AS t6
    FROM (
        SELECT * FROM pgr_maxflowpushrelabel(
            'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
            2, ARRAY[3]) ) AS a
    limit 1
;
PREPARE pl_v_o2m AS
SELECT  'integer'::text AS t1,'bigint'::text AS t2,
    'bigint'::text AS t3, 'bigint'::text AS t4,
    'bigint'::text AS t5, 'bigint'::text AS t6;


PREPARE pl_q_m2o AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(edge_id)::text AS t2,
    pg_typeof(source)::text AS t3, pg_typeof(target)::text AS t4,
    pg_typeof(flow)::text AS t5, pg_typeof(residual_capacity)::TEXT AS t6
    FROM (
        SELECT * FROM pgr_maxflowpushrelabel(
            'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
            ARRAY[2], 3) ) AS a
    limit 1
;
PREPARE pl_v_m2o AS
SELECT  'integer'::text AS t1,'bigint'::text AS t2,
    'bigint'::text AS t3, 'bigint'::text AS t4,
    'bigint'::text AS t5, 'bigint'::text AS t6;

PREPARE pl_q_m2m AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(edge_id)::text AS t2,
    pg_typeof(source)::text AS t3, pg_typeof(target)::text AS t4,
    pg_typeof(flow)::text AS t5, pg_typeof(residual_capacity)::TEXT AS t6
    FROM (
        SELECT * FROM pgr_maxflowpushrelabel(
            'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
            ARRAY[2], ARRAY[3]) ) AS a
    limit 1
;
PREPARE pl_v_m2m AS
SELECT  'integer'::text AS t1,'bigint'::text AS t2,
    'bigint'::text AS t3, 'bigint'::text AS t4,
    'bigint'::text AS t5, 'bigint'::text AS t6;

-- Edmonds-Karp
PREPARE ek_q_o2o AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(edge_id)::text AS t2,
    pg_typeof(source)::text AS t3, pg_typeof(target)::text AS t4,
    pg_typeof(flow)::text AS t5, pg_typeof(residual_capacity)::TEXT AS t6
    FROM (
        SELECT * FROM pgr_maxflowedmondskarp(
            'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
            2, 3) ) AS a
    limit 1
;
PREPARE ek_v_o2o AS
SELECT  'integer'::text AS t1,'bigint'::text AS t2,
    'bigint'::text AS t3, 'bigint'::text AS t4,
    'bigint'::text AS t5, 'bigint'::text AS t6;

PREPARE ek_q_o2m AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(edge_id)::text AS t2,
    pg_typeof(source)::text AS t3, pg_typeof(target)::text AS t4,
    pg_typeof(flow)::text AS t5, pg_typeof(residual_capacity)::TEXT AS t6
    FROM (
        SELECT * FROM pgr_maxflowedmondskarp(
            'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
            2, ARRAY[3]) ) AS a
    limit 1
;

PREPARE ek_v_o2m AS
SELECT  'integer'::text AS t1,'bigint'::text AS t2,
    'bigint'::text AS t3, 'bigint'::text AS t4,
    'bigint'::text AS t5, 'bigint'::text AS t6;


PREPARE ek_q_m2o AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(edge_id)::text AS t2,
    pg_typeof(source)::text AS t3, pg_typeof(target)::text AS t4,
    pg_typeof(flow)::text AS t5, pg_typeof(residual_capacity)::TEXT AS t6
    FROM (
        SELECT * FROM pgr_maxflowedmondskarp(
            'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
            ARRAY[2], 3) ) AS a
    limit 1
;
PREPARE ek_v_m2o AS
SELECT  'integer'::text AS t1,'bigint'::text AS t2,
    'bigint'::text AS t3, 'bigint'::text AS t4,
    'bigint'::text AS t5, 'bigint'::text AS t6;

PREPARE ek_q_m2m AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(edge_id)::text AS t2,
    pg_typeof(source)::text AS t3, pg_typeof(target)::text AS t4,
    pg_typeof(flow)::text AS t5, pg_typeof(residual_capacity)::TEXT AS t6
    FROM (
        SELECT * FROM pgr_maxflowedmondskarp(
            'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
            ARRAY[2], ARRAY[3]) ) AS a
    limit 1
;
PREPARE ek_v_m2m AS
SELECT  'integer'::text AS t1,'bigint'::text AS t2,
    'bigint'::text AS t3, 'bigint'::text AS t4,
    'bigint'::text AS t5, 'bigint'::text AS t6;

-- Boykov-Kolmogorov
PREPARE bk_q_o2o AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(edge_id)::text AS t2,
    pg_typeof(source)::text AS t3, pg_typeof(target)::text AS t4,
    pg_typeof(flow)::text AS t5, pg_typeof(residual_capacity)::TEXT AS t6
    FROM (
        SELECT * FROM pgr_maxflowboykovkolmogorov(
            'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
            2, 3) ) AS a
    limit 1
;
PREPARE bk_v_o2o AS
SELECT  'integer'::text AS t1,'bigint'::text AS t2,
    'bigint'::text AS t3, 'bigint'::text AS t4,
    'bigint'::text AS t5, 'bigint'::text AS t6;

PREPARE bk_q_o2m AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(edge_id)::text AS t2,
    pg_typeof(source)::text AS t3, pg_typeof(target)::text AS t4,
    pg_typeof(flow)::text AS t5, pg_typeof(residual_capacity)::TEXT AS t6
    FROM (
        SELECT * FROM pgr_maxflowboykovkolmogorov(
            'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
            2, ARRAY[3]) ) AS a
    limit 1
;
PREPARE bk_v_o2m AS
SELECT  'integer'::text AS t1,'bigint'::text AS t2,
    'bigint'::text AS t3, 'bigint'::text AS t4,
    'bigint'::text AS t5, 'bigint'::text AS t6;


PREPARE bk_q_m2o AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(edge_id)::text AS t2,
    pg_typeof(source)::text AS t3, pg_typeof(target)::text AS t4,
    pg_typeof(flow)::text AS t5, pg_typeof(residual_capacity)::TEXT AS t6
    FROM (
        SELECT * FROM pgr_maxflowboykovkolmogorov(
            'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
            ARRAY[2], 3) ) AS a
    limit 1
;
PREPARE bk_v_m2o AS
SELECT  'integer'::text AS t1,'bigint'::text AS t2,
    'bigint'::text AS t3, 'bigint'::text AS t4,
    'bigint'::text AS t5, 'bigint'::text AS t6;

PREPARE bk_q_m2m AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(edge_id)::text AS t2,
    pg_typeof(source)::text AS t3, pg_typeof(target)::text AS t4,
    pg_typeof(flow)::text AS t5, pg_typeof(residual_capacity)::TEXT AS t6
    FROM (
        SELECT * FROM pgr_maxflowboykovkolmogorov(
            'SELECT id, source, target, capacity, reverse_capacity FROM edge_table',
            ARRAY[2], ARRAY[3]) ) AS a
    limit 1
;
PREPARE bk_v_m2m AS
SELECT  'integer'::text AS t1,'bigint'::text AS t2,
    'bigint'::text AS t3, 'bigint'::text AS t4,
    'bigint'::text AS t5, 'bigint'::text AS t6;

-- testing
-- Push-relabel
SELECT set_eq('pl_v_o2o', 'pl_q_o2o','1 to 1: Expected returning, columns names & types');
SELECT set_eq('pl_v_o2m', 'pl_q_o2m','1 to many: Expected returning, columns names & types');
SELECT set_eq('pl_v_m2o', 'pl_q_m2o','many to 1: Expected returning, columns names & types');
SELECT set_eq('pl_v_m2m', 'pl_q_m2m','many to many: Expected returning, columns names & types');

-- Edmonds-Karp
SELECT set_eq('ek_v_o2o', 'ek_q_o2o','1 to 1: Expected returning, columns names & types');
SELECT set_eq('ek_v_o2m', 'ek_q_o2m','1 to many: Expected returning, columns names & types');
SELECT set_eq('ek_v_m2o', 'ek_q_m2o','many to 1: Expected returning, columns names & types');
SELECT set_eq('ek_v_m2m', 'ek_q_m2m','many to many: Expected returning, columns names & types');

-- Boykov-Kolmogorov
SELECT set_eq('bk_v_o2o', 'bk_q_o2o','1 to 1: Expected returning, columns names & types');
SELECT set_eq('bk_v_o2m', 'bk_q_o2m','1 to many: Expected returning, columns names & types');
SELECT set_eq('bk_v_m2o', 'bk_q_m2o','many to 1: Expected returning, columns names & types');
SELECT set_eq('bk_v_m2m', 'bk_q_m2m','many to many: Expected returning, columns names & types');
