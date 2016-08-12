\i setup.sql
SET client_min_messages TO WARNING;

SELECT plan(114);
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

-- Push-relabel
PREPARE pl_q_o2o AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(edge_id)::text AS t2,
    pg_typeof(source)::text AS t3, pg_typeof(target)::text AS t4,
    pg_typeof(flow)::text AS t5, pg_typeof(residual_capacity)::TEXT AS t6
    FROM (
        SELECT * FROM pgr_maxflowpushrelabel(
            'SELECT id,
            source,
            target,
            c1.capacity as capacity,
            c2.capacity as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
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
            'SELECT id,
            source,
            target,
            c1.capacity as capacity,
            c2.capacity as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
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
            'SELECT id,
            source,
            target,
            c1.capacity as capacity,
            c2.capacity as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
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
            'SELECT id,
            source,
            target,
            c1.capacity as capacity,
            c2.capacity as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
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
            'SELECT id,
            source,
            target,
            c1.capacity as capacity,
            c2.capacity as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
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
            'SELECT id,
            source,
            target,
            c1.capacity as capacity,
            c2.capacity as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
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
            'SELECT id,
            source,
            target,
            c1.capacity as capacity,
            c2.capacity as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
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
            'SELECT id,
            source,
            target,
            c1.capacity as capacity,
            c2.capacity as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
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
            'SELECT id,
            source,
            target,
            c1.capacity as capacity,
            c2.capacity as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
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
            'SELECT id,
            source,
            target,
            c1.capacity as capacity,
            c2.capacity as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
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
            'SELECT id,
            source,
            target,
            c1.capacity as capacity,
            c2.capacity as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
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
            'SELECT id,
            source,
            target,
            c1.capacity as capacity,
            c2.capacity as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
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


-- Testing inner query

-- Push-relabel
-- id is ANY-INTEGER
PREPARE pl_id1 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::SMALLINT,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE pl_id2 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::BIGINT,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE pl_id3 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::REAL,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE pl_id4 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::FLOAT,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE pl_id5 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::TEXT,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);

SELECT lives_ok('pl_id1',
    'lives because id is SMALLINT');
SELECT lives_ok('pl_id2',
    'lives because id is BIGINT');
SELECT throws_ok('pl_id3',
    'XX000','Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'throws because id is REAL');
SELECT throws_ok('pl_id4',
    'XX000','Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'throws because id is FLOAT');
SELECT throws_ok('pl_id5',
    'XX000','Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'throws because id is TEXT');

-- source is ANY-INTEGER
PREPARE pl_s1 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::INTEGER,
            source::SMALLINT,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE pl_s2 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::INTEGER,
            source::BIGINT,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE pl_s3 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::INTEGER,
            source::REAL,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE pl_s4 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::INTEGER,
            source::FLOAT,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE pl_s5 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::INTEGER,
            source::TEXT,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);

SELECT lives_ok('pl_s1',
    'lives because source is SMALLINT');
SELECT lives_ok('pl_s2',
    'lives because source is BIGINT');
SELECT throws_ok('pl_s3',
    'XX000','Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'throws because source is REAL');
SELECT throws_ok('pl_s4',
    'XX000','Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'throws because source is FLOAT');
SELECT throws_ok('pl_s5',
    'XX000','Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'throws because source is TEXT');
        
-- target is ANY-INTEGER
PREPARE pl_t1 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::SMALLINT,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE pl_t2 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::BIGINT,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE pl_t3 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::REAL,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE pl_t4 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::FLOAT,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE pl_t5 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::TEXT,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);

SELECT lives_ok('pl_t1',
    'lives because target is SMALLINT');
SELECT lives_ok('pl_t2',
    'lives because target is BIGINT');
SELECT throws_ok('pl_t3',
    'XX000','Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'throws because target is REAL');
SELECT throws_ok('pl_t4',
    'XX000','Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'throws because target is FLOAT');
SELECT throws_ok('pl_t5',
    'XX000','Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'throws because target is TEXT');

-- capacity is ANY-INTEGER
PREPARE pl_c1 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::SMALLINT as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE pl_c2 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::BIGINT as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE pl_c3 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::REAL as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE pl_c4 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::FLOAT as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE pl_c5 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::TEXT as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);

SELECT lives_ok('pl_c1',
    'lives because capacity is SMALLINT');
SELECT lives_ok('pl_c2',
    'lives because capacity is BIGINT');
SELECT throws_ok('pl_c3',
    'XX000','Unexpected Column ''capacity'' type. Expected ANY-INTEGER',
    'throws because capacity is REAL');
SELECT throws_ok('pl_c4',
    'XX000','Unexpected Column ''capacity'' type. Expected ANY-INTEGER',
    'throws because capacity is FLOAT');
SELECT throws_ok('pl_c5',
    'XX000','Unexpected Column ''capacity'' type. Expected ANY-INTEGER',
    'throws because capacity is TEXT');

-- reverse_capacity is ANY-INTEGER
PREPARE pl_rc1 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::SMALLINT as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE pl_rc2 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::BIGINT as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE pl_rc3 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::REAL as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE pl_rc4 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::FLOAT as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE pl_rc5 AS
SELECT * FROM pgr_maxflowpushrelabel(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::TEXT as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);

SELECT lives_ok('pl_rc1',
    'lives because reverse_capacity is SMALLINT');
SELECT lives_ok('pl_rc2',
    'lives because reverse_capacity is BIGINT');
SELECT throws_ok('pl_rc3',
    'XX000','Unexpected Column ''reverse_capacity'' type. Expected ANY-INTEGER',
    'throws because reverse_capacity is REAL');
SELECT throws_ok('pl_rc4',
    'XX000','Unexpected Column ''reverse_capacity'' type. Expected ANY-INTEGER',
    'throws because reverse_capacity is FLOAT');
SELECT throws_ok('pl_rc5',
    'XX000','Unexpected Column ''reverse_capacity'' type. Expected ANY-INTEGER',
    'throws because reverse_capacity is TEXT');


-- Edmonds-Karp
-- id is ANY-INTEGER
PREPARE ek_id1 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::SMALLINT,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE ek_id2 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::BIGINT,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE ek_id3 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::REAL,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE ek_id4 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::FLOAT,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE ek_id5 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::TEXT,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);

SELECT lives_ok('ek_id1',
    'lives because id is SMALLINT');
SELECT lives_ok('ek_id2',
    'lives because id is BIGINT');
SELECT throws_ok('ek_id3',
    'XX000','Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'throws because id is REAL');
SELECT throws_ok('ek_id4',
    'XX000','Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'throws because id is FLOAT');
SELECT throws_ok('ek_id5',
    'XX000','Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'throws because id is TEXT');

-- source is ANY-INTEGER
PREPARE ek_s1 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::INTEGER,
            source::SMALLINT,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE ek_s2 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::INTEGER,
            source::BIGINT,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE ek_s3 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::INTEGER,
            source::REAL,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE ek_s4 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::INTEGER,
            source::FLOAT,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE ek_s5 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::INTEGER,
            source::TEXT,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);

SELECT lives_ok('ek_s1',
    'lives because source is SMALLINT');
SELECT lives_ok('ek_s2',
    'lives because source is BIGINT');
SELECT throws_ok('ek_s3',
    'XX000','Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'throws because source is REAL');
SELECT throws_ok('ek_s4',
    'XX000','Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'throws because source is FLOAT');
SELECT throws_ok('ek_s5',
    'XX000','Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'throws because source is TEXT');
        
-- target is ANY-INTEGER
PREPARE ek_t1 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::SMALLINT,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE ek_t2 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::BIGINT,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE ek_t3 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::REAL,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE ek_t4 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::FLOAT,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE ek_t5 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::TEXT,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);

SELECT lives_ok('ek_t1',
    'lives because target is SMALLINT');
SELECT lives_ok('ek_t2',
    'lives because target is BIGINT');
SELECT throws_ok('ek_t3',
    'XX000','Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'throws because target is REAL');
SELECT throws_ok('ek_t4',
    'XX000','Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'throws because target is FLOAT');
SELECT throws_ok('ek_t5',
    'XX000','Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'throws because target is TEXT');

-- capacity is ANY-INTEGER
PREPARE ek_c1 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::SMALLINT as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE ek_c2 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::BIGINT as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE ek_c3 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::REAL as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE ek_c4 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::FLOAT as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE ek_c5 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::TEXT as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);

SELECT lives_ok('ek_c1',
    'lives because capacity is SMALLINT');
SELECT lives_ok('ek_c2',
    'lives because capacity is BIGINT');
SELECT throws_ok('ek_c3',
    'XX000','Unexpected Column ''capacity'' type. Expected ANY-INTEGER',
    'throws because capacity is REAL');
SELECT throws_ok('ek_c4',
    'XX000','Unexpected Column ''capacity'' type. Expected ANY-INTEGER',
    'throws because capacity is FLOAT');
SELECT throws_ok('ek_c5',
    'XX000','Unexpected Column ''capacity'' type. Expected ANY-INTEGER',
    'throws because capacity is TEXT');

-- reverse_capacity is ANY-INTEGER
PREPARE ek_rc1 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::SMALLINT as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE ek_rc2 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::BIGINT as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE ek_rc3 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::REAL as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE ek_rc4 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::FLOAT as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE ek_rc5 AS
SELECT * FROM pgr_maxflowedmondskarp(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::TEXT as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);

SELECT lives_ok('ek_rc1',
    'lives because reverse_capacity is SMALLINT');
SELECT lives_ok('ek_rc2',
    'lives because reverse_capacity is BIGINT');
SELECT throws_ok('ek_rc3',
    'XX000','Unexpected Column ''reverse_capacity'' type. Expected ANY-INTEGER',
    'throws because reverse_capacity is REAL');
SELECT throws_ok('ek_rc4',
    'XX000','Unexpected Column ''reverse_capacity'' type. Expected ANY-INTEGER',
    'throws because reverse_capacity is FLOAT');
SELECT throws_ok('ek_rc5',
    'XX000','Unexpected Column ''reverse_capacity'' type. Expected ANY-INTEGER',
    'throws because reverse_capacity is TEXT');

-- Edmonds-Karp
-- id is ANY-INTEGER
PREPARE bk_id1 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::SMALLINT,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE bk_id2 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::BIGINT,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE bk_id3 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::REAL,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE bk_id4 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::FLOAT,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE bk_id5 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::TEXT,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);

SELECT lives_ok('bk_id1',
    'lives because id is SMALLINT');
SELECT lives_ok('bk_id2',
    'lives because id is BIGINT');
SELECT throws_ok('bk_id3',
    'XX000','Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'throws because id is REAL');
SELECT throws_ok('bk_id4',
    'XX000','Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'throws because id is FLOAT');
SELECT throws_ok('bk_id5',
    'XX000','Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'throws because id is TEXT');

-- source is ANY-INTEGER
PREPARE bk_s1 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::INTEGER,
            source::SMALLINT,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE bk_s2 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::INTEGER,
            source::BIGINT,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE bk_s3 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::INTEGER,
            source::REAL,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE bk_s4 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::INTEGER,
            source::FLOAT,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE bk_s5 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::INTEGER,
            source::TEXT,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);

SELECT lives_ok('bk_s1',
    'lives because source is SMALLINT');
SELECT lives_ok('bk_s2',
    'lives because source is BIGINT');
SELECT throws_ok('bk_s3',
    'XX000','Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'throws because source is REAL');
SELECT throws_ok('bk_s4',
    'XX000','Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'throws because source is FLOAT');
SELECT throws_ok('bk_s5',
    'XX000','Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'throws because source is TEXT');
        
-- target is ANY-INTEGER
PREPARE bk_t1 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::SMALLINT,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE bk_t2 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::BIGINT,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE bk_t3 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::REAL,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE bk_t4 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::FLOAT,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE bk_t5 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::TEXT,
            c1.capacity::INTEGER as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);

SELECT lives_ok('bk_t1',
    'lives because target is SMALLINT');
SELECT lives_ok('bk_t2',
    'lives because target is BIGINT');
SELECT throws_ok('bk_t3',
    'XX000','Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'throws because target is REAL');
SELECT throws_ok('bk_t4',
    'XX000','Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'throws because target is FLOAT');
SELECT throws_ok('bk_t5',
    'XX000','Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'throws because target is TEXT');

-- capacity is ANY-INTEGER
PREPARE bk_c1 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::SMALLINT as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE bk_c2 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::BIGINT as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE bk_c3 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::REAL as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE bk_c4 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::FLOAT as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE bk_c5 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::TEXT as capacity,
            c2.capacity::INTEGER as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);

SELECT lives_ok('bk_c1',
    'lives because capacity is SMALLINT');
SELECT lives_ok('bk_c2',
    'lives because capacity is BIGINT');
SELECT throws_ok('bk_c3',
    'XX000','Unexpected Column ''capacity'' type. Expected ANY-INTEGER',
    'throws because capacity is REAL');
SELECT throws_ok('bk_c4',
    'XX000','Unexpected Column ''capacity'' type. Expected ANY-INTEGER',
    'throws because capacity is FLOAT');
SELECT throws_ok('bk_c5',
    'XX000','Unexpected Column ''capacity'' type. Expected ANY-INTEGER',
    'throws because capacity is TEXT');

-- reverse_capacity is ANY-INTEGER
PREPARE bk_rc1 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::SMALLINT as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE bk_rc2 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::BIGINT as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE bk_rc3 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::REAL as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE bk_rc4 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::FLOAT as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);
PREPARE bk_rc5 AS
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id::INTEGER,
            source::INTEGER,
            target::INTEGER,
            c1.capacity::INTEGER as capacity,
            c2.capacity::TEXT as reverse_capacity
    FROM edge_table JOIN categories AS c1 USING(category_id), categories AS c2
    WHERE edge_table.reverse_category_id = c2.category_id',
    2, 3);

SELECT lives_ok('bk_rc1',
    'lives because reverse_capacity is SMALLINT');
SELECT lives_ok('bk_rc2',
    'lives because reverse_capacity is BIGINT');
SELECT throws_ok('bk_rc3',
    'XX000','Unexpected Column ''reverse_capacity'' type. Expected ANY-INTEGER',
    'throws because reverse_capacity is REAL');
SELECT throws_ok('bk_rc4',
    'XX000','Unexpected Column ''reverse_capacity'' type. Expected ANY-INTEGER',
    'throws because reverse_capacity is FLOAT');
SELECT throws_ok('bk_rc5',
    'XX000','Unexpected Column ''reverse_capacity'' type. Expected ANY-INTEGER',
    'throws because reverse_capacity is TEXT');

SELECT * FROM finish();
ROLLBACK;