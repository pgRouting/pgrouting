\i setup.sql

SET client_min_messages TO WARNING;

SELECT plan(4);

SELECT can(ARRAY['pgr_contractgraph']);

SELECT  todo_start('Remove the type pgr_contracted_blob');

SELECT function_returns('pgr_contractgraph',  ARRAY['text','bigint','boolean'], 'setof record');

SELECT todo_end();


--V2.1+
SELECT has_function('pgr_contractgraph', ARRAY['text','bigint','boolean']);

SELECT function_returns('pgr_contractgraph', ARRAY['text','bigint','boolean'],'setof pgr_contracted_blob');

-- testing for the 2 signatures that they return the correct names & columns
-- Preparing
PREPARE v21q00 AS
SELECT pg_typeof(contracted_graph_name)::text AS t1, 
       pg_typeof(contracted_graph_blob )::text AS t2,
       pg_typeof(removedVertices)::text AS t3,
       pg_typeof(removedEdges)::text AS t4,
       pg_typeof(psuedoEdges)::text AS t5 
    FROM ( 
        SELECT * FROM pgr_contractgraph(
            'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table',
            0,false) ) AS a
    limit 1
;

PREPARE v21q01 AS
SELECT  'text'::text AS t1,'text'::text AS t2,
    'text'::text AS t3, 'text'::text AS t4,
    'text'::text AS t5;

PREPARE v21q10 AS
SELECT pg_typeof(contracted_graph_name)::text AS t1, 
       pg_typeof(contracted_graph_blob )::text AS t2,
       pg_typeof(removedVertices)::text AS t3,
       pg_typeof(removedEdges)::text AS t4,
       pg_typeof(psuedoEdges)::text AS t5
    FROM ( 
        SELECT * FROM pgr_contractgraph(
            'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT FROM edge_table',
            1, false) ) AS a
    limit 1
;

PREPARE v21q11 AS
SELECT  'text'::text AS t1,'text'::text AS t2,
    'text'::text AS t3, 'text'::text AS t4,
    'text'::text AS t5;


-- testing
SELECT set_eq('v21q00', 'v21q01','Level 0: Expected returning, columns names & types');
SELECT set_eq('v21q10', 'v21q11','Level 1: Expected returning, columns names & types');


-- V2.0
-- CHECKING INNER QUERY
-- ANY-INTEGER, ANY-INTEGER, ANY-INTEGER, ANY-NUMERIC, [ANY-NUMERIC]

/* TEST USES THE EDGES_INPUT FOR 5 PARAMETERS */

--id ANY-INTEGER
PREPARE v200id1 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::SMALLINT, source::INTEGER, target::INTEGER, cost::FLOAT FROM edge_table',
     0, false);
PREPARE v200id2 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::BIGINT, source::INTEGER, target::INTEGER, cost::FLOAT FROM edge_table',
     0, false);
PREPARE v200id3 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::FLOAT, source::INTEGER, target::INTEGER, cost::FLOAT FROM edge_table',
     0, false);
PREPARE v200id4 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::REAL, source::INTEGER, target::INTEGER, cost::FLOAT FROM edge_table',
     0, false);
PREPARE v200id5 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::TEXT, source::INTEGER, target::INTEGER, cost::FLOAT FROM edge_table',
     0, false);

SELECT lives_ok('v200id1',
    'lives because id is SMALLINT');
SELECT lives_ok('v200id2',
    'lives because id is BIGINT');
SELECT throws_ok('v200id3',
    'XX000','Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'throws because id is REAL');
SELECT throws_ok('v200id4',
    'XX000','Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'throws because id is FLOAT');
SELECT throws_ok('v200id5',
    'XX000','Unexpected Column ''id'' type. Expected ANY-INTEGER',
    'throws because id is TEXT');

--source is only integer
PREPARE v200s1 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::INTEGER, source::SMALLINT, target::INTEGER, cost::FLOAT FROM edge_table',
    0, false);
PREPARE v200s2 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::INTEGER, source::BIGINT, target::INTEGER, cost::FLOAT FROM edge_table',
    0, false);
PREPARE v200s3 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::INTEGER, source::REAL, target::INTEGER, cost::FLOAT FROM edge_table',
    0, false);
PREPARE v200s4 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::INTEGER, source::FLOAT, target::INTEGER, cost::FLOAT FROM edge_table',
    0, false);
PREPARE v200s5 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::INTEGER, source::TEXT, target::INTEGER, cost::FLOAT FROM edge_table',
    0, false);

SELECT lives_ok('v200s1',
    'lives because source is SMALLINT');
SELECT lives_ok('v200s2',
    'lives because source is BIGINT');
SELECT throws_ok('v200s3',
    'XX000','Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'throws because source is REAL');
SELECT throws_ok('v200s4',
    'XX000','Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'throws because source is FLOAT');
SELECT throws_ok('v200s5',
    'XX000','Unexpected Column ''source'' type. Expected ANY-INTEGER',
    'throws because id is TEXT');


--target is only integer
PREPARE v200t1 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::INTEGER, source::INTEGER, target::SMALLINT, cost::FLOAT FROM edge_table',
    0, false);
PREPARE v200t2 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::INTEGER, source::INTEGER, target::BIGINT, cost::FLOAT FROM edge_table',
    0, false);
PREPARE v200t3 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::INTEGER, source::INTEGER, target::FLOAT, cost::FLOAT FROM edge_table',
    0, false);
PREPARE v200t4 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::INTEGER, source::INTEGER, target::REAL, cost::FLOAT FROM edge_table',
    0, false);
PREPARE v200t5 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::INTEGER, source::INTEGER, target::TEXT, cost::FLOAT FROM edge_table',
    0, false);

SELECT lives_ok('v200t1',
    'lives because target is SMALLINT');
SELECT lives_ok('v200t2',
    'lives because target is BIGINT');
SELECT throws_ok('v200t3',
    'XX000','Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'throws because target is REAL');
SELECT throws_ok('v200t4',
    'XX000','Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'throws because target is FLOAT');
SELECT throws_ok('v200t5',
    'XX000','Unexpected Column ''target'' type. Expected ANY-INTEGER',
    'throws because target is TEXT');

-- cost
PREPARE v200c1 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::SMALLINT FROM edge_table',
    0, false);
PREPARE v200c2 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::INTEGER FROM edge_table',
    0, false);
PREPARE v200c3 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::BIGINT FROM edge_table',
    0, false);
PREPARE v200c4 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::REAL FROM edge_table',
    0, false);
PREPARE v200c5 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::TEXT FROM edge_table',
    0, false);

SELECT lives_ok('v200c1',
    'lives because cost is SMALLINT');
SELECT lives_ok('v200c2',
    'lives because cost is INTEGER');
SELECT lives_ok('v200c3',
    'lives because cost is BIGINT');
SELECT lives_ok('v200c4',
    'lives because cost is REAL');
SELECT throws_ok('v200c5',
    'XX000','Unexpected Column ''cost'' type. Expected ANY-NUMERICAL',
    'throws because cost is TEXT');

-- reverse_cost
PREPARE v200r1 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::INTEGER, reverse_cost::SMALLINT FROM edge_table',
    0, false);
PREPARE v200r2 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::INTEGER, reverse_cost::INTEGER FROM edge_table',
    0, false);
PREPARE v200r3 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::INTEGER, reverse_cost::BIGINT FROM edge_table',
    0, false);
PREPARE v200r4 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::INTEGER, reverse_cost::REAL FROM edge_table',
    0, false);
PREPARE v200r5 AS
SELECT * FROM pgr_contractgraph(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::INTEGER, reverse_cost::TEXT FROM edge_table',
    0, false);

SELECT lives_ok('v200r1',
    'lives because reverse_cost is SMALLINT');
SELECT lives_ok('v200r2',
    'lives because reverse_cost is INTEGER');
SELECT lives_ok('v200r3',
    'lives because reverse_cost is BIGINT');
SELECT lives_ok('v200r4',
    'lives because reverse_cost is REAL');
SELECT throws_ok('v200r5',
    'XX000','Unexpected Column ''reverse_cost'' type. Expected ANY-NUMERICAL',
    'throws because reverse_cost is TEXT');



SELECT finish();
ROLLBACK;

