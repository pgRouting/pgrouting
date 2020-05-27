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
along with this program; if not, write TO the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/
\i setup.sql

SELECT PLAN(3);

SET client_min_messages = WARNING;

-- The following should be OK
-- id INTEGER
SELECT * INTO edges2 FROM edge_table;
ALTER TABLE edges2 ALTER COLUMN id SET DATA TYPE INTEGER;

SELECT is(pgr_createVerticesTable('edges2'), 'OK', '1');
DROP TABLE IF EXISTS public.edges2_vertices_pgr;


-- source INTEGER
DROP TABLE IF EXISTS public.edges2;
SELECT * INTO edges2 FROM edge_table;
ALTER TABLE edges2 ALTER COLUMN source SET DATA TYPE INTEGER;

SELECT is(pgr_createVerticesTable('edges2'), 'OK', '1');
DROP TABLE IF EXISTS public.edges2_vertices_pgr;


-- target INTEGER
DROP TABLE IF EXISTS public.edges2;
SELECT * INTO edges2 FROM edge_table;
ALTER TABLE edges2 ALTER COLUMN target SET DATA TYPE INTEGER;

SELECT is(pgr_createVerticesTable('edges2'), 'OK', '1');
DROP TABLE IF EXISTS public.edges2_vertices_pgr;

SELECT finish();

