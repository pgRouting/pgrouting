-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/


/* --q1 */
SELECT  pgr_createVerticesTable('edges', 'geom');
/* --q1.1 */

/* --q2 */
SELECT  pgr_createVerticesTable('edges', 'geom', 'source', 'target');
/* --q2.1 */
SELECT  pgr_createVerticesTable('edges', 'source', 'geom', 'target');
/* --q2.2 */

/* --q3.1 */
SELECT  pgr_createVerticesTable('edges', the_geom:='geom', source:='source', target:='target');
/* --q3.2 */

/* --q4 */
SELECT  pgr_createVerticesTable('edges', source:='source', target:='target', the_geom:='geom');
/* --q4.1 */

/* --q5 */
SELECT  pgr_createVerticesTable('edges', 'geom', source:='source');
/* --q5.1 */

/* --q6 */
SELECT  pgr_createVerticesTable('edges', 'geom', rows_where:='id < 10');
/* --q6.1 */

/* --q7 */
SELECT  pgr_createVerticesTable('edges', 'geom',
    rows_where:='geom && (select st_buffer(geom,0.5) FROM edges WHERE id=5)');
/* --q7.1 */

/* --q8 */
DROP TABLE IF EXISTS otherTable;
CREATE TABLE otherTable AS  (SELECT 100 AS gid, st_point(2.5,2.5) AS other_geom) ;
SELECT  pgr_createVerticesTable('edges', 'geom',
    rows_where:='geom && (select st_buffer(other_geom,0.5) FROM otherTable WHERE gid=100)');
/* --q8.1 */

/* --tab1 */
DROP TABLE IF EXISTS mytable;
CREATE TABLE mytable AS (SELECT id AS gid, geom AS mygeom, source AS src ,target AS tgt FROM edges) ;
/* --tab2 */

/* --q9 */
SELECT  pgr_createVerticesTable('mytable', 'mygeom', 'src', 'tgt');
/* --q9.1 */
SELECT  pgr_createVerticesTable('mytable', 'src', 'mygeom', 'tgt');
/* --q9.2 */

/* --q10 */
SELECT  pgr_createVerticesTable('mytable',the_geom:='mygeom',source:='src',target:='tgt');
/* --q10.1 */

/* --q11 */
SELECT  pgr_createVerticesTable(
    'mytable', source:='src', target:='tgt',
    the_geom:='mygeom');
/* --q11.1 */

/* --q12 */
SELECT  pgr_createVerticesTable(
    'mytable', 'mygeom', 'src', 'tgt',
    rows_where:='gid < 10');
/* --q12.1 */

/* --q13 */
SELECT  pgr_createVerticesTable(
    'mytable', source:='src', target:='tgt', the_geom:='mygeom',
    rows_where:='gid < 10');
/* --q13.1 */

/* --q14 */
SELECT  pgr_createVerticesTable(
    'mytable', 'mygeom', 'src', 'tgt',
    rows_where := 'the_geom && (SELECT st_buffer(mygeom,0.5) FROM mytable WHERE gid=5)');
/* --q14.1 */

/* --q15 */
SELECT  pgr_createVerticesTable(
    'mytable', source:='src', target:='tgt', the_geom:='mygeom',
    rows_where:='mygeom && (SELECT st_buffer(mygeom,0.5) FROM mytable WHERE id=5)');
/* --q15.1 */

/* --q16 */
DROP TABLE IF EXISTS otherTable;
CREATE TABLE otherTable AS  (SELECT 100 AS gid, st_point(2.5,2.5) AS other_geom) ;
/* --q16.1 */
SELECT pgr_createVerticesTable(
    'mytable', 'mygeom', 'src', 'tgt',
    rows_where:='the_geom && (SELECT st_buffer(othergeom,0.5) FROM otherTable WHERE gid=100)');
/* --q16.2 */

/* --q17 */
SELECT  pgr_createVerticesTable(
    'mytable',source:='src',target:='tgt',the_geom:='mygeom',
    rows_where:='the_geom && (SELECT st_buffer(othergeom,0.5) FROM otherTable WHERE gid=100)');
/* --q17.1 */
