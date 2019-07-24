------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--                  PGR_createVerticesTable
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

--    SELECT pgr_createTopology('edge_table',0.001);
	 SELECT  pgr_createVerticesTable('edge_table');
	 SELECT  pgr_createVerticesTable('edge_table','the_geom','source','target');
	-- SELECT  pgr_createVerticesTable('edge_table','source','the_geom','target');
	 SELECT  pgr_createVerticesTable('edge_table',the_geom:='the_geom',source:='source',target:='target');
	 SELECT  pgr_createVerticesTable('edge_table',source:='source',target:='target',the_geom:='the_geom');
	 SELECT  pgr_createVerticesTable('edge_table',source:='source');
	 SELECT  pgr_createVerticesTable('edge_table',rows_where:='id < 10');
	 SELECT  pgr_createVerticesTable('edge_table',rows_where:='the_geom && (select st_buffer(the_geom,0.5) FROM edge_table WHERE id=5)');
	CREATE TABLE otherTable AS  (SELECT 100 AS gid, st_point(2.5,2.5) AS other_geom) ;
	 SELECT  pgr_createVerticesTable('edge_table',rows_where:='the_geom && (select st_buffer(other_geom,0.5) FROM otherTable WHERE gid=100)');
	CREATE TABLE mytable AS (SELECT id AS gid, the_geom AS mygeom,source AS src ,target AS tgt FROM edge_table) ;
	 SELECT  pgr_createVerticesTable('mytable','mygeom','src','tgt');
	-- SELECT  pgr_createVerticesTable('mytable','src','mygeom','tgt');
	 SELECT  pgr_createVerticesTable('mytable',the_geom:='mygeom',source:='src',target:='tgt');
	 SELECT  pgr_createVerticesTable('mytable',source:='src',target:='tgt',the_geom:='mygeom');
	 SELECT  pgr_createVerticesTable('mytable','mygeom','src','tgt',rows_where:='gid < 10');
	 SELECT  pgr_createVerticesTable('mytable',source:='src',target:='tgt',the_geom:='mygeom',rows_where:='gid < 10');
	 SELECT  pgr_createVerticesTable('mytable','mygeom','src','tgt',
	                            rows_where:='mygeom && (SELECT st_buffer(mygeom,0.5) FROM mytable WHERE gid=5)');
	 SELECT  pgr_createVerticesTable('mytable',source:='src',target:='tgt',the_geom:='mygeom',
	                            rows_where:='mygeom && (SELECT st_buffer(mygeom,0.5) FROM mytable WHERE gid=5)');
	DROP TABLE IF EXISTS otherTable;
	CREATE TABLE otherTable AS  (SELECT 100 AS gid, st_point(2.5,2.5) AS other_geom) ;
	 SELECT  pgr_createVerticesTable('mytable','mygeom','src','tgt',
	                            rows_where:='mygeom && (SELECT st_buffer(other_geom,0.5) FROM otherTable WHERE gid=100)');
	 SELECT  pgr_createVerticesTable('mytable',source:='src',target:='tgt',the_geom:='mygeom',
	                            rows_where:='mygeom && (SELECT st_buffer(other_geom,0.5) FROM otherTable WHERE gid=100)');

-------------------------------------
	SELECT pgr_createVerticesTable('edge_table');

