------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              PGR_analyzegraph
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

\echo -- q1
SELECT  pgr_createTopology('edge_table',0.001, clean := true);
SELECT  pgr_analyzeGraph('edge_table',0.001);
\echo -- q1.1

\echo -- q3
SELECT  pgr_analyzeGraph('edge_table',0.001,'the_geom','id','source','target');
\echo -- q3.1

\echo -- q5
SELECT  pgr_analyzeGraph('edge_table',0.001,'id','the_geom','source','target');
\echo -- q5.1

\echo -- q6
SELECT  pgr_analyzeGraph('edge_table',0.001,the_geom:='the_geom',id:='id',source:='source',target:='target');
\echo -- q6.1

\echo -- q7
SELECT  pgr_analyzeGraph('edge_table',0.001,source:='source',id:='id',target:='target',the_geom:='the_geom');
\echo -- q7.1

\echo -- q8.
SELECT  pgr_analyzeGraph('edge_table',0.001,source:='source');
\echo -- q8.1

\echo -- q9
SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='id < 10');
\echo -- q9.1

\echo -- q10
SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='the_geom && (SELECT st_buffer(the_geom,0.05) FROM edge_table WHERE id=5)');
\echo -- q10.1

\echo -- q11
CREATE TABLE otherTable AS  (SELECT 100 AS gid, st_point(2.5,2.5) AS other_geom) ;
SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='the_geom && (SELECT st_buffer(other_geom,1) FROM otherTable WHERE gid=100)');
\echo -- q11.1

\echo -- q12
CREATE TABLE mytable AS (SELECT id AS gid, source AS src ,target AS tgt , the_geom AS mygeom FROM edge_table);
SELECT pgr_createTopology('mytable',0.001,'mygeom','gid','src','tgt', clean := true);
\echo -- q12.1

\echo -- q13
SELECT  pgr_analyzeGraph('mytable',0.001,'mygeom','gid','src','tgt');
\echo -- q13.1

\echo -- q14
SELECT  pgr_analyzeGraph('mytable',0.0001,'gid','mygeom','src','tgt');
\echo -- q14.1

\echo -- q15
SELECT  pgr_analyzeGraph('mytable',0.001,the_geom:='mygeom',id:='gid',source:='src',target:='tgt');
\echo -- q15.1

\echo -- q16
SELECT  pgr_analyzeGraph('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom');
\echo -- q16.1

\echo -- q17
SELECT  pgr_analyzeGraph('mytable',0.001,'mygeom','gid','src','tgt',rows_where:='gid < 10');
\echo -- q17.1

\echo -- q18
SELECT  pgr_analyzeGraph('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom',rows_where:='gid < 10');
\echo -- q18.1

\echo -- q19
SELECT  pgr_analyzeGraph('mytable',0.001,'mygeom','gid','src','tgt',
    rows_where:='mygeom && (SELECT st_buffer(mygeom,1) FROM mytable WHERE gid=5)');
\echo -- q19.1

\echo -- q20
SELECT  pgr_analyzeGraph('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom',
    rows_where:='mygeom && (SELECT st_buffer(mygeom,1) FROM mytable WHERE gid=5)');
\echo -- q20.1

\echo -- q21
DROP TABLE IF EXISTS otherTable;
CREATE TABLE otherTable AS  (SELECT 'myhouse'::text AS place, st_point(2.5,2.5) AS other_geom) ;
SELECT  pgr_analyzeGraph('mytable',0.001,'mygeom','gid','src','tgt',
    rows_where:='mygeom && (SELECT st_buffer(other_geom,1) FROM otherTable WHERE place='||quote_literal('myhouse')||')');
\echo -- q21.1

\echo -- q22
SELECT  pgr_analyzeGraph('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom',
    rows_where:='mygeom && (SELECT st_buffer(other_geom,1) FROM otherTable WHERE place='||quote_literal('myhouse')||')');
\echo -- q22.1
--------------------

\echo -- q23
SELECT  pgr_createTopology('edge_table',0.001, clean := true);
SELECT pgr_analyzeGraph('edge_table', 0.001);
\echo -- q23.1

\echo -- q24
SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='id < 10');
\echo -- q24.1

\echo -- q25
SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='id >= 10');
\echo -- q25.1

\echo -- q26
SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='id < 17');
\echo -- q26.1

-- Simulate removal of edges

\echo -- q27
SELECT pgr_createTopology('edge_table', 0.001,rows_where:='id <17', clean := true);
\echo -- q27.1

\echo -- q28
SELECT pgr_analyzeGraph('edge_table', 0.001);
\echo -- q28.1
