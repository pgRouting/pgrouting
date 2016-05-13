
BEGIN;


    ------------------------------------------------------------------------------------------------------
    ------------------------------------------------------------------------------------------------------
    --              PGR_analyzegraph
    ------------------------------------------------------------------------------------------------------
    ------------------------------------------------------------------------------------------------------
    --q01
    SELECT  pgr_create_topology('edge_table',0.001);
    --q02
    SELECT  pgr_analyzeGraph('edge_table',0.001);
    --q03
    SELECT  pgr_analyzeGraph('edge_table',0.001,'the_geom','id','source','target');
    --q04
    SELECT  pgr_analyzeGraph('edge_table',0.001,the_geom:='the_geom',id:='id',source:='source',target:='target');
    --q05
    SELECT  pgr_analyzeGraph('edge_table',0.001,source:='source',id:='id',target:='target',the_geom:='the_geom');
    --q06
    SELECT  pgr_analyzeGraph('edge_table',0.001,source:='source');
    --q07
    SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='id < 10');
    --q08
    SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='the_geom && (SELECT st_buffer(the_geom,0.05) FROM edge_table WHERE id=5)');
    --q09
    DROP TABLE IF EXISTS otherTable;
    --q10
    CREATE TABLE otherTable AS  (SELECT 100 AS gid, st_point(2.5,2.5) AS other_geom) ;
    --q11
    SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='the_geom && (SELECT st_buffer(other_geom,1) FROM otherTable WHERE gid=100)');
    --q12
    DROP TABLE IF EXISTS mytable;
    --q13
    CREATE TABLE mytable AS (SELECT id AS gid, source AS src ,target AS tgt , the_geom AS mygeom FROM edge_table);
    --q14
    SELECT pgr_createTopology('mytable',0.001,'mygeom','gid','src','tgt');
    --q15
    SELECT  pgr_analyzeGraph('mytable',0.001,'mygeom','gid','src','tgt');
    --q16
    SELECT  pgr_analyzeGraph('mytable',0.001,the_geom:='mygeom',id:='gid',source:='src',target:='tgt');
    --q17
    SELECT  pgr_analyzeGraph('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom');
    --q18
    SELECT  pgr_analyzeGraph('mytable',0.001,'mygeom','gid','src','tgt',rows_where:='gid < 10');
    --q19
    SELECT  pgr_analyzeGraph('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom',rows_where:='gid < 10');
    --q20
    SELECT  pgr_analyzeGraph('mytable',0.001,'mygeom','gid','src','tgt',
                                rows_where:='mygeom && (SELECT st_buffer(mygeom,1) FROM mytable WHERE gid=5)');

    --q21
    SELECT  pgr_analyzeGraph('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom',
                                rows_where:='mygeom && (SELECT st_buffer(mygeom,1) FROM mytable WHERE gid=5)');
    --q22
    DROP TABLE IF EXISTS otherTable;
    --q23
    CREATE TABLE otherTable AS  (SELECT 'myhouse'::text AS place, st_point(2.5,2.5) AS other_geom) ;
    --q24
    SELECT  pgr_analyzeGraph('mytable',0.001,'mygeom','gid','src','tgt',
                 rows_where:='mygeom && (SELECT st_buffer(other_geom,1) FROM otherTable WHERE place='||quote_literal('myhouse')||')');
    --q25
    SELECT  pgr_analyzeGraph('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom',
                 rows_where:='mygeom && (SELECT st_buffer(other_geom,1) FROM otherTable WHERE place='||quote_literal('myhouse')||')');
    --q26
    SELECT  pgr_create_topology('edge_table',0.001);
    --q27
    SELECT pgr_analyzeGraph('edge_table', 0.001);
    --q28
    SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='id < 10');
    --q29
    SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='id >= 10');
    --q30
    SELECT pgr_createTopology('edge_table', 0.001,rows_where:='id <17');
    --q31
    SELECT pgr_analyzeGraph('edge_table', 0.001);
    --q32
    SELECT pgr_createTopology('edge_table', 0.001,rows_where:='id <17');
    --q33
    SELECT pgr_analyzeGraph('edge_table', 0.001);

    ROLLBACK;
