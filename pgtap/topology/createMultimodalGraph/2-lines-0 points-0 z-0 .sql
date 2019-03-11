\i setup.sql

SET client_min_messages TO WARNING ;

SELECT plan(13);
DROP TABLE IF EXISTS test_table_l1;
CREATE  TABLE test_table_l1(
   id INTEGER PRIMARY  KEY,
   geom GEOMETRY('linestringz',4326)
);
--1rst group
INSERT INTO  test_table_l1 (geom, id) VALUES
 ('SRID=4326;linestring(5 0 0,10 10 0, 13 10 0, 15 10 0)',1),
 ('SRID=4326;linestring(0 0 0, 10 10 0)',2),
 ('SRID=4326;linestring(10 10 0, 10 0 0)',3),
 ('SRID=4326;linestring(8 0 0, 10 10 0)',4),
 ('SRID=4326;linestring(8 0 0, 8 10 0, 10 10 0)', 5),
 ('SRID=4326;linestring(7 12 0, 13 10 0, 14 8 0)', 6);

--for test z -2nd group
INSERT INTO test_table_l1 (geom, id) VALUES
 ('SRID=4326;linestring(15 14 50, 15 10 50, 15 8 50)', 7), --overpass over point(15 10 0)
 ('SRID=4326;linestring(15 16 35, 15 14 50)', 8), -- z connects with edge points
 ('SRID=4326;linestring(13 16 35, 15 14 50, 13 14 50)', 9); -- z connects with interior points


--2nd layer
DROP TABLE IF EXISTS test_table_l2;
CREATE  TABLE test_table_l2(
  id INTEGER PRIMARY  KEY,
  geom GEOMETRY('linestringz',4326)
);

INSERT INTO test_table_l2 (geom, id) VALUES
 ('SRID=4326;linestring(13 18 35, 13 16 35, 7 12 0)', 1),
 ('SRID=4326;linestring(15 18 0, 15 16 35, 17 18 0)', 2);

DROP TABLE if EXISTS test_table_p1;
CREATE  TABLE test_table_p1(
   id INTEGER PRIMARY  KEY,
   geom GEOMETRY('pointz',4326)
);

INSERT INTO test_table_p1 (geom, id) VALUES
 ('SRID=4326;point(10 10 0)',1),
 ('SRID=4326;point(10 0 0)',2),
 ('SRID=4326;point(8 10 0)',3),
 ('SRID=4326;point(8 0 0)',4),
 ('SRID=4326;point(5 0 0)',5),
 ('SRID=4326;point(0 0 0)',6),
 ('SRID=4326;point(7 12 0)',7);

--for test z
INSERT INTO test_table_p1 (geom, id) VALUES
 ('SRID=4326;point(15 10 0)',8),
 ('SRID=4326;point(13 14 50)',9), --edge point  of layer 2
 ('SRID=4326;point(15 16 35)',10), --interior point of layer 2
 ('SRID=4326;point(15 8 50)',11);

--for test connectivity with 2nd layer
INSERT INTO test_table_p1 (geom, id) VALUES
 ('SRID=4326;point(14 8 0)',12),
 ('SRID=4326;point(13 18 35)',13),
 ('SRID=4326;point(15 18 0)',14);


PREPARE createTopology_1 AS
  SELECT count(*) FROM  pgr_create_multimodal_graph('{
  "1": [
    "linealLayer-1"
  ],
  "2": [
    "linealLayer-2"
  ]
}','{"pointLayer-1":["linealLayer-1","linealLayer-2"]}'
                         , '{
  "linealLayer-1": {
    "sql": "SELECT id AS id, geom AS the_geom,0 AS z_start, 0 AS z_end FROM \"test_table_l1\"",
    "pconn": 0,
    "zconn": 0
  },"linealLayer-2": {
    "sql": "SELECT id AS id, geom AS the_geom,0 AS z_start, 0 AS z_end FROM \"test_table_l2\"",
    "pconn": 0,
    "zconn": 0
  },
  "pointLayer-1":{
    "sql":"SELECT id AS id, geom AS the_geom,0 AS z FROM \"test_table_p1\"",
    "pconn":0,
    "zconn":0
   }
}', 'graph_lines', 'public', 0.000001);

SELECT results_eq('createTopology_1', ARRAY[14]::BIGINT[]);

--testing connectivity

--test simple conn
PREPARE test1 AS
  SELECT count(*) FROM pgr_dijkstra(
                           'SELECT id, source, target, 0 AS cost, 0 AS reverse_cost FROM graph_lines',
                           (SELECT id FROM graph_lines_pt WHERE id_geom =6 ),
                           (SELECT id FROM graph_lines_pt WHERE id_geom =4 )
                         );
SELECT results_eq('test1', ARRAY[3]::BIGINT[]);

--test simple conn
PREPARE test2 AS
  SELECT count(*) FROM pgr_dijkstra(
                           'SELECT id, source, target, 0 AS cost, 0 AS reverse_cost FROM graph_lines',
                           (SELECT id FROM graph_lines_pt WHERE id_geom =6 ),
                           (SELECT id FROM graph_lines_pt WHERE id_geom =2 )
                         );
SELECT results_eq('test2', ARRAY[3]::BIGINT[]);

--test connectivity through inner point 1 on line 1
PREPARE test3 AS
  SELECT count(*) FROM pgr_dijkstra(
                           'SELECT id, source, target, 0 AS cost, 0 AS reverse_cost FROM graph_lines',
                           (SELECT id FROM graph_lines_pt WHERE id_geom =6 ),
                           (SELECT id FROM graph_lines_pt WHERE id_geom =5 )
                         );
SELECT results_eq('test3', ARRAY[0]::BIGINT[]);

--test point 3 was included, and connectivity to it ------------------------
--test point 3 was included
PREPARE test4 AS
  SELECT count(*) FROM graph_lines_pt WHERE id_geom = 3;
SELECT results_eq('test4',ARRAY[0]::BIGINT[]); --not included AS lineConn is 0 and point conn is 0

--test Testing connection policy and connectivity.
-- PREPARE test5 AS
-- SELECT count(*) FROM pgr_dijkstra(
--   'SELECT id, source, target, 0 AS cost, 0 AS reverse_cost FROM graph_lines',
--   (SELECT id FROM graph_lines_pt WHERE id_geom =2 ),
--   (SELECT id FROM graph_lines_pt WHERE id_geom =3 )
-- );
-- SELECT throws_ok('test5');  --there is no connection because point 3 is not included

--Testing connectivity through lines inner vertices
--test connectivity between 2 lines that crosses
PREPARE test6 AS
  SELECT count(*) FROM pgr_dijkstra(
                           'SELECT id, source, target, 0 AS cost, 0 AS reverse_cost FROM graph_lines',
                           (SELECT id FROM graph_lines_pt WHERE id_geom =7 and group_id = 0),
                           (SELECT id FROM graph_lines_pt WHERE id_geom =5 and group_id = 0)
                         );
SELECT results_eq('test6', ARRAY[0]::BIGINT[]); --there is no connection because of line conn policy

PREPARE test7 AS
  SELECT count(*) FROM pgr_dijkstra(
                           'SELECT id, source, target, 0 AS cost, 0 AS reverse_cost FROM graph_lines',
                           (SELECT id FROM graph_lines_pt WHERE id_geom =7 and group_id = 0),
                           (SELECT id FROM graph_lines_pt WHERE id_geom =6 and group_id = 0)
                         );
SELECT results_eq('test7', ARRAY[0]::BIGINT[]); -- there is no connection because of line conn policy

PREPARE test8 AS
  SELECT count(*) FROM pgr_dijkstra(
                           'SELECT id, source, target, 0 AS cost, 0 AS reverse_cost FROM graph_lines',
                           (SELECT id FROM graph_lines_pt WHERE id_geom =11 and group_id = 0),
                           (SELECT id FROM graph_lines_pt WHERE id_geom =1  and group_id = 0)
                         );
SELECT results_eq('test8', ARRAY[0]::BIGINT[]); -- there is no connection because of z in point 11 and point 8

PREPARE test9 AS
  SELECT count(*) FROM pgr_dijkstra(
                           'SELECT id, source, target, 0 AS cost, 0 AS reverse_cost FROM graph_lines',
                           (SELECT id FROM graph_lines_pt WHERE id_geom =11 and group_id = 0),
                           (SELECT id FROM graph_lines_pt WHERE id_geom =9  and group_id = 0)
                         );
SELECT results_eq('test9', ARRAY[0]::BIGINT[]); -- there is no connection because inner points not connecting policy

PREPARE test10 AS
  SELECT count(*) FROM pgr_dijkstra(
                           'SELECT id, source, target, 0 AS cost, 0 AS reverse_cost FROM graph_lines',
                           (SELECT id FROM graph_lines_pt WHERE id_geom =10 and group_id = 0),
                           (SELECT id FROM graph_lines_pt WHERE id_geom =9  and group_id = 0)
                         );
SELECT results_eq('test10', ARRAY[0]::BIGINT[]); -- there is no connection because inner points not connecting policy

PREPARE test11 AS
  SELECT count(*) FROM pgr_dijkstra(
                           'SELECT id, source, target, 0 AS cost, 0 AS reverse_cost FROM graph_lines',
                           (SELECT id FROM graph_lines_pt WHERE id_geom =13 and group_id = 1 ),
                           (SELECT id FROM graph_lines_pt WHERE id_geom =12 and group_id = 0 )
                         );
SELECT results_eq('test11', ARRAY[4]::BIGINT[]); --there is always be connection, unless point-layer dont join the two layers

PREPARE test12 AS
  SELECT count(*) FROM pgr_dijkstra(
                           'SELECT id, source, target, 0 AS cost, 0 AS reverse_cost FROM graph_lines',
                           (SELECT id FROM graph_lines_pt WHERE id_geom =13 and group_id = 1 ),
                           (SELECT id FROM graph_lines_pt WHERE id_geom =9 and group_id = 0 )
                         );
SELECT results_eq('test12', ARRAY[0]::BIGINT[]); --there will not be connection because 2nd group is not connected to first group and
                         --2nd layer not connect on point(13 16 35) because there is not such point in pontLayer-1

PREPARE test13 AS
  SELECT count(*) FROM pgr_dijkstra(
                           'SELECT id, source, target, 0 AS cost, 0 AS reverse_cost FROM graph_lines',
                           (SELECT id FROM graph_lines_pt WHERE id_geom =14 and group_id = 1 ),
                           (SELECT id FROM graph_lines_pt WHERE id_geom =9 and group_id = 0 )
                         );
SELECT results_eq('test13', ARRAY[0]::BIGINT[]); -- there is not connection because of layer and point connectivity policy, must be layer-1 point-1 to have connectivity


SELECT * FROM finish();
ROLLBACK;




