------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--                pgr_createTopology
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

\set VERBOSITY terse

DROP TABLE edge_table_vertices_pgr;
UPDATE edge_table SET source = NULL,  target = NULL;
\echo --q1
SELECT  pgr_createTopology('edge_table', 0.001);
\echo --q1.1

DROP TABLE edge_table_vertices_pgr;
UPDATE edge_table SET source = NULL,  target = NULL;
\echo --q2
SELECT  pgr_createTopology('edge_table', 0.001,
    'the_geom', 'id', 'source', 'target');
\echo --q2.1

SET client_min_messages TO NOTICE;
\echo --q3
SELECT  pgr_createTopology('edge_table', 0.001,
    'id', 'the_geom');
\echo --q3.1

SET client_min_messages TO WARNING;
DROP TABLE edge_table_vertices_pgr;
UPDATE edge_table SET source = NULL,  target = NULL;
\echo --q4
SELECT  pgr_createTopology('edge_table', 0.001,
    the_geom:='the_geom', id:='id', source:='source', target:='target');
\echo --q4.1

DROP TABLE edge_table_vertices_pgr;
UPDATE edge_table SET source = NULL,  target = NULL;
\echo --q5
SELECT  pgr_createTopology('edge_table', 0.001,
    source:='source', id:='id', target:='target', the_geom:='the_geom');
\echo --q5.1

DROP TABLE edge_table_vertices_pgr;
UPDATE edge_table SET source = NULL,  target = NULL;
\echo --q6
SELECT  pgr_createTopology('edge_table', 0.001, source:='source');
\echo --q6.1

\echo --q7
SELECT  pgr_createTopology('edge_table', 0.001, rows_where:='id < 10');
\echo --q7.1

\echo --q8
SELECT  pgr_createTopology('edge_table', 0.001,
    rows_where:='the_geom && (SELECT st_buffer(the_geom, 0.05) FROM edge_table WHERE id=5)');
\echo --q8.1

\echo --q9
CREATE TABLE otherTable AS  (SELECT 100 AS gid,  st_point(2.5, 2.5) AS other_geom);
SELECT  pgr_createTopology('edge_table', 0.001,
    rows_where:='the_geom && (SELECT st_buffer(other_geom, 1) FROM otherTable WHERE gid=100)');
\echo --q9.1

-- THE NEXT SECTION

\echo --q10
CREATE TABLE mytable AS (SELECT id AS gid,  the_geom AS mygeom, source AS src , target AS tgt FROM edge_table) ;
\echo --q10.1

\echo --q11
SELECT  pgr_createTopology('mytable', 0.001, 'mygeom', 'gid', 'src', 'tgt', clean := TRUE);
\echo --q11.1

DROP TABLE mytable_vertices_pgr;
UPDATE mytable SET src = NULL,  tgt = NULL;
SET client_min_messages TO NOTICE;
\echo --q12
SELECT  pgr_createTopology('mytable', 0.001, 'gid', 'mygeom', 'src', 'tgt');
\echo --q12.1
SET client_min_messages TO WARNING;

\echo --q13
SELECT  pgr_createTopology('mytable', 0.001, the_geom:='mygeom', id:='gid', source:='src', target:='tgt');
\echo --q13.1

DROP TABLE mytable_vertices_pgr;
UPDATE mytable SET src = NULL,  tgt = NULL;
\echo --q14
SELECT  pgr_createTopology('mytable', 0.001, source:='src', id:='gid', target:='tgt', the_geom:='mygeom');
\echo --q14.1

\echo --q15
SELECT  pgr_createTopology('mytable', 0.001, 'mygeom', 'gid', 'src', 'tgt', rows_where:='gid < 10');
SELECT  pgr_createTopology('mytable', 0.001, source:='src', id:='gid', target:='tgt', the_geom:='mygeom', rows_where:='gid < 10');
SELECT  pgr_createTopology('mytable', 0.001, 'mygeom', 'gid', 'src', 'tgt',
    rows_where:='mygeom && (SELECT st_buffer(mygeom, 1) FROM mytable WHERE gid=5)');
SELECT  pgr_createTopology('mytable', 0.001, source:='src', id:='gid', target:='tgt', the_geom:='mygeom',
    rows_where:='mygeom && (SELECT st_buffer(mygeom, 1) FROM mytable WHERE gid=5)');
\echo --q15.1

\echo --q16
SELECT  pgr_createTopology('mytable', 0.001, 'mygeom', 'gid', 'src', 'tgt',
    rows_where:='mygeom && (SELECT st_buffer(other_geom, 1) FROM otherTable WHERE gid=100)');
SELECT  pgr_createTopology('mytable', 0.001, source:='src', id:='gid', target:='tgt', the_geom:='mygeom',
    rows_where:='mygeom && (SELECT st_buffer(other_geom, 1) FROM otherTable WHERE gid=100)');
\echo --q16.1

-------------------------------------------------------------------------------
SET client_min_messages TO NOTICE;
\echo --q17
SELECT pgr_createTopology('edge_table',  0.001, rows_where:='id < 6', clean := true);
SELECT pgr_createTopology('edge_table',  0.001);
\echo --q17.1

SET client_min_messages TO NOTICE;
