/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/
BEGIN;
-- node the network

select count(*) from unnoded limit 1;
select pgr_nodenetwork('unnoded', 0.000001);
select pgr_createtopology('unnoded_noded', 0.000001 );
select pgr_analyzegraph('unnoded_noded',  0.000001);

select * from unnoded_noded order by old_id, sub_id;
select * from unnoded_noded_vertices_pgr order by id;



-- create a new table and merge attribute from original and noded
-- into table noded2
set client_min_messages to warning;
drop table if exists noded2;
create table noded2 (
  gid serial not null primary key,
  id int,
  dir text,
  source int,
  target int,
  cost float8,
  rcost float8
 );



select regexp_replace(addgeometrycolumn('noded2', 'the_geom', ST_SRID('POINT(0 0)'::geometry) , 'LINESTRING', 2), 'SRID:-1', 'SRID:0');
insert into noded2 (id, dir, the_geom)
 select a.old_id, b.dir, a.the_geom
   from unnoded_noded a, unnoded b
  where a.old_id=b.id;

set client_min_messages to notice;


select pgr_createtopology('noded2', 0.000001,id:='gid');
select pgr_analyzegraph('noded2', 0.000001,id:='gid');
select pgr_analyzeoneway('noded2',
    ARRAY['', 'B', 'TF'],
    ARRAY['', 'B', 'FT'],
    ARRAY['', 'B', 'FT'],
    ARRAY['', 'B', 'TF'],
    oneway:='dir');


select * from noded2 order by id;
select * from noded2_vertices_pgr order by id;



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
ROLLBACK;
