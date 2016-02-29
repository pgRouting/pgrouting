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
select * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],0,1);
\echo '----------------------------------'
select * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],0,2);
\echo '----------------------------------'
select * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],0,3);
\echo '----------------------------------'
select * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],0,4);
\echo '----------------------------------'
select * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],1,0);
\echo '----------------------------------'
select * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],1,2);
\echo '----------------------------------'
select * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],1,3);
\echo '----------------------------------'
select * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],1,4);
\echo '----------------------------------'
select * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],2,0);
\echo '----------------------------------'
select * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],2,1);
\echo '----------------------------------'
select * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],2,3);
\echo '----------------------------------'
select * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],2,4);
\echo '----------------------------------'
select * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],3,0);
\echo '----------------------------------'
select * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],3,1);
\echo '----------------------------------'
select * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],3,2);
\echo '----------------------------------'
select * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],3,4);
\echo '----------------------------------'
select * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],4,0);
\echo '----------------------------------'
select * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],4,1);
\echo '----------------------------------'
select * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],4,2);
\echo '----------------------------------'
select * from pgr_tsp('{{0,1,2,3,4},{1,0,3,2,1},{2,3,0,4,2},{3,2,4,0,3},{4,1,2,3,0}}'::float8[],4,3);
\echo '----------------------------------'
