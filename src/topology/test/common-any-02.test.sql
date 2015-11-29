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
select pgr_createTopology('edges2',0.000001,id:='eid');
select pgr_analyzegraph('edges2', 0.000001,id:='eid');
select count(*) from edges2_vertices_pgr where chk=1;
\echo '---------------------------------------'
select cnt, count(*) from edges2_vertices_pgr group by cnt order by cnt;
\echo '---------------------------------------'
select * from edges2_vertices_pgr order by id;
ROLLBACK;
