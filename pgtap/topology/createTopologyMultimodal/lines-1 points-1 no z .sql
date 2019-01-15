\i setup.sql

-- Cant be Warning because postgis is printing into warning channel.
set client_min_messages to warning;
select plan(8);

drop TABLE IF EXISTS test_table_l1;
create table test_table_l1(
   geom geometry('linestring',4326),
   id integer primary key
);

insert into test_table_l1 VALUES ('SRID=4326;linestring(5 0,10 10, 13 10, 15 10)',1);
insert into test_table_l1 VALUES ('SRID=4326;linestring(0 0, 10 10)',2);
insert into test_table_l1 VALUES ('SRID=4326;linestring(10 10,10 0)',3);
insert into test_table_l1 VALUES ('SRID=4326;linestring(8 0, 10 10)',4);
insert into test_table_l1 VALUES ('SRID=4326;linestring(8 0, 8 10, 10 10)', 5);
insert into test_table_l1 values ('SRID=4326;linestring(7 12, 13 10, 14 8)', 6);

drop table if EXISTS test_table_p1;
create TABLE test_table_p1(
   geom geometry('point',4326),
   id serial
  );

insert into test_table_p1 values('SRID=4326;point(10 10)',1);
insert into test_table_p1 values('SRID=4326;point(10 0)',2);
insert into test_table_p1 values('SRID=4326;point(8 10)',3);
insert into test_table_p1 values('SRID=4326;point(8 0)',4);
insert into test_table_p1 values('SRID=4326;point(5 0)',5);
insert into test_table_p1 values('SRID=4326;point(0 0)',6);
insert into test_table_p1 values('SRID=4326;point(7 12)',7);

prepare createTopology_1 as
SELECT count(*) from pgr_createtopology_layers('{
  "1": [
    "linealLayer-1"
  ]
}','{"pointLayer-1":["linealLayer-1"]}'
         , '{
  "linealLayer-1": {
    "sql": "select id as id, geom as the_geom,0 as z_start, 0 as z_end from \"test_table_l1\"",
    "pconn": 1,
    "zconn": 2
  },
  "pointLayer-1":{
    "sql":"select id as id, geom as the_geom,0 as z from \"test_table_p1\"",
    "pconn":1,
    "zconn":2
   }
}', 'graph_lines', 'public', 0.000001);

select results_eq('createTopology_1', array[0]::bigint[]); --point( 8 10) not intersect with any line point because of connection policy

--testing connectivity

--test simple conn
prepare test1 as
select count(*) from pgr_dijkstra(
  'select id, source, target, 0 as cost, 0 as reverse_cost from graph_l' ||
  'ines',
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
select results_eq('test3', array[3]::bigint[]);

--test point 3 was included, and connectivity to it ------------------------
--test point 3 was included
prepare test4 as
select count(*) from graph_lines_pt where id_geom = 3;
select results_eq('test4',array[1]::bigint[]);

--test Testing connection policy and connectivity.
prepare test5 as
select count(*) from pgr_dijkstra(
  'select id, source, target, 0 as cost, 0 as reverse_cost from graph_lines',
  (select id from graph_lines_pt where id_geom =2 ),
  (select id from graph_lines_pt where id_geom =3 )
);
select results_eq('test5',array[3]::bigint[]);

--Testing connectivity through lines inner vertices
--test connectivity between 2 lines that crosses
prepare test6 as
select count(*) from pgr_dijkstra(
  'select id, source, target, 0 as cost, 0 as reverse_cost from graph_lines',
  (select id from graph_lines_pt where id_geom =7 ),
  (select id from graph_lines_pt where id_geom =5 )
);
select results_eq('test6', array[4]::bigint[]);

prepare test7 as
select count(*) from pgr_dijkstra(
  'select id, source, target, 0 as cost, 0 as reverse_cost from graph_lines',
  (select id from graph_lines_pt where id_geom =7 ),
  (select id from graph_lines_pt where id_geom =6 )
);
select results_eq('test7', array[4]::bigint[]);

SELECT * FROM finish();
ROLLBACK;




