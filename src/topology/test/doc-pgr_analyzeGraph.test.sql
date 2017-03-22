


------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              PGR_analyzegraph
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

SELECT  pgr_createTopology('edge_table',0.001, clean := true);
SELECT  pgr_analyzeGraph('edge_table',0.001);
SELECT  pgr_analyzeGraph('edge_table',0.001,'the_geom','id','source','target');
SELECT  pgr_analyzeGraph('edge_table',0.001,'id','the_geom','source','target');
SELECT  pgr_analyzeGraph('edge_table',0.001,the_geom:='the_geom',id:='id',source:='source',target:='target');
SELECT  pgr_analyzeGraph('edge_table',0.001,source:='source',id:='id',target:='target',the_geom:='the_geom');
SELECT  pgr_analyzeGraph('edge_table',0.001,source:='source');
SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='id < 10');
SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='the_geom && (SELECT st_buffer(the_geom,0.05) FROM edge_table WHERE id=5)');
CREATE TABLE otherTable AS  (SELECT 100 AS gid, st_point(2.5,2.5) AS other_geom) ;
SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='the_geom && (SELECT st_buffer(other_geom,1) FROM otherTable WHERE gid=100)');
CREATE TABLE mytable AS (SELECT id AS gid, source AS src ,target AS tgt , the_geom AS mygeom FROM edge_table);
SELECT pgr_createTopology('mytable',0.001,'mygeom','gid','src','tgt', clean := true);
SELECT  pgr_analyzeGraph('mytable',0.001,'mygeom','gid','src','tgt');
SELECT  pgr_analyzeGraph('mytable',0.0001,'gid','mygeom','src','tgt');
SELECT  pgr_analyzeGraph('mytable',0.001,the_geom:='mygeom',id:='gid',source:='src',target:='tgt');
SELECT  pgr_analyzeGraph('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom');
SELECT  pgr_analyzeGraph('mytable',0.001,'mygeom','gid','src','tgt',rows_where:='gid < 10');
SELECT  pgr_analyzeGraph('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom',rows_where:='gid < 10');
SELECT  pgr_analyzeGraph('mytable',0.001,'mygeom','gid','src','tgt',
    rows_where:='mygeom && (SELECT st_buffer(mygeom,1) FROM mytable WHERE gid=5)');
SELECT  pgr_analyzeGraph('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom',
    rows_where:='mygeom && (SELECT st_buffer(mygeom,1) FROM mytable WHERE gid=5)');
DROP TABLE IF EXISTS otherTable;
CREATE TABLE otherTable AS  (SELECT 'myhouse'::text AS place, st_point(2.5,2.5) AS other_geom) ;
SELECT  pgr_analyzeGraph('mytable',0.001,'mygeom','gid','src','tgt',
    rows_where:='mygeom && (SELECT st_buffer(other_geom,1) FROM otherTable WHERE place='||quote_literal('myhouse')||')');
SELECT  pgr_analyzeGraph('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom',
    rows_where:='mygeom && (SELECT st_buffer(other_geom,1) FROM otherTable WHERE place='||quote_literal('myhouse')||')');
--------------------
SELECT  pgr_createTopology('edge_table',0.001, clean := true);
SELECT pgr_analyzeGraph('edge_table', 0.001);
SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='id < 10');
SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='id >= 10');
SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='id < 17');

-- Simulate removal of edges
SELECT pgr_createTopology('edge_table', 0.001,rows_where:='id <17', clean := true);
SELECT pgr_analyzeGraph('edge_table', 0.001);

