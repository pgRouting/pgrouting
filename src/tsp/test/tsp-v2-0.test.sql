
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

--------------------------------------------------------------------------------
--              PGR_pgr_tsp
--------------------------------------------------------------------------------

CREATE TEMP TABLE vertex_table (
    id serial,
    x double precision,
    y double precision
);

INSERT INTO vertex_table VALUES
(1,2,0), (2,2,1), (3,3,1), (4,4,1), (5,0,2), (6,1,2), (7,2,2),
(8,3,2), (9,4,2), (10,2,3), (11,3,3), (12,4,3), (13,2,4);

CREATE table tsp_00 (
    source_id serial not null primary key,
    x float,
    y float);

insert into tsp_00 (x,y) values
(1,7),
(1,4),
(3,8),
(1,2),
(3,5),
(5,8),
(1,1),
(2,1),
(3,2),
(4.5,3),
(7,7),
(3,1),
(3,3),
(4.5,5),
(3,4),
(4.5,4),
(7,4),
(5,1),
(4.5,2),
(7,2),
(7,1),
(4.5,1);



SELECT round(sum(cost)::numeric, 4) as cost
FROM pgr_tsp('SELECT id, x, y FROM vertex_table ORDER BY id', 6, 5);
SELECT case when r=array[1,2,3,0] then 'OK' when r=array[1,0,3,2] then 'OK' else 'FAIL' end from (SELECT array_agg(id) as r from (SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[],1)) as a) as b;
SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[],1,2);
SELECT round(sum(cost)::numeric, 4) as cost
FROM pgr_tsp('SELECT id::integer, st_x(the_geom) as x,st_x(the_geom) as y FROM edge_table_vertices_pgr  ORDER BY id', 6, 5);
SELECT round(sum(cost)::numeric, 4) as cost from pgr_tsp('SELECT source_id as id, x, y from tsp_00 where source_id in (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22)', 1);
SELECT round(sum(cost)::numeric, 4) as cost from pgr_tsp('SELECT source_id as id, x, y from tsp_00 where source_id<12 order by source_id', 6);
SELECT * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],0,1);
\echo '----------------------------------'
SELECT * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],0,2);
\echo '----------------------------------'
SELECT * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],0,3);
\echo '----------------------------------'
SELECT * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],0,4);
\echo '----------------------------------'
SELECT * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],1,0);
\echo '----------------------------------'
SELECT * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],1,2);
\echo '----------------------------------'
SELECT * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],1,3);
\echo '----------------------------------'
SELECT * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],1,4);
\echo '----------------------------------'
SELECT * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],2,0);
\echo '----------------------------------'
SELECT * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],2,1);
\echo '----------------------------------'
SELECT * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],2,3);
\echo '----------------------------------'
SELECT * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],2,4);
\echo '----------------------------------'
SELECT * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],3,0);
\echo '----------------------------------'
SELECT * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],3,1);
\echo '----------------------------------'
SELECT * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],3,2);
\echo '----------------------------------'
SELECT * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],3,4);
\echo '----------------------------------'
SELECT * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],4,0);
\echo '----------------------------------'
SELECT * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],4,1);
\echo '----------------------------------'
SELECT * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],4,2);
\echo '----------------------------------'
SELECT * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],4,3);
\echo '----------------------------------'
SELECT seq, id1, id2, round(cost::numeric, 2) as cost from pgr_tsp('SELECT source_id as id, x, y from tsp_00 where source_id<12 order by source_id', 6, 5);
\echo '----------------------------------1(in ints)'
SELECT case when r=array[1,2,3,0] then 'OK' when r=array[1,0,3,2] then 'OK' else 'FAIL' end from (SELECT array_agg(id) as r from (SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[],1)) as a) as b;
\echo '----------------------------------1(in tenths)'
SELECT case when r=array[1,2,3,0] then 'OK' when r=array[1,0,3,2] then 'OK' else 'FAIL' end from (SELECT array_agg(id) as r from (SELECT seq, id FROM pgr_tsp('{{0,.1,.2,.3},{.1,0,.4,.5},{.2,.4,0,.6},{.3,.5,.6,0}}'::float8[],1)) as a) as b;
\echo '----------------------------------1-0'
SELECT seq, id FROM pgr_tsp('{{0,1,3,3},{1,0,2,2},{3,2,0,2},{3,2,2,0}}'::float8[],1,0);
\echo '----------------------------------1-2'
SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[],1,2);
\echo '----------------------------------1-3'
SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[],1,3);
\echo '----------------------------------0-1'
SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[],0,1);
\echo '----------------------------------2-1'
SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[],2,1);
\echo '----------------------------------3-1'
SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[],3,1);

