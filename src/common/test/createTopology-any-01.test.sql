\set VERBOSITY 'terse'

set client_min_messages to warning;
SET search_path TO public;
drop table if exists "Edges2";
drop table if exists "Edges2_vertices_pgr";
drop schema if exists "myschema" cascade;


-- The following should be OK

select 1, pgr_createTopology('edges2', 0.000001, id:='eid') = 'OK';
select 2, pgr_createTopology('Edges2', 0.000001, 'the_geom', 'eid') = 'OK';
select 3, pgr_createTopology('edges2', 0.000001, 'The_Geom', 'eid') = 'OK';
select 4, pgr_createTopology('edges2', 0.000001, 'the_geom', 'Eid') = 'OK';
select 5, pgr_createTopology('edges2', 0.000001, 'The_geom', 'Eid') = 'OK';
select 6, pgr_createTopology('Edges2', 0.000001, 'The_geom', 'Eid') = 'OK';
select 7, pgr_createTopology('edges2', 0.000001, 'the_geom', 'eid','source') = 'OK';
select 8, pgr_createTopology('edges2', 0.000001, 'the_geom', 'eid','Source') = 'OK';
select 9, pgr_createTopology('edges2', 0.000001, 'the_geom', 'eid','source','target') = 'OK';
select 10, pgr_createTopology('edges2', 0.000001, 'the_geom', 'eid',rows_where:='eid<15') = 'OK';
select 11, pgr_createTopology('edges2', 0.000001, 'the_geom', 'eid',source:='source') = 'OK';
select 12, pgr_createTopology('edges2', 0.000001, 'the_geom', 'eid',target:='target',rows_where:='eid<15 and eid>5') = 'OK';

-- the following tests should FAIL

select 13, pgr_createTopology('edges22', 0.000001, 'the_geom', 'eid') = 'FAIL';
select 14, pgr_createTopology('edges2', 0.000001, 'geom', 'eid') = 'FAIL';
select 15, pgr_createTopology('edges2', 0.000001, 'the_geom', 'gid') = 'FAIL';
select 16, pgr_createTopology('edges2', 0.000001, 'the_geom', 'eid','sourc') = 'FAIL';
select 17, pgr_createTopology('edges2', 0.000001, 'the_geom', 'eid',target:='source') = 'FAIL';
select 18, pgr_createTopology('edges2', 0.000001, id:='eid',source:='the_geom') = 'FAIL';
select 19, pgr_createTopology('edges2', 0.000001, id:='eid',target:='the_geom') = 'FAIL';

-- Testing with table with a similar name

select * into "Edges2" from edges2;

-- The following should be OK

select 20, pgr_createTopology('Edges2', 0.000001,id:='eid') = 'OK';
update "Edges2" set the_geom=NULL where eid=5;
select 21, pgr_createTopology('Edges2', 0.000001,id:='eid') = 'OK';
update "Edges2" set eid=NULL where eid=7;
select 22, pgr_createTopology('Edges2', 0.000001,id:='eid') = 'OK';

create schema myschema;
SET search_path TO myschema,public;

-- reaching tables when located in another schema
-- the following should be OK

select 23, pgr_createTopology('public.edges2', 0.000001, id:='eid') = 'OK';
select 24, pgr_createTopology('public.Edges2', 0.000001, 'the_geom', 'eid') = 'OK';
select 25, pgr_createTopology('Public.edges2', 0.000001, 'The_Geom', 'eid') = 'OK';
select 26, pgr_createTopology('public.edges2', 0.000001, 'the_geom', 'Eid') = 'OK';
select 27, pgr_createTopology('public.edges2', 0.000001, 'The_geom', 'Eid') = 'OK';
select 28, pgr_createTopology('PUBLIC.Edges2', 0.000001, 'The_geom', 'Eid') = 'OK';
select 29, pgr_createTopology('public.edges2', 0.000001, 'the_geom', 'eid','source') = 'OK';
select 30, pgr_createTopology('public.edges2', 0.000001, 'the_geom', 'eid','Source') = 'OK';
select 30, pgr_createTopology('public.edges2', 0.000001, 'the_geom', 'eid','source','target') = 'OK';
select 31, pgr_createTopology('public.edges2', 0.000001, 'the_geom', 'eid',rows_where:='eid<15') = 'OK';
select 32, pgr_createTopology('public.edges2', 0.000001, 'the_geom', 'eid',source:='source') = 'OK';
select 33, pgr_createTopology('public.edges2', 0.000001, 'the_geom', 'eid',target:='target',rows_where:='eid<15') = 'OK';

-- the following tests should fail

select 34, pgr_createTopology('public.edges22', 0.000001, 'the_geom', 'eid') = 'FAIL';
select 35, pgr_createTopology('public.edges2', 0.000001, 'geom', 'eid') = 'FAIL';
select 36, pgr_createTopology('public.edges2', 0.000001, 'the_geom', 'gid') = 'FAIL';
select 37, pgr_createTopology('public.Edges2', 0.000001, 'the_geom', 'eid','sourc') = 'FAIL';
select 38, pgr_createTopology('public.edges2', 0.000001, 'the_geom', 'eid',target:='source') = 'FAIL';
select 39, pgr_createTopology('public.edges2', 0.000001, id:='eid',source:='the_geom') = 'FAIL';
select 40, pgr_createTopology('public.edges2', 0.000001, id:='eid',target:='the_geom') = 'FAIL';

-- creating tables with similar names in myschema

select * into edges2 from public.edges2;
select * into "Edges2" from public."Edges2";

-- The following should be OK

select 41, pgr_createTopology('edges2', 0.000001, id:='eid') = 'OK';
select 42, pgr_createTopology('Edges2', 0.000001, 'the_geom', 'eid') = 'OK';
select 43, pgr_createTopology('edges2', 0.000001, 'The_Geom', 'eid') = 'OK';
select 44, pgr_createTopology('edges2', 0.000001, 'the_geom', 'Eid') = 'OK';
select 45, pgr_createTopology('edges2', 0.000001, 'The_geom', 'Eid') = 'OK';
select 46, pgr_createTopology('Edges2', 0.000001, 'The_geom', 'Eid') = 'OK';
select 47, pgr_createTopology('edges2', 0.000001, 'the_geom', 'eid','source') = 'OK';
select 48, pgr_createTopology('edges2', 0.000001, 'the_geom', 'eid','Source') = 'OK';
select 49, pgr_createTopology('edges2', 0.000001, 'the_geom', 'eid','source','target') = 'OK';
select 50, pgr_createTopology('edges2', 0.000001, 'the_geom', 'eid',rows_where:='eid<15') = 'OK';
select 51, pgr_createTopology('edges2', 0.000001, 'the_geom', 'eid',source:='source') = 'OK';
select 52, pgr_createTopology('edges2', 0.000001, 'the_geom', 'eid',target:='target',rows_where:='eid<15') = 'OK';

-- the following tests should FAIL

select 53, pgr_createTopology('edges22', 0.000001, 'the_geom', 'eid') = 'FAIL';
select 54, pgr_createTopology('edges2', 0.000001, 'geom', 'eid') = 'FAIL';
select 55, pgr_createTopology('edges2', 0.000001, 'the_geom', 'gid') = 'FAIL';
select 56, pgr_createTopology('edges2', 0.000001, 'the_geom', 'eid','sourc') = 'FAIL';
select 57, pgr_createTopology('edges2', 0.000001, 'the_geom', 'eid',target:='source') = 'FAIL';
select 58, pgr_createTopology('edges2', 0.000001, id:='eid',source:='the_geom') = 'FAIL';
select 59, pgr_createTopology('edges2', 0.000001, id:='eid',target:='the_geom') = 'FAIL';

--Test of the rows_where clause


-- The following should be OK
select 60, pgr_createTopology('edges2', 0.000001, id:= 'eid',rows_where:='eid<15') = 'OK';
select 61, pgr_createTopology('Edges2', 0.000001, id:= 'eid',rows_where:='eid<15 and eid>5') = 'OK';
select 62, pgr_createTopology('Edges2', 0.000001, id:= 'eid',rows_where:=' the_geom && (select st_buffer(the_geom,0.0001) as buffer from "Edges2" where eid=6)') = 'OK';

-- The following should FAIL (working on myschema)

select 63, pgr_createTopology('edges2', 0.000001, id:= 'eid',rows_where:='eid<') = 'FAIL';
select 64, pgr_createTopology('Edges2', 0.000001, id:= 'eid',rows_where:='eid<15 and ed>5') = 'FAIL';
select 65, pgr_createTopology('Edges2', 0.000001, id:= 'eid',rows_where:=' the_geom && (select st_buffer(the_geom,0.0001 as buffer from "Edges2" where eid=6)')  = 'FAIL';

-- BIG TABLE TEST TAKES TIME
--create table bigtable ( id bigserial PRIMARY KEY, source smallint,target smallint);
--SELECT AddGeometryColumn( 'bigtable', 'the_geom', 0, 'LINESTRING',2);
--insert into bigtable (the_geom) ( select ST_MakeLine(ST_MakePoint(random()*1000,random()*1000), ST_MakePoint(random()*1000,random()*1000))
--from (SELECT * FROM generate_series(1,20000) AS id) AS x) ;
--select 66, pgr_createTopology('bigtable', 0.000001);


set client_min_messages to warning;

SET search_path TO public;
drop table if exists "Edges2";
drop table if exists "Edges2_vertices_pgr";
drop schema if exists "myschema" cascade;
