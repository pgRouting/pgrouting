\i setup.sql

set client_min_messages TO warning;

select plan(13);
drop TABLE IF EXISTS test_table_l1;
create table test_table_l1(
   geom geometry('linestringz',4326),
   id integer primary key
);
--1rst group
insert into test_table_l1 VALUES ('SRID=4326;linestring(5 0 0,10 10 0, 13 10 0, 15 10 0)',1);
insert into test_table_l1 VALUES ('SRID=4326;linestring(0 0 0, 10 10 0)',2);
insert into test_table_l1 VALUES ('SRID=4326;linestring(10 10 0, 10 0 0)',3);
insert into test_table_l1 VALUES ('SRID=4326;linestring(8 0 0, 10 10 0)',4);
insert into test_table_l1 VALUES ('SRID=4326;linestring(8 0 0, 8 10 0, 10 10 0)', 5);
insert into test_table_l1 values ('SRID=4326;linestring(7 12 0, 13 10 0, 14 8 0)', 6);

--for test z -2nd group
insert into test_table_l1 values ('SRID=4326;linestring(15 14 50, 15 10 50, 15 8 50)', 7); --overpass over point(15 10 0)
insert into test_table_l1 values ('SRID=4326;linestring(15 16 35, 15 14 50)', 8); -- z connects with edge points
insert into test_table_l1 values ('SRID=4326;linestring(13 16 35, 15 14 50, 13 14 50)', 9); -- z connects with interior points


--2nd layer
drop TABLE IF EXISTS test_table_l2;
create table test_table_l2(
  geom geometry('linestringz',4326),
  id integer primary key
);

insert into test_table_l2 values ('SRID=4326;linestring(13 18 35, 13 16 35, 7 12 0)', 1);
insert into test_table_l2 values ('SRID=4326;linestring(15 18 0, 15 16 35, 17 18 0)', 2);

drop table if EXISTS test_table_p1;
create TABLE test_table_p1(
   geom geometry('pointz',4326),
   id serial
);

insert into test_table_p1 values('SRID=4326;point(10 10 0)',1);
insert into test_table_p1 values('SRID=4326;point(10 0 0)',2);
insert into test_table_p1 values('SRID=4326;point(8 10 0)',3);
insert into test_table_p1 values('SRID=4326;point(8 0 0)',4);
insert into test_table_p1 values('SRID=4326;point(5 0 0)',5);
insert into test_table_p1 values('SRID=4326;point(0 0 0)',6);
insert into test_table_p1 values('SRID=4326;point(7 12 0)',7);

--for test z
insert into test_table_p1 values('SRID=4326;point(15 10 0)',8);
insert into test_table_p1 values('SRID=4326;point(13 14 50)',9); --edge point  of layer 2
insert into test_table_p1 values('SRID=4326;point(15 16 35)',10); --interior point of layer 2
insert into test_table_p1 values('SRID=4326;point(15 8 50)',11);

--for test connectivity with 2nd layer
insert into test_table_p1 values('SRID=4326;point(14 8 0)',12);
insert into test_table_p1 values('SRID=4326;point(13 18 35)',13);
insert into test_table_p1 values('SRID=4326;point(15 18 0)',14);


prepare createTopology_1 as
  SELECT count(*) from  pgr_createtopology_layers('{
  "1": [
    "linealLayer-1"
  ],
  "2": [
    "linealLayer-2"
  ]
}','{"pointLayer-1":["linealLayer-1","linealLayer-2"]}'
                         , '{
  "linealLayer-1": {
    "sql": "select id as id, geom as the_geom,0 as z_start, 0 as z_end from \"test_table_l1\"",
    "pconn": 0,
    "zconn": 0
  },"linealLayer-2": {
    "sql": "select id as id, geom as the_geom,0 as z_start, 0 as z_end from \"test_table_l2\"",
    "pconn": 0,
    "zconn": 0
  },
  "pointLayer-1":{
    "sql":"select id as id, geom as the_geom,0 as z from \"test_table_p1\"",
    "pconn":0,
    "zconn":0
   }
}', 'graph_lines', 'public', 0.000001);

select results_eq('createTopology_1', array[1]::bigint[]); --point( 8 10 0) not intersect with any line point because of connection policy

--testing connectivity

--test simple conn
prepare test1 as
  select count(*) from pgr_dijkstra(
                           'select id, source, target, 0 as cost, 0 as reverse_cost from graph_lines',
                           (select id from graph_lines_pt where id_geom =6 ),
                           (select id from graph_lines_pt where id_geom =4 )
                         );
select results_eq('test1', array[3]::bigint[]);

--test simple conn
prepare test2 as
  select count(*) from pgr_dijkstra(
                           'select id, source, target, 0 as cost, 0 as reverse_cost from graph_lines',
                           (select id from graph_lines_pt where id_geom =6 ),
                           (select id from graph_lines_pt where id_geom =2 )
                         );
select results_eq('test2', array[3]::bigint[]);

--test connectivity through inner point 1 on line 1
prepare test3 as
  select count(*) from pgr_dijkstra(
                           'select id, source, target, 0 as cost, 0 as reverse_cost from graph_lines',
                           (select id from graph_lines_pt where id_geom =6 ),
                           (select id from graph_lines_pt where id_geom =5 )
                         );
select results_eq('test3', array[0]::bigint[]);

--test point 3 was included, and connectivity to it ------------------------
--test point 3 was included
prepare test4 as
  select count(*) from graph_lines_pt where id_geom = 3;
select results_eq('test4',array[0]::bigint[]); --not included as lineConn is 0 and point conn is 0

--test Testing connection policy and connectivity.
-- prepare test5 as
-- select count(*) from pgr_dijkstra(
--   'select id, source, target, 0 as cost, 0 as reverse_cost from graph_lines',
--   (select id from graph_lines_pt where id_geom =2 ),
--   (select id from graph_lines_pt where id_geom =3 )
-- );
-- select throws_ok('test5');  --there is no connection because point 3 is not included

--Testing connectivity through lines inner vertices
--test connectivity between 2 lines that crosses
prepare test6 as
  select count(*) from pgr_dijkstra(
                           'select id, source, target, 0 as cost, 0 as reverse_cost from graph_lines',
                           (select id from graph_lines_pt where id_geom =7 ),
                           (select id from graph_lines_pt where id_geom =5 )
                         );
select results_eq('test6', array[0]::bigint[]); --there is no connection because of line conn policy

prepare test7 as
  select count(*) from pgr_dijkstra(
                           'select id, source, target, 0 as cost, 0 as reverse_cost from graph_lines',
                           (select id from graph_lines_pt where id_geom =7 ),
                           (select id from graph_lines_pt where id_geom =6 )
                         );
select results_eq('test7', array[0]::bigint[]); -- there is no connection because of line conn policy

prepare test8 as
  select count(*) from pgr_dijkstra(
                           'select id, source, target, 0 as cost, 0 as reverse_cost from graph_lines',
                           (select id from graph_lines_pt where id_geom =11 ),
                           (select id from graph_lines_pt where id_geom =1 )
                         );
select results_eq('test8', array[0]::bigint[]); -- there is no connection because of z in point 11 and point 8

prepare test9 as
  select count(*) from pgr_dijkstra(
                           'select id, source, target, 0 as cost, 0 as reverse_cost from graph_lines',
                           (select id from graph_lines_pt where id_geom =11 ),
                           (select id from graph_lines_pt where id_geom =9 )
                         );
select results_eq('test9', array[0]::bigint[]); -- there is no connection because inner points not connecting policy

prepare test10 as
  select count(*) from pgr_dijkstra(
                           'select id, source, target, 0 as cost, 0 as reverse_cost from graph_lines',
                           (select id from graph_lines_pt where id_geom =10 ),
                           (select id from graph_lines_pt where id_geom =9 )
                         );
select results_eq('test10', array[0]::bigint[]); -- there is no connection because inner points not connecting policy

prepare test11 as
  select count(*) from pgr_dijkstra(
                           'select id, source, target, 0 as cost, 0 as reverse_cost from graph_lines',
                           (select id from graph_lines_pt where id_geom =13 ),
                           (select id from graph_lines_pt where id_geom =12 )
                         );
select results_eq('test11', array[3]::bigint[]); --there is always be connection, unless point-layer dont join the two layers

prepare test12 as
  select count(*) from pgr_dijkstra(
                           'select id, source, target, 0 as cost, 0 as reverse_cost from graph_lines',
                           (select id from graph_lines_pt where id_geom =13 ),
                           (select id from graph_lines_pt where id_geom =9 )
                         );
select results_eq('test12', array[0]::bigint[]); --there will not be connection because 2nd group is not connected to first group and
                         --2nd layer not connect on point(13 16 35) because there is not such point in pontLayer-1

prepare test13 as
  select count(*) from pgr_dijkstra(
                           'select id, source, target, 0 as cost, 0 as reverse_cost from graph_lines',
                           (select id from graph_lines_pt where id_geom =14 ),
                           (select id from graph_lines_pt where id_geom =9 )
                         );
select results_eq('test13', array[0]::bigint[]); -- there is not connection because of layer and point connectivity policy, must be layer-1 point-1 to have connectivity


SELECT * FROM finish();
ROLLBACK;




