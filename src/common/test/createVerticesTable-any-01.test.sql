BEGIN;
set client_min_messages to warning;
SET search_path TO public;

drop table if exists "Edges2";
drop table if exists "Edges2_vertices_pgr";
drop schema if exists "myschema" cascade;

set client_min_messages to notice;

-- The following should be OK

select pgr_createTopology('edges2',0.00001, id:='eid');
drop table if exists edges2_vertex_pgr;
select pgr_createVerticesTable('edges2');
select pgr_createVerticesTable('Edges2', 'the_geom');
select pgr_createVerticesTable('edges2', 'The_Geom');
select pgr_createVerticesTable('edges2', 'the_geom');
select pgr_createVerticesTable('edges2', 'The_geom');
select pgr_createVerticesTable('Edges2', 'The_geom');
select pgr_createVerticesTable('edges2', 'the_geom','source');
select pgr_createVerticesTable('edges2', 'the_geom','Source');
select pgr_createVerticesTable('edges2', 'the_geom','source','target');
select pgr_createVerticesTable('edges2', 'the_geom',rows_where:='eid<15');
select pgr_createVerticesTable('edges2', 'the_geom',source:='source');
select pgr_createVerticesTable('edges2', 'the_geom',target:='target',rows_where:='eid<15 and eid>5');

-- the following tests should FAIL
set client_min_messages to warning;

select pgr_createVerticesTable('edges22', 'the_geom');
select pgr_createVerticesTable('edges2', 'geom');
select pgr_createVerticesTable('edges2', 'the_geom', 'gid');
select pgr_createVerticesTable('edges2', 'the_geom','sourc');
select pgr_createVerticesTable('edges2', 'the_geom',target:='source');
select pgr_createVerticesTable('edges2', source:='the_geom');
select pgr_createVerticesTable('edges2', target:='the_geom');

-- Testing with table with a similar name

select * into "Edges2" from edges2;

-- The following should be OK                                                                                                                 
set client_min_messages to notice;
                                                                                                                                              
select pgr_createVerticesTable('Edges2');                                                                                     
update "Edges2" set the_geom=NULL where eid=5;                                                                                                
select pgr_createVerticesTable('Edges2');                                                                                     
update "Edges2" set source=NULL where eid=7;                                                                                                     
select pgr_createVerticesTable('Edges2');                                                                                     
                                                                                                                                              
create schema myschema;                                                                                                                       
SET search_path TO myschema,public;                                                                                                           
                                                                                                                                              
-- reaching tables when located in another schema                                                                                             
-- the following should be OK                                                                                                                 
                                                                                                                                              
select pgr_createVerticesTable('public.edges2' );                                                                              
select pgr_createVerticesTable('public.Edges2', 'the_geom');                                                                      
select pgr_createVerticesTable('Public.edges2', 'The_Geom');
select pgr_createVerticesTable('public.edges2', 'The_geom'); 
select pgr_createVerticesTable('PUBLIC.Edges2', 'The_geom');
select pgr_createVerticesTable('public.edges2', 'the_geom','source');
select pgr_createVerticesTable('public.edges2', 'the_geom','Source');
select pgr_createVerticesTable('public.edges2', 'the_geom','source','target');
select pgr_createVerticesTable('public.edges2', 'the_geom',rows_where:='eid<15');
select pgr_createVerticesTable('public.edges2', 'the_geom',source:='source');
select pgr_createVerticesTable('public.edges2', 'the_geom',target:='target',rows_where:='eid<15');

-- the following tests should fail
set client_min_messages to warning;

select pgr_createVerticesTable('public.edges22', 'the_geom');
select pgr_createVerticesTable('public.edges2', 'geom');
select pgr_createVerticesTable('public.edges2', 'the_geom', 'gid');
select pgr_createVerticesTable('public.Edges2', 'the_geom','sourc');
select pgr_createVerticesTable('public.edges2', 'the_geom',target:='source');
select pgr_createVerticesTable('public.edges2',source:='the_geom');
select pgr_createVerticesTable('public.edges2', target:='the_geom');

-- creating tables with similar names in myschema

select * into edges2 from public.edges2;
select * into "Edges2" from public."Edges2";

-- The following should be OK 
set client_min_messages to notice;

select pgr_createVerticesTable('edges2');
select pgr_createVerticesTable('Edges2', 'the_geom');
select pgr_createVerticesTable('edges2', 'The_Geom');
select pgr_createVerticesTable('edges2', 'The_geom');
select pgr_createVerticesTable('Edges2', 'The_geom');
select pgr_createVerticesTable('edges2', 'the_geom','source');
select pgr_createVerticesTable('edges2', 'the_geom','Source');
select pgr_createVerticesTable('edges2', 'the_geom','source','target');
select pgr_createVerticesTable('edges2', 'the_geom',rows_where:='eid<15');
select pgr_createVerticesTable('edges2', 'the_geom',source:='source');
select pgr_createVerticesTable('edges2', 'the_geom',target:='target',rows_where:='eid<15');

-- the following tests should FAIL
set client_min_messages to warning;

select pgr_createVerticesTable('edges22', 'the_geom');
select pgr_createVerticesTable('edges2', 'geom');
select pgr_createVerticesTable('edges2', 'the_geom', 'gid');
select pgr_createVerticesTable('edges2', 'the_geom','sourc');
select pgr_createVerticesTable('edges2', 'the_geom',target:='source');
select pgr_createVerticesTable('edges2', source:='the_geom');
select pgr_createVerticesTable('edges2', target:='the_geom');

--Test of the rows_where  clause


-- The following should be OK
set client_min_messages to notice;
select pgr_createVerticesTable('edges2', rows_where:='eid<15');
select pgr_createVerticesTable('Edges2', rows_where:='eid<15 and eid>5');
select pgr_createVerticesTable('Edges2', rows_where:=' the_geom && (select st_buffer(the_geom,0.0001) as buffer from "Edges2" 
where eid=6)') ;

-- The following should FAIL (working on myschema)
set client_min_messages to warning;

select pgr_createVerticesTable('edges2',rows_where:='eid<');
select pgr_createVerticesTable('Edges2', rows_where:='eid<15 and ed>5');
select pgr_createVerticesTable('Edges2', rows_where:=' the_geom && (select st_buffer(the_geom,0.0001 as buffer from "Edges2" where eid=6)') ;

/* BIG TABLE TEST TAKES TIME
create table bigtable ( id bigserial PRIMARY KEY, source integer,target integer);
SELECT AddGeometryColumn( 'bigtable', 'the_geom', 0, 'LINESTRING',2);
insert into bigtable (the_geom) ( select ST_MakeLine(ST_MakePoint(random()*1000,random()*1000), ST_MakePoint(random()*1000,random()*1000)) 
from  (SELECT * FROM generate_series(1,3000) AS id) AS x) ;
select pgr_createVerticesTable('bigtable');
select pgr_createTopology('bigtable',0.0001);
select * into big_table_vertices from bigtable_vertices_pgr;
drop table if exists bigtable_vertices_pgr;
select pgr_createVerticesTable('myschema.bigtable');
select (select count(*) from myschema.big_table_vertices) = (select count(*) from myschema.bigtable_vertices_pgr);
select count(*) from ( (select * from myschema.big_table_vertices) union (select * from myschema.bigtable_vertices_pgr)) as a;
*/

set client_min_messages to warning;
SET search_path TO public;
drop table if exists "Edges2";
drop table if exists "Edges2_vertices_pgr";
drop schema if exists "myschema" cascade;
ROLLBACK;
