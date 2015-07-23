-- create a new table and merge attribute from original and noded
-- into table noded3

set client_min_messages to warning;
drop table if exists noded3 cascade;
create table noded3 (
  gid serial not null primary key,
  id int,
  dir text,
  source int,
  target int,
  cost float8,
  rcost float8
 );
select regexp_replace(addgeometrycolumn('noded3', 'the_geom', ST_SRID('POINT(0 0)'::geometry) , 'MULTILINESTRING', 2), 'SRID:-1', 'SRID:0');
insert into noded3 (id, dir, the_geom)
 select a.id, b.dir, st_multi(a.the_geom) as the_geom
   from unnoded_noded a, unnoded b
  where a.old_id=b.id;

set client_min_messages to notice;

\echo '---------------------------------'

select pgr_createtopology('noded3', 0.000001, id:= 'gid');

\echo '---------------------------------'

select * from noded3_vertices_pgr order by id;
