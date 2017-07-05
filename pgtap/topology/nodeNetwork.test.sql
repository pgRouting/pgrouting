\i setup.sql

-- node the network
-- create 4x4 grid of overlapping lines horizontal and vertical
-- set the even numbered ones with dir='FT' and the odd ones 'TF'

SELECT plan(41);

SET client_min_messages = WARNING;
CREATE TABLE original (
      id serial NOT NULL primary key,
      dir text
);

SELECT addgeometrycolumn('original', 'the_geom', ST_SRID('POINT(0 0)'::geometry) , 'LINESTRING', 2);

-- creating the grid geometry
INSERT INTO original (dir, the_geom)
    SELECT case when s1%2=0 THEN 'FT' ELSE 'TF' END, st_astext(st_makeline(st_makepoint(1,s1), st_makepoint(6,s1))) FROM (SELECT generate_series(2,5) AS s1) AS foo
    UNION ALL
    SELECT case when s1%2=0 THEN 'FT' ELSE 'TF' END, st_astext(st_makeline(st_makepoint(s1,1), st_makepoint(s1,6))) FROM (select generate_series(2,5) AS s1) AS foo;


SELECT is((SELECT count(*)::INTEGER FROM original), 8, 'we have 8 original edges');
SELECT pgr_nodenetwork('original', 0.000001);
SELECT is((SELECT count(*)::INTEGER FROM original_noded), 40, 'Now we have 40 edges');
PREPARE q1 AS
SELECT old_id, count(*) FROM original_noded GROUP BY old_id ORDER BY old_id;
prepare vals1 AS
VALUES (1,5),(2,5),(3,5),(4,5),(5,5),(6,5),(7,5),(8,5);
SELECT set_eq('q1', 'vals1',
    'For each original edge we have now 5 subedges');


SELECT is((SELECT count(*)::INTEGER FROM original_noded WHERE source is NULL), 40, '40 edges are missing source');
SELECT is((SELECT count(*)::INTEGER FROM original_noded WHERE target is NULL), 40, '40 edges are missing target');
SELECT hasnt_table('original_noded_vertices_pgr', 'original_noded_vertices_pgr table does not exist');
SELECT pgr_createtopology('original_noded', 0.000001);
SELECT has_table('original_noded_vertices_pgr', 'original_noded_vertices_pgr table now exist');
SELECT is((SELECT count(*)::INTEGER FROM original_noded WHERE source is NULL), 0, '0 edges are missing source');
SELECT is((SELECT count(*)::INTEGER FROM original_noded WHERE target is NULL), 0, '0 edges are missing target');
SELECT is((SELECT count(*)::INTEGER FROM original_noded_vertices_pgr), 32, 'Now we have 32 vertices');
SELECT is((SELECT count(*)::INTEGER FROM original_noded_vertices_pgr WHERE cnt is NULL), 32, '32 vertices are missing cnt');
SELECT is((SELECT count(*)::INTEGER FROM original_noded_vertices_pgr WHERE chk is NULL), 32, '32 vertices are missing chk');
SELECT is((SELECT count(*)::INTEGER FROM original_noded_vertices_pgr WHERE ein is NULL), 32, '32 vertices are missing ein');
SELECT is((SELECT count(*)::INTEGER FROM original_noded_vertices_pgr WHERE eout is NULL), 32, '32 vertices are missing eout');


SELECT pgr_analyzegraph('original_noded',  0.000001);
SELECT is((SELECT count(*)::INTEGER FROM original_noded_vertices_pgr WHERE cnt is NULL), 0, '0 vertices are missing cnt');
SELECT is((SELECT count(*)::INTEGER FROM original_noded_vertices_pgr WHERE chk is NULL), 0, '0 vertices are missing chk');
SELECT is((SELECT count(*)::INTEGER FROM original_noded_vertices_pgr WHERE ein is NULL), 32, '32 vertices are missing ein');
SELECT is((SELECT count(*)::INTEGER FROM original_noded_vertices_pgr WHERE eout is NULL), 32, '32 vertices are missing eout');
SELECT is((SELECT count(*)::INTEGER FROM original_noded_vertices_pgr WHERE chk = 0), 32, 'In 32 vertices chk=0 aka have no problem');
PREPARE q2 AS
SELECT cnt, count(*) AS M  FROM original_noded_vertices_pgr GROUP BY cnt ORDER BY cnt;
PREPARE vals2 AS
VALUES (1,16), (4,16);
SELECT set_eq('q2', 'vals2',
        '(N,16) 16 vertices referenced by N edges');


-- create a new TABLE and merge attribute FROM original and noded
-- INTO TABLE noded

create TABLE noded (
  gid serial NOT NULL primary key,
  id INTEGER,
  dir text,
  source INTEGER,
  target INTEGER,
  cost FLOAT,
  reverse_cost FLOAT
 );

SELECT addgeometrycolumn('noded', 'the_geom', ST_SRID('POINT(0 0)'::geometry) , 'LINESTRING', 2);

INSERT INTO noded(id, dir, the_geom)
 SELECT a.old_id, b.dir, a.the_geom
   FROM original_noded a, original b
  WHERE a.old_id=b.id;

SELECT is((SELECT count(*)::INTEGER FROM noded), 40, 'Now we have 40 edges');
SELECT is(
(SELECT DISTINCT count(*)::INTEGER FROM original GROUP BY dir),
 4, '4 is the count of different TF and FT in original') ;
SELECT is(
(SELECT DISTINCT count(*)::INTEGER FROM noded GROUP BY dir),
 20, '20 is the count of different TF and FT in noded (4*5)') ;


SELECT is((SELECT count(*)::INTEGER FROM noded WHERE source is NULL), 40, '40 edges are missing source');
SELECT is((SELECT count(*)::INTEGER FROM noded WHERE target is NULL), 40, '40 edges are missing target');
SELECT hasnt_table('noded_vertices_pgr', 'noded_vertices_pgr table does not exist');
SELECT pgr_createtopology('noded', 0.000001,id:='gid');
SELECT has_table('noded_vertices_pgr', 'noded_vertices_pgr table now exist');
SELECT is((SELECT count(*)::INTEGER FROM noded WHERE source is NULL), 0, '0 edges are missing source');
SELECT is((SELECT count(*)::INTEGER FROM noded WHERE target is NULL), 0, '0 edges are missing target');
SELECT is((SELECT count(*)::INTEGER FROM noded_vertices_pgr), 32, 'Now we have 32 vertices');
SELECT is((SELECT count(*)::INTEGER FROM noded_vertices_pgr WHERE cnt is NULL), 32, '32 vertices are missing cnt');
SELECT is((SELECT count(*)::INTEGER FROM noded_vertices_pgr WHERE chk is NULL), 32, '32 vertices are missing chk');
SELECT is((SELECT count(*)::INTEGER FROM noded_vertices_pgr WHERE ein is NULL), 32, '32 vertices are missing ein');
SELECT is((SELECT count(*)::INTEGER FROM noded_vertices_pgr WHERE eout is NULL), 32, '32 vertices are missing eout');


SELECT pgr_analyzegraph('noded', 0.000001,id:='gid');
SELECT is((SELECT count(*)::INTEGER FROM noded_vertices_pgr WHERE cnt is NULL), 0, '0 vertices are missing cnt');
SELECT is((SELECT count(*)::INTEGER FROM noded_vertices_pgr WHERE chk is NULL), 0, '0 vertices are missing chk');
SELECT is((SELECT count(*)::INTEGER FROM noded_vertices_pgr WHERE ein is NULL), 32, '32 vertices are missing ein');
SELECT is((SELECT count(*)::INTEGER FROM noded_vertices_pgr WHERE eout is NULL), 32, '32 vertices are missing eout');
SELECT is((SELECT count(*)::INTEGER FROM noded_vertices_pgr WHERE chk = 0), 32, 'In 32 vertices chk=0 aka have no problem');

SELECT pgr_analyzeoneway('noded',
    ARRAY['', 'B', 'TF'],
    ARRAY['', 'B', 'FT'],
    ARRAY['', 'B', 'FT'],
    ARRAY['', 'B', 'TF'],
    oneway:='dir');
SELECT is((SELECT count(*)::INTEGER FROM noded_vertices_pgr WHERE ein is NULL), 0, '0 vertices are missing ein');
SELECT is((SELECT count(*)::INTEGER FROM noded_vertices_pgr WHERE eout is NULL), 0, '0 vertices are missing eout');


SELECT finish();
ROLLBACK;


