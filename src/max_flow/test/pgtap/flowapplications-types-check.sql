\i setup.sql
SET client_min_messages TO WARNING;

SELECT plan(71);

SELECT has_function('pgr_edgedisjointpaths');

SELECT has_function('pgr_edgedisjointpaths', ARRAY[
    'text', 'bigint', 'bigint', 'boolean'
    ]);
SELECT has_function('pgr_edgedisjointpaths', ARRAY[
    'text', 'bigint', 'anyarray', 'boolean'
    ]);
SELECT has_function('pgr_edgedisjointpaths', ARRAY[
    'text', 'anyarray', 'bigint', 'boolean'
    ]);
SELECT has_function('pgr_edgedisjointpaths', ARRAY[
    'text', 'anyarray', 'anyarray', 'boolean'
    ]);

SELECT has_function('pgr_maximumcardinalitymatching');

SELECT has_function('pgr_maximumcardinalitymatching', ARRAY[
    'text', 'boolean'
    ]);

SELECT function_returns('pgr_edgedisjointpaths', ARRAY[
    'text', 'bigint', 'bigint', 'boolean'
    ], 'setof record');
SELECT function_returns('pgr_edgedisjointpaths', ARRAY[
    'text', 'bigint', 'anyarray', 'boolean'
    ], 'setof record');
SELECT function_returns('pgr_edgedisjointpaths', ARRAY[
    'text', 'anyarray', 'bigint', 'boolean'
    ], 'setof record');
SELECT function_returns('pgr_edgedisjointpaths', ARRAY[
    'text', 'anyarray', 'anyarray', 'boolean'
    ], 'setof record');

SELECT function_returns('pgr_maximumcardinalitymatching', ARRAY[
    'text', 'boolean'
    ], 'setof record');

-- Maximum cardinality matching
PREPARE mcm_dir AS
SELECT * FROM pgr_maximumCardinalityMatching(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table'
);
PREPARE mcm_undir AS
SELECT * FROM pgr_maximumCardinalityMatching(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
    directed := false
);

SELECT lives_ok('mcm_dir','directed');
SELECT lives_ok('mcm_undir','undirected');

PREPARE mcm_q AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(edge_id)::text AS t2,
    pg_typeof(source)::text AS t3, pg_typeof(target)::text AS t4
    FROM (
        SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table'
        ) ) AS a
    limit 1
;

PREPARE mcm_v AS
SELECT  'integer'::text AS t1,'bigint'::text AS t2,
    'bigint'::text AS t3, 'bigint'::text AS t4;

SELECT set_eq('mcm_q', 'mcm_v','Expected returning, columns names & types');

PREPARE mcm_id1 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::SMALLINT,
                source::INTEGER, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table'
);
PREPARE mcm_id2 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::BIGINT,
                source::INTEGER, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table'
);
PREPARE mcm_id3 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::REAL,
                source::INTEGER, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table'
);
PREPARE mcm_id4 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::FLOAT,
                source::INTEGER, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table'
);
PREPARE mcm_id5 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::TEXT,
                source::INTEGER, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table'
);

SELECT lives_ok('mcm_id1',
    'lives because id is SMALLINT');
SELECT lives_ok('mcm_id2',
    'lives because id is BIGINT');
SELECT throws_ok('mcm_id3',
    'XX000','Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'throws because id is REAL');
SELECT throws_ok('mcm_id4',
    'XX000','Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'throws because id is FLOAT');
SELECT throws_ok('mcm_id5',
    'XX000','Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'throws because id is TEXT');

-- source is ANY-INTEGER
PREPARE mcm_s1 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::INTEGER,
                source::SMALLINT, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table'
);
PREPARE mcm_s2 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::INTEGER,
                source::BIGINT, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table'
);
PREPARE mcm_s3 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::INTEGER,
                source::REAL, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table'
);
PREPARE mcm_s4 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::INTEGER,
                source::FLOAT, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table'
);
PREPARE mcm_s5 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::INTEGER,
                source::TEXT, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table'
);

SELECT lives_ok('mcm_s1',
    'lives because source is SMALLINT');
SELECT lives_ok('mcm_s2',
    'lives because source is BIGINT');
SELECT throws_ok('mcm_s3',
    'XX000','Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'throws because source is REAL');
SELECT throws_ok('mcm_s4',
    'XX000','Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'throws because source is FLOAT');
SELECT throws_ok('mcm_s5',
    'XX000','Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'throws because source is TEXT');
        
-- target is ANY-INTEGER
PREPARE mcm_t1 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::SMALLINT,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table'
);
PREPARE mcm_t2 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::BIGINT,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table'
);
PREPARE mcm_t3 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::REAL,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table'
);
PREPARE mcm_t4 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::FLOAT,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table'
);
PREPARE mcm_t5 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::TEXT,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table'
);

SELECT lives_ok('mcm_t1',
    'lives because target is SMALLINT');
SELECT lives_ok('mcm_t2',
    'lives because target is BIGINT');
SELECT throws_ok('mcm_t3',
    'XX000','Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'throws because target is REAL');
SELECT throws_ok('mcm_t4',
    'XX000','Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'throws because target is FLOAT');
SELECT throws_ok('mcm_t5',
    'XX000','Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'throws because target is TEXT');

-- GOING is ANY-NUMERIC
PREPARE mcm_g1 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::INTEGER,
                cost::SMALLINT AS going,
                reverse_cost::INTEGER AS coming FROM edge_table'
);
PREPARE mcm_g2 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::INTEGER,
                cost::BIGINT AS going,
                reverse_cost::INTEGER AS coming FROM edge_table'
);
PREPARE mcm_g3 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::INTEGER,
                cost::REAL AS going,
                reverse_cost::INTEGER AS coming FROM edge_table'
);
PREPARE mcm_g4 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::INTEGER,
                cost::FLOAT AS going,
                reverse_cost::INTEGER AS coming FROM edge_table'
);
PREPARE mcm_g5 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::INTEGER,
                cost::TEXT AS going,
                reverse_cost::INTEGER AS coming FROM edge_table'
);

SELECT lives_ok('mcm_g1',
    'lives because going is SMALLINT');
SELECT lives_ok('mcm_g2',
    'lives because going is BIGINT');
SELECT lives_ok('mcm_g3',
    'lives because going is REAL');
SELECT lives_ok('mcm_g4',
    'lives because going is FLOAT');
SELECT throws_ok('mcm_g5',
    'XX000','Unexpected Column ''going'' type. Expected ANY-NUMERICAL',
    'throws because going is TEXT');
        

-- coming is ANY-NUMERIC
PREPARE mcm_c1 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::SMALLINT AS coming FROM edge_table'
);
PREPARE mcm_c2 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::BIGINT AS coming FROM edge_table'
);
PREPARE mcm_c3 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::REAL AS coming FROM edge_table'
);
PREPARE mcm_c4 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::FLOAT AS coming FROM edge_table'
);
PREPARE mcm_c5 AS
SELECT * FROM pgr_maximumCardinalityMatching(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::TEXT AS coming FROM edge_table'
);

SELECT lives_ok('mcm_c1',
    'lives because coming is SMALLINT');
SELECT lives_ok('mcm_c2',
    'lives because coming is BIGINT');
SELECT lives_ok('mcm_c3',
    'lives because coming is REAL');
SELECT lives_ok('mcm_c4',
    'lives because coming is FLOAT');
SELECT throws_ok('mcm_c5',
    'XX000','Unexpected Column ''coming'' type. Expected ANY-NUMERICAL',
    'throws because coming is TEXT');


-- Edge disjoint paths
PREPARE edp_dir AS
SELECT * FROM pgr_edgeDisjointPaths(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
    3, 5
);
PREPARE edp_undir AS
SELECT * FROM pgr_edgeDisjointPaths(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
    3, 5,
    directed := false
);

SELECT lives_ok('edp_dir','directed');
SELECT lives_ok('edp_undir','undirected');

-- Boykov-Kolmogorov
PREPARE edp_q_o2o AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(path_seq)::text AS t2,
    pg_typeof(node)::text AS t3, pg_typeof(edge)::text AS t4
    FROM (
        SELECT * FROM pgr_edgeDisjointPaths(
            'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
        3, 5
        ) ) AS a
    limit 1
;
PREPARE edp_v_o2o AS
SELECT  'integer'::text AS t1,'integer'::text AS t2,
    'bigint'::text AS t3, 'bigint'::text AS t4;

PREPARE edp_q_o2m AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(path_seq)::text AS t2,
    pg_typeof(end_vid)::text as t3,
    pg_typeof(node)::text AS t4, pg_typeof(edge)::text AS t5
    FROM (
        SELECT * FROM pgr_edgeDisjointPaths(
            'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
        3, ARRAY[5]
        ) ) AS a
    limit 1
;
PREPARE edp_v_o2m AS
SELECT  'integer'::text AS t1,'integer'::text AS t2,
    'bigint'::text AS t3, 'bigint'::text AS t4,
    'bigint'::text AS t5;


PREPARE edp_q_m2o AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(path_seq)::text AS t2,
    pg_typeof(start_vid)::text as t3,
    pg_typeof(node)::text AS t4, pg_typeof(edge)::text AS t5
    FROM (
        SELECT * FROM pgr_edgeDisjointPaths(
            'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
        ARRAY[3], 5
        ) ) AS a
    limit 1
;
PREPARE edp_v_m2o AS
SELECT  'integer'::text AS t1,'integer'::text AS t2,
    'bigint'::text AS t3, 'bigint'::text AS t4,
    'bigint'::text AS t5;

PREPARE edp_q_m2m AS
SELECT pg_typeof(seq)::text AS t1, pg_typeof(path_seq)::text AS t2,
    pg_typeof(start_vid)::text as t3, pg_typeof(end_vid)::text as t4,
    pg_typeof(node)::text AS t5, pg_typeof(edge)::text AS t6
    FROM (
        SELECT * FROM pgr_edgeDisjointPaths(
            'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
        ARRAY[3], ARRAY[5]
        ) ) AS a
    limit 1
;
PREPARE edp_v_m2m AS
SELECT  'integer'::text AS t1,'integer'::text AS t2,
    'bigint'::text AS t3, 'bigint'::text AS t4,
    'bigint'::text AS t5, 'bigint'::text AS t6;

SELECT set_eq('edp_v_o2o', 'edp_q_o2o','1 to 1: Expected returning, columns names & types');
SELECT set_eq('edp_v_o2m', 'edp_q_o2m','1 to many: Expected returning, columns names & types');
SELECT set_eq('edp_v_m2o', 'edp_q_m2o','many to 1: Expected returning, columns names & types');
SELECT set_eq('edp_v_m2m', 'edp_q_m2m','many to many: Expected returning, columns names & types');

PREPARE edp_id1 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::SMALLINT,
                source::INTEGER, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table',
                3, 5
);
PREPARE edp_id2 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::BIGINT,
                source::INTEGER, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table',
                3, 5
);
PREPARE edp_id3 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::REAL,
                source::INTEGER, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table',
                3, 5
);
PREPARE edp_id4 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::FLOAT,
                source::INTEGER, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table',
                3, 5
);
PREPARE edp_id5 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::TEXT,
                source::INTEGER, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table',
                3, 5
);

SELECT lives_ok('edp_id1',
    'lives because id is SMALLINT');
SELECT lives_ok('edp_id2',
    'lives because id is BIGINT');
SELECT throws_ok('edp_id3',
    'XX000','Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'throws because id is REAL');
SELECT throws_ok('edp_id4',
    'XX000','Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'throws because id is FLOAT');
SELECT throws_ok('edp_id5',
    'XX000','Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'throws because id is TEXT');

-- source is ANY-INTEGER
PREPARE edp_s1 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::INTEGER,
                source::SMALLINT, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table',
                3, 5
);
PREPARE edp_s2 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::INTEGER,
                source::BIGINT, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table',
                3, 5
);
PREPARE edp_s3 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::INTEGER,
                source::REAL, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table',
                3, 5
);
PREPARE edp_s4 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::INTEGER,
                source::FLOAT, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table',
                3, 5
);
PREPARE edp_s5 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::INTEGER,
                source::TEXT, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table',
                3, 5
);

SELECT lives_ok('edp_s1',
    'lives because source is SMALLINT');
SELECT lives_ok('edp_s2',
    'lives because source is BIGINT');
SELECT throws_ok('edp_s3',
    'XX000','Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'throws because source is REAL');
SELECT throws_ok('edp_s4',
    'XX000','Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'throws because source is FLOAT');
SELECT throws_ok('edp_s5',
    'XX000','Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'throws because source is TEXT');
        
-- target is ANY-INTEGER
PREPARE edp_t1 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::SMALLINT,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table',
                3, 5
);
PREPARE edp_t2 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::BIGINT,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table',
                3, 5
);
PREPARE edp_t3 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::REAL,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table',
                3, 5
);
PREPARE edp_t4 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::FLOAT,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table',
                3, 5
);
PREPARE edp_t5 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::TEXT,
                cost::INTEGER AS going,
                reverse_cost::INTEGER AS coming FROM edge_table',
                3, 5
);

SELECT lives_ok('edp_t1',
    'lives because target is SMALLINT');
SELECT lives_ok('edp_t2',
    'lives because target is BIGINT');
SELECT throws_ok('edp_t3',
    'XX000','Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'throws because target is REAL');
SELECT throws_ok('edp_t4',
    'XX000','Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'throws because target is FLOAT');
SELECT throws_ok('edp_t5',
    'XX000','Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'throws because target is TEXT');

-- GOING is ANY-NUMERIC
PREPARE edp_g1 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::INTEGER,
                cost::SMALLINT AS going,
                reverse_cost::INTEGER AS coming FROM edge_table',
                3, 5
);
PREPARE edp_g2 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::INTEGER,
                cost::BIGINT AS going,
                reverse_cost::INTEGER AS coming FROM edge_table',
                3, 5
);
PREPARE edp_g3 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::INTEGER,
                cost::REAL AS going,
                reverse_cost::INTEGER AS coming FROM edge_table',
                3, 5
);
PREPARE edp_g4 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::INTEGER,
                cost::FLOAT AS going,
                reverse_cost::INTEGER AS coming FROM edge_table',
                3, 5
);
PREPARE edp_g5 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::INTEGER,
                cost::TEXT AS going,
                reverse_cost::INTEGER AS coming FROM edge_table',
                3, 5
);

SELECT lives_ok('edp_g1',
    'lives because going is SMALLINT');
SELECT lives_ok('edp_g2',
    'lives because going is BIGINT');
SELECT lives_ok('edp_g3',
    'lives because going is REAL');
SELECT lives_ok('edp_g4',
    'lives because going is FLOAT');
SELECT throws_ok('edp_g5',
    'XX000','Unexpected Column ''going'' type. Expected ANY-NUMERICAL',
    'throws because going is TEXT');
        

-- coming is ANY-NUMERIC
PREPARE edp_c1 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::SMALLINT AS coming FROM edge_table',
                3, 5
);
PREPARE edp_c2 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::BIGINT AS coming FROM edge_table',
                3, 5
);
PREPARE edp_c3 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::REAL AS coming FROM edge_table',
                3, 5
);
PREPARE edp_c4 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::FLOAT AS coming FROM edge_table',
                3, 5
);
PREPARE edp_c5 AS
SELECT * FROM pgr_edgeDisjointPaths(
        'SELECT id::INTEGER,
                source::INTEGER, 
                target::INTEGER,
                cost::INTEGER AS going,
                reverse_cost::TEXT AS coming FROM edge_table',
                3, 5
);

SELECT lives_ok('edp_c1',
    'lives because coming is SMALLINT');
SELECT lives_ok('edp_c2',
    'lives because coming is BIGINT');
SELECT lives_ok('edp_c3',
    'lives because coming is REAL');
SELECT lives_ok('edp_c4',
    'lives because coming is FLOAT');
SELECT throws_ok('edp_c5',
    'XX000','Unexpected Column ''coming'' type. Expected ANY-NUMERICAL',
    'throws because coming is TEXT');



SELECT * FROM finish();
ROLLBACK;