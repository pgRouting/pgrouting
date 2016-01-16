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
\echo '----------------------------------1(in ints)'
select case when r=array[1,2,3,0] then 'OK' when r=array[1,0,3,2] then 'OK' else 'FAIL' end from (select array_agg(id) as r from (SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[],1)) as a) as b;
\echo '----------------------------------1(in tenths)'
select case when r=array[1,2,3,0] then 'OK' when r=array[1,0,3,2] then 'OK' else 'FAIL' end from (select array_agg(id) as r from (SELECT seq, id FROM pgr_tsp('{{0,.1,.2,.3},{.1,0,.4,.5},{.2,.4,0,.6},{.3,.5,.6,0}}'::float8[],1)) as a) as b;
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

