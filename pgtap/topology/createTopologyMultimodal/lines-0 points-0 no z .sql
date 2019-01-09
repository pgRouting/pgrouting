\i setup.sql

-- Cant be Warning because postgis is printing into warning channel.
set client_min_messages TO error;
select plan(1);

drop TABLE IF EXISTS test_table_l1;
create table test_table_l1(
   geom geometry('linestring',4326),
   id integer primary key
);

insert into test_table_l1 VALUES ('SRID=4326;linestring(5 0,10 10, 15 10)',1);
insert into test_table_l1 VALUES ('SRID=4326;linestring(0 0, 10 10)',2);
insert into test_table_l1 VALUES ('SRID=4326;linestring(10 10,10 0)',3);
insert into test_table_l1 VALUES ('SRID=4326;linestring(8 0, 10 10)',4);
insert into test_table_l1 VALUES ('SRID=4326;linestring(8 0, 8 10, 10 10)', 5);

drop table if EXISTS test_table_p1;
create TABLE test_table_p1(
   geom geometry('point',4326),
   id serial
  );

insert into test_table_p1 values('SRID=4326;point(10 10)',1);
insert into test_table_p1 values('SRID=4326;point(10 0)',2);
insert into test_table_p1 values('SRID=4326;point(8 10)',2);

prepare createTopology_1 as
SELECT count(*) from pgr_createtopology_layers('{
  "1": [
    "manoL"
  ]
}','{"manoP":["manoL"]}'
         , '{
  "manoL": {
    "sql": "select id as id, geom as the_geom,0 as z_start, 0 as z_end from \"test_table_l1\"",
    "pconn": 0,
    "zconn": 2
  },
  "manoP":{
    "sql":"select id as id, geom as the_geom,0 as z_start, 0 as z_end from \"test_table_p1\"",
    "pconn":0,
    "zconn":2
   }
}', 'graph_lines', 'public', 0.000001);

select results_eq('createTopology_1', array[1]::bigint[]); --point( 8 10) not intersect with any line point because of connection policy

--testing connectivity




