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

-- create a new network with harder cases
CREATE TABLE original_hard (
      id serial NOT NULL PRIMARY KEY
);
SELECT addgeometrycolumn('original_hard', 'the_geom', 2154, 'LINESTRING', 2);


INSERT INTO original_hard (the_geom)
VALUES ('SRID=2154;LineString (843731.96837530436459929 6520224.37815147917717695, 843720.94724064960610121 6520301.25479774083942175, 843720.39054300379939377 6520305.10738470777869225, 843720.05595255247317255 6520307.44336138013750315)');
INSERT INTO original_hard (the_geom)
VALUES ('SRID=2154;LineString (843681.62641362368594855 6520255.06193162966519594, 843726.45216981507837772 6520262.85580295417457819)');
INSERT INTO original_hard (the_geom)
VALUES ('SRID=2154;LineString (840244.02465303440112621 6519477.50229732040315866, 840223.44937284057959914 6519479.99790611304342747, 840211.45880026079248637 6519479.72390817571431398, 840199.53777986904606223 6519474.36406740732491016, 840190.58289291337132454 6519464.75101129896938801, 840181.41248250298667699 6519451.97837869636714458, 840170.42565293482039124 6519434.3100799722597003, 840168.55425586295314133 6519430.80164159927517176, 840151.01772376440931112 6519396.24404330458492041, 840139.96531795407645404 6519372.58710443042218685, 840138.63576878234744072 6519367.48039372358471155, 840146.19430478441063315 6519303.77102805394679308, 840150.080136300297454 6519274.91238123551011086, 840119.29845047416165471 6519150.59983278997242451, 840117.11253627086989582 6519135.32088061515241861, 840114.83395825664047152 6519129.54824385419487953, 840108.43866820889525115 6519120.18260038271546364, 840107.31546254467684776 6519117.61322824936360121, 840099.29079487535636872 6519102.64529608283191919)');
INSERT INTO original_hard (the_geom)
VALUES ('SRID=2154;LineString (840225.44422620115801692 6519369.90784595627337694, 840164.1949518381152302 6519407.88627272564917803, 840153.53890225966461003 6519408.40932532399892807, 840127.25395868462510407 6519400.12230102065950632, 840102.82493207952938974 6519361.2863945122808218, 840094.13878045929595828 6519332.39617275167256594)');


SELECT is((SELECT count(*)::INTEGER FROM original_hard), 4, 'we have 4 original_hard edges');
SELECT pgr_nodeNetwork('original_hard', 0.1, 'id', 'the_geom');
SELECT is((SELECT count(*)::INTEGER FROM original_hard_noded), 7, 'Now we have 7 edges');


PREPARE q1_hard AS
SELECT old_id, count(*) FROM original_hard_noded GROUP BY old_id ORDER BY old_id;
prepare vals1_hard AS
VALUES (1,2),(2,1),(3,2),(4,2);
SELECT set_eq('q1_hard', 'vals1_hard',
    'For each original_hard edge we have now 2 subedges except for the second one');


SELECT is((SELECT count(*)::INTEGER FROM original_hard_noded WHERE source is NULL), (SELECT count(*)::INTEGER FROM original_hard_noded), 'all edges are missing source');
SELECT is((SELECT count(*)::INTEGER FROM original_hard_noded WHERE target is NULL), (SELECT count(*)::INTEGER FROM original_hard_noded), 'all edges are missing target');
SELECT hasnt_table('original_hard_noded_vertices_pgr', 'original_hard_noded_vertices_pgr table does not exist');
SELECT pgr_createtopology('original_hard_noded', 0.000001);
SELECT has_table('original_hard_noded_vertices_pgr', 'original_hard_noded_vertices_pgr table now exist');
SELECT is((SELECT count(*)::INTEGER FROM original_hard_noded WHERE source is NULL), 0, '0 edges are missing source');
SELECT is((SELECT count(*)::INTEGER FROM original_hard_noded WHERE target is NULL), 0, '0 edges are missing target');
SELECT is((SELECT count(*)::INTEGER FROM original_hard_noded_vertices_pgr), 9, 'Now we have 9 vertices');
SELECT is((SELECT count(*)::INTEGER FROM original_hard_noded_vertices_pgr WHERE cnt is NULL), 9, '9 vertices are missing cnt');
SELECT is((SELECT count(*)::INTEGER FROM original_hard_noded_vertices_pgr WHERE chk is NULL), 9, '9 vertices are missing chk');
SELECT is((SELECT count(*)::INTEGER FROM original_hard_noded_vertices_pgr WHERE ein is NULL), 9, '9 vertices are missing ein');
SELECT is((SELECT count(*)::INTEGER FROM original_hard_noded_vertices_pgr WHERE eout is NULL), 9, '9 vertices are missing eout');


SELECT pgr_analyzegraph('original_hard_noded',  0.000001);
SELECT is((SELECT count(*)::INTEGER FROM original_hard_noded_vertices_pgr WHERE cnt is NULL), 0, '0 vertices are missing cnt');
SELECT is((SELECT count(*)::INTEGER FROM original_hard_noded_vertices_pgr WHERE chk is NULL), 0, '0 vertices are missing chk');
SELECT is((SELECT count(*)::INTEGER FROM original_hard_noded_vertices_pgr WHERE ein is NULL), 9, '9 vertices are missing ein');
SELECT is((SELECT count(*)::INTEGER FROM original_hard_noded_vertices_pgr WHERE eout is NULL), 9, '9 vertices are missing eout');
SELECT is((SELECT count(*)::INTEGER FROM original_hard_noded_vertices_pgr WHERE chk = 0), 9, 'In 9 vertices chk=0 aka have no problem');
PREPARE q2_hard AS
SELECT cnt, count(*) AS M  FROM original_hard_noded_vertices_pgr GROUP BY cnt ORDER BY cnt;
PREPARE vals2_hard AS
VALUES (1,7), (3, 1), (4,1);
SELECT set_eq('q2_hard', 'vals2_hard',
        'vertices referenced correctly by edges');


SELECT finish();
ROLLBACK;


