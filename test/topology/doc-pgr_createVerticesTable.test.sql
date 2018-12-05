

\echo --q1
SELECT  pgr_createVerticesTable('edge_table');
\echo --q1.1

\echo --q2
SELECT  pgr_createVerticesTable('edge_table', 'the_geom', 'source', 'target');
\echo --q2.1
SELECT  pgr_createVerticesTable('edge_table', 'source', 'the_geom', 'target');
\echo --q2.2

\echo --q3.1
SELECT  pgr_createVerticesTable('edge_table', the_geom:='the_geom', source:='source', target:='target');
\echo --q3.2

\echo --q4
SELECT  pgr_createVerticesTable('edge_table', source:='source', target:='target', the_geom:='the_geom');
\echo --q4.1

\echo --q5
SELECT  pgr_createVerticesTable('edge_table',source:='source');
\echo --q5.1

\echo --q6
SELECT  pgr_createVerticesTable('edge_table',rows_where:='id < 10');
\echo --q6.1

\echo --q7
SELECT  pgr_createVerticesTable('edge_table',
    rows_where:='the_geom && (select st_buffer(the_geom,0.5) FROM edge_table WHERE id=5)');
\echo --q7.1

\echo --q8
DROP TABLE IF EXISTS otherTable;
CREATE TABLE otherTable AS  (SELECT 100 AS gid, st_point(2.5,2.5) AS other_geom) ;
SELECT  pgr_createVerticesTable('edge_table',
    rows_where:='the_geom && (select st_buffer(other_geom,0.5) FROM otherTable WHERE gid=100)');
\echo --q8.1

\echo --tab1
DROP TABLE IF EXISTS mytable;
CREATE TABLE mytable AS (SELECT id AS gid, the_geom AS mygeom, source AS src ,target AS tgt FROM edge_table) ;
\echo --tab2

\echo --q9
SELECT  pgr_createVerticesTable('mytable', 'mygeom', 'src', 'tgt');
\echo --q9.1
SELECT  pgr_createVerticesTable('mytable', 'src', 'mygeom', 'tgt');
\echo --q9.2

\echo --q10
SELECT  pgr_createVerticesTable('mytable',the_geom:='mygeom',source:='src',target:='tgt');
\echo --q10.1

\echo --q11
SELECT  pgr_createVerticesTable(
    'mytable', source:='src', target:='tgt',
    the_geom:='mygeom');
\echo --q11.1

\echo --q12
SELECT  pgr_createVerticesTable(
    'mytable', 'mygeom', 'src', 'tgt',
    rows_where:='gid < 10');
\echo --q12.1

\echo --q13
SELECT  pgr_createVerticesTable(
    'mytable', source:='src', target:='tgt', the_geom:='mygeom',
    rows_where:='gid < 10');
\echo --q13.1

\echo --q14
SELECT  pgr_createVerticesTable(
    'mytable', 'mygeom', 'src', 'tgt',
    rows_where := 'the_geom && (SELECT st_buffer(mygeom,0.5) FROM mytable WHERE gid=5)');
\echo --q14.1

\echo --q15
SELECT  pgr_createVerticesTable(
    'mytable', source:='src', target:='tgt', the_geom:='mygeom',
    rows_where:='mygeom && (SELECT st_buffer(mygeom,0.5) FROM mytable WHERE id=5)');
\echo --q15.1

\echo --q16
DROP TABLE IF EXISTS otherTable;
CREATE TABLE otherTable AS  (SELECT 100 AS gid, st_point(2.5,2.5) AS other_geom) ;
\echo --q16.1
SELECT pgr_createVerticesTable(
    'mytable', 'mygeom', 'src', 'tgt',
    rows_where:='the_geom && (SELECT st_buffer(othergeom,0.5) FROM otherTable WHERE gid=100)');
\echo --q16.2

\echo --q17
SELECT  pgr_createVerticesTable(
    'mytable',source:='src',target:='tgt',the_geom:='mygeom',
    rows_where:='the_geom && (SELECT st_buffer(othergeom,0.5) FROM otherTable WHERE gid=100)');
\echo --q17.1
