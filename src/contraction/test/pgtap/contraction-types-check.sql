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
            'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT AS revcost FROM edge_table',
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
            'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT, reverse_cost::FLOAT AS revcost FROM edge_table',
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


SELECT finish();
ROLLBACK;

