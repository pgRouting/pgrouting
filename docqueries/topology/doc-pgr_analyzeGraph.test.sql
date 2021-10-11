------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
--              PGR_analyzegraph
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------

/* -- q1 */
SELECT  pgr_createTopology('edge_table',0.001, clean := true);
SELECT  pgr_analyzeGraph('edge_table',0.001);
/* -- q1.1 */

/* -- q3 */
SELECT  pgr_analyzeGraph('edge_table',0.001,'the_geom','id','source','target');
/* -- q3.1 */

/* -- q5 */
SELECT  pgr_analyzeGraph('edge_table',0.001,'id','the_geom','source','target');
/* -- q5.1 */

/* -- q6 */
SELECT  pgr_analyzeGraph('edge_table',0.001,the_geom:='the_geom',id:='id',source:='source',target:='target');
/* -- q6.1 */

/* -- q7 */
SELECT  pgr_analyzeGraph('edge_table',0.001,source:='source',id:='id',target:='target',the_geom:='the_geom');
/* -- q7.1 */

/* -- q8. */
SELECT  pgr_analyzeGraph('edge_table',0.001,source:='source');
/* -- q8.1 */

/* -- q9 */
SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='id < 10');
/* -- q9.1 */

/* -- q10 */
SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='the_geom && (SELECT st_buffer(the_geom,0.05) FROM edge_table WHERE id=5)');
/* -- q10.1 */

/* -- q11 */
CREATE TABLE otherTable AS  (SELECT 100 AS gid, st_point(2.5,2.5) AS other_geom) ;
SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='the_geom && (SELECT st_buffer(other_geom,1) FROM otherTable WHERE gid=100)');
/* -- q11.1 */

/* -- q12 */
CREATE TABLE mytable AS (SELECT id AS gid, source AS src ,target AS tgt , the_geom AS mygeom FROM edge_table);
SELECT pgr_createTopology('mytable',0.001,'mygeom','gid','src','tgt', clean := true);
/* -- q12.1 */

/* -- q13 */
SELECT  pgr_analyzeGraph('mytable',0.001,'mygeom','gid','src','tgt');
/* -- q13.1 */

/* -- q14 */
SELECT  pgr_analyzeGraph('mytable',0.0001,'gid','mygeom','src','tgt');
/* -- q14.1 */

/* -- q15 */
SELECT  pgr_analyzeGraph('mytable',0.001,the_geom:='mygeom',id:='gid',source:='src',target:='tgt');
/* -- q15.1 */

/* -- q16 */
SELECT  pgr_analyzeGraph('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom');
/* -- q16.1 */

/* -- q17 */
SELECT  pgr_analyzeGraph('mytable',0.001,'mygeom','gid','src','tgt',rows_where:='gid < 10');
/* -- q17.1 */

/* -- q18 */
SELECT  pgr_analyzeGraph('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom',rows_where:='gid < 10');
/* -- q18.1 */

/* -- q19 */
SELECT  pgr_analyzeGraph('mytable',0.001,'mygeom','gid','src','tgt',
    rows_where:='mygeom && (SELECT st_buffer(mygeom,1) FROM mytable WHERE gid=5)');
/* -- q19.1 */

/* -- q20 */
SELECT  pgr_analyzeGraph('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom',
    rows_where:='mygeom && (SELECT st_buffer(mygeom,1) FROM mytable WHERE gid=5)');
/* -- q20.1 */

/* -- q21 */
DROP TABLE IF EXISTS otherTable;
CREATE TABLE otherTable AS  (SELECT 'myhouse'::text AS place, st_point(2.5,2.5) AS other_geom) ;
SELECT  pgr_analyzeGraph('mytable',0.001,'mygeom','gid','src','tgt',
    rows_where:='mygeom && (SELECT st_buffer(other_geom,1) FROM otherTable WHERE place='||quote_literal('myhouse')||')');
/* -- q21.1 */

/* -- q22 */
SELECT  pgr_analyzeGraph('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom',
    rows_where:='mygeom && (SELECT st_buffer(other_geom,1) FROM otherTable WHERE place='||quote_literal('myhouse')||')');
/* -- q22.1 */
--------------------

/* -- q23 */
SELECT  pgr_createTopology('edge_table',0.001, clean := true);
SELECT pgr_analyzeGraph('edge_table', 0.001);
/* -- q23.1 */

/* -- q24 */
SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='id < 10');
/* -- q24.1 */

/* -- q25 */
SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='id >= 10');
/* -- q25.1 */

/* -- q26 */
SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='id < 17');
/* -- q26.1 */

-- Simulate removal of edges

/* -- q27 */
SELECT pgr_createTopology('edge_table', 0.001,rows_where:='id <17', clean := true);
/* -- q27.1 */

/* -- q28 */
SELECT pgr_analyzeGraph('edge_table', 0.001);
/* -- q28.1 */
