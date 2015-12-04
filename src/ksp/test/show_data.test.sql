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
--------------------------------------------------
-- Test to see that we are working with the same data 
--------------------------------------------------
\echo network
select * from network order by id;
\echo nodes
select * from nodes order by id;
--------------------------------------------------
\echo edge_table
select * from edge_table order by id;
\echo edge_table_vertices
select * from edge_table_vertices_pgr order by id;
--------------------------------------------------
\echo paralell
select * from parallel order by id;
\echo paralell_vertices
select * from parallel_vertices_pgr order by id;
 
