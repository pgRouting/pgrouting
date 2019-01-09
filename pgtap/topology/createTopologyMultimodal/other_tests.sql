select * from pgr_create_top_graph_ptos;


------------------z index-------------------------------
drop TABLE IF EXISTS test_table_l1;
create table test_table_l1(
                            geom geometry('linestringz',4326),
                            id SERIAL
);
insert into test_table_l1 VALUES ('SRID=4326;linestring(5 0 0,10 10 10, 15 10 0)');
insert into test_table_l1 VALUES ('SRID=4326;linestring(0 0 0, 10 10 0)');
insert into test_table_l1 VALUES ('SRID=4326;linestring(10 10 10,10 10 0)');
insert into test_table_l1 VALUES ('SRID=4326;linestring(8 0 0, 10 10 10)');

drop table if EXISTS test_table_p1;
create TABLE test_table_p1(
                            geom geometry('pointz',4326),
                            id serial
);
insert into test_table_p1 values('SRID=4326;point(10 10 10)');
insert into test_table_p1 values('SRID=4326;point(10 10 0)');
insert into test_table_p1 values('SRID=4326;point(10 0 0)');

SELECT pgr_createtopology_layers('{
  "1": [
    "manoL"
  ]
}','{"manoP":["manoL"]}'
         , '{
  "manoL": {
    "sql": "select id as id, geom as the_geom,0 as z_start, 0 as z_end from \"test_table_l1\"",
    "pconn": 0,
    "zconn": 0
  },
  "manoP":{
    "sql":"select id as id, geom as the_geom,0 as z_start, 0 as z_end from \"test_table_p1\"",
    "pconn":1,
    "zconn":0
   }
}', 'graph_lines_222', 'mod_graphmanager', 0.000001);


------------------------Using z from column--------------------------------------------
drop TABLE IF EXISTS test_table_l1;
create table test_table_l1(
                            geom geometry('linestring',4326),
                            z_start FLOAT,
                            z_end FLOAT,
                            id SERIAL
);
insert into test_table_l1 VALUES ('SRID=4326;linestring(5 0,10 10, 15 10)',0,0);
insert into test_table_l1 VALUES ('SRID=4326;linestring(0 0,10 10, 20 10)', 0, 10);
insert into test_table_l1 VALUES ('SRID=4326;linestring(10 10,10 0)',10, 0);
insert into test_table_l1 VALUES ('SRID=4326;linestring(8 0, 10 10)',0, 10);

drop table if EXISTS test_table_p1;
create TABLE test_table_p1(
                            geom geometry('point',4326),
                            z FLOAT,
                            id serial
);
insert into test_table_p1 values('SRID=4326;point(10 10)',10);
insert into test_table_p1 values('SRID=4326;point(10 0)',0);

SELECT pgr_createtopology_layers('{
  "1": [
    "manoL"
  ]
}','{"manoP":["manoL"]}'
         , '{
  "manoL": {
    "sql": "select id , geom as the_geom,z_start, z_end from \"test_table_l1\"",
    "pconn": 1,
    "zconn": 1
  },
  "manoP":{
    "sql":"select id , geom as the_geom,z  from \"test_table_p1\"",
    "pconn":0,
    "zconn":1
   }
}', 'graph_lines_222', 'mod_graphmanager', 0.000001);


------------------------Many Groups---------------------------------------------------
drop TABLE IF EXISTS test_table_l1;
create table test_table_l1(
                            geom geometry('linestring',4326),
                            id SERIAL
);
insert into test_table_l1 VALUES ('SRID=4326;linestring(5 0,10 0, 15 0)'),
                                 ('SRID=4326;linestring(5 1,10 1, 15 1)');

drop table if EXISTS test_table_p1;
create TABLE test_table_p1(
                            geom geometry('point',4326),
                            id serial
);
insert into test_table_p1 values('SRID=4326;point(10 0)');

drop table if EXISTS test_table_l2;
create TABLE test_table_l2(
                            geom geometry('linestring',4326),
                            id serial
);

INSERT into test_table_l2 values ('SRID=4326;linestring(10 -1, 10 0, 10 1, 10 2)');


SELECT pgr_createtopology_layers('{
  "1": [
    "manoL"
  ],"2":["manoL2"]
}','{"manoP":["manoL","manoL2"]}'
         , '{
  "manoL": {
    "sql": "select id , geom as the_geom from \"test_table_l1\"",
    "pconn": 1,
    "zconn": 2
  },
  "manoL2": {
    "sql": "select id , geom as the_geom from \"test_table_l2\"",
    "pconn": 1,
    "zconn": 2
  },
  "manoP":{
    "sql":"select id , geom as the_geom from \"test_table_p1\"",
    "pconn":1,
    "zconn":2
  }
}', 'graph_lines_222', 'mod_graphmanager', 0.00000001);

------------------------LinearLayer3d and puntual layer 2d-------------------------------
drop TABLE IF EXISTS test_table_l1;
create table test_table_l1(
                            geom geometry('linestringz',4326),
                            id SERIAL
);
insert into test_table_l1 VALUES ('SRID=4326;linestring(5 0 0,10 10 0, 15 10 0)');

drop TABLE IF EXISTS test_table_l2;
create table test_table_l2(
                            geom geometry('linestringz',4326),
                            id SERIAL
);
insert into test_table_l2 VALUES ('SRID=4326;linestring(5 0 1,10 10 1, 15 10 1)');

drop table if EXISTS test_table_p1;
create TABLE test_table_p1(
                            geom geometry('point',4326),
                            id serial
);
insert into test_table_p1 values('SRID=4326;point(10 10)');

SELECT pgr_createtopology_layers('{
  "1": [
    "manoL"
  ],"2":["manoL2"]
}','{"manoP":["manoL","manoL2"]}'
         , '{
  "manoL": {
    "sql": "select id , geom as the_geom from \"test_table_l1\"",
    "pconn": 0,
    "zconn": 0
  },
  "manoL2": {
    "sql": "select id , geom as the_geom from \"test_table_l2\"",
    "pconn": 0,
    "zconn": 0
  },
  "manoP":{
    "sql":"select id , geom as the_geom from \"test_table_p1\"",
    "pconn":1,
    "zconn":2
  }
}', 'graph_lines_222', 'mod_graphmanager', 0.00000001);

----------------------Testing multilines------------------------------------------------
drop TABLE IF EXISTS test_table_l1;
create table test_table_l1(
                            geom geometry('multilinestring',4326),
                            id SERIAL
);
insert into test_table_l1 VALUES ('SRID=4326;multilinestring((5 0,10 0),(10 0, 15 0))'),
                                 ('SRID=4326;multilinestring((5 2,10 0),(10 0, 15 2))');

drop table if EXISTS test_table_p1;
create TABLE test_table_p1(
                            geom geometry('point',4326),
                            id serial
);
insert into test_table_p1 values('SRID=4326;point(10 0)');

SELECT pgr_createtopology_layers('{
  "1": [
    "manoL"
  ]
}','{"manoP":["manoL"]}'
         , '{
  "manoL": {
    "sql": "select id , geom as the_geom from \"test_table_l1\"",
    "pconn": 1,
    "zconn": 2
  },
  "manoP":{
    "sql":"select id , geom as the_geom from \"test_table_p1\"",
    "pconn":1,
    "zconn":2
  }
}', 'graph_lines_222', 'mod_graphmanager', 0.00000001);


-----------------testin some error---------------------------------
drop TABLE IF EXISTS test_table_l1;
create table test_table_l1(
                            geom geometry('linestring',4326),
                            id SERIAL
);
insert into test_table_l1 VALUES ('SRID=4326;linestring(1 1, 2 2, 1 3)'),
                                 ('SRID=4326;linestring(2 1, 2 2, 3 4)'),
                                 ('SRID=4326;linestring(3 1, 2 2)'),
                                 ('SRID=4326;linestring(4 1, 2 2)');
SELECT pgr_createtopology_layers('{
  "1": [
    "manoL"
  ]
}',null
         , '{
  "manoL":{
    "sql":"select id , geom as the_geom from \"test_table_l1\"",
    "pconn":1,
    "zconn":2
  }
}', 'graph_lines_222', 'mod_graphmanager', 0.000001)