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

SET client_min_messages TO warning;
SELECT PLAN(31);

SELECT * INTO incr_table FROM edge_table WHERE id < 9;
UPDATE incr_table SET source = NULL, target = NULL;                                -- unknown
SELECT is((SELECT count(*)::INTEGER FROM incr_table), 8, 'initial table has 8 edges');


SELECT is((SELECT count(*)::INTEGER FROM incr_table WHERE source is NULL), 8, '8 edges are missing source');
SELECT is((SELECT count(*)::INTEGER FROM incr_table WHERE target is NULL), 8, '8 edges are missing target');
SELECT hasnt_table('incr_table_vertices_pgr', 'incr_table_vertices_pgr table does not exist');

SELECT is((SELECT pgr_createTopology('incr_table',0.0001)), 'OK','OK Creating the topology');
SELECT has_table('incr_table_vertices_pgr', 'incr_table_vertices_pgr table now exist');
SELECT is((SELECT count(*)::INTEGER FROM incr_table WHERE source is NULL), 0, '0 edges are missing source');
SELECT is((SELECT count(*)::INTEGER FROM incr_table WHERE target is NULL), 0, '0 edges are missing target');
SELECT is((SELECT count(*)::INTEGER FROM incr_table_vertices_pgr), 8, 'Now we have 8 vertices');
SELECT is((SELECT count(*)::INTEGER FROM incr_table_vertices_pgr WHERE cnt is NULL), 8, '8 vertices are missing cnt');
SELECT is((SELECT count(*)::INTEGER FROM incr_table_vertices_pgr WHERE chk is NULL), 8, '8 vertices are missing chk');
SELECT is((SELECT count(*)::INTEGER FROM incr_table_vertices_pgr WHERE ein is NULL), 8, '8 vertices are missing ein');
SELECT is((SELECT count(*)::INTEGER FROM incr_table_vertices_pgr WHERE eout is NULL), 8, '8 vertices are missing eout');

INSERT INTO incr_table(id, dir, cost, reverse_cost, x1, y1, x2, y2, the_geom)
SELECT  id, dir, cost, reverse_cost, x1, y1, x2, y2, the_geom FROM edge_table WHERE id >= 9;
SELECT is((SELECT count(*)::INTEGER FROM incr_table), 18, 'After inserting table has 18 edges');

SELECT is((SELECT count(*)::INTEGER FROM incr_table WHERE source is NULL), 10, '10 edges are missing source');
SELECT is((SELECT count(*)::INTEGER FROM incr_table WHERE target is NULL), 10, '10 edges are missing target');
-- 10 edges must be updated the rest is untouched
SELECT is((SELECT pgr_createTopology('incr_table',0.0001,clean:=false)), 'OK', 'OK Creating the topology');
SELECT is((SELECT count(*)::INTEGER FROM incr_table WHERE source is NULL), 0, '0 edges are missing source');
SELECT is((SELECT count(*)::INTEGER FROM incr_table WHERE target is NULL), 0, '0 edges are missing target');


-- 12 edges have 'B' the rest is wiped
SELECT is((SELECT pgr_createTopology('incr_table',0.0001,rows_where:='dir=''B''',clean:=true)), 'OK', 'OK Creating the topology');
SELECT is((SELECT count(*)::INTEGER FROM incr_table WHERE source is NULL), 6, '6 edges are missing source');
SELECT is((SELECT count(*)::INTEGER FROM incr_table WHERE target is NULL), 6, '6 edges are missing target');

-- 6 edges must be processed, the rest is untouched
SELECT is((SELECT pgr_createTopology('incr_table',0.0001,clean:=false)), 'OK', 'OK Creating the topology');
SELECT is((SELECT count(*)::INTEGER FROM incr_table WHERE source is NULL), 0, '0 edges are missing source');
SELECT is((SELECT count(*)::INTEGER FROM incr_table WHERE target is NULL), 0, '0 edges are missing target');

--12 edgedds have 'B' the rest is untouched
SELECT is((SELECT pgr_createTopology('incr_table',0.0001,rows_where:='dir=''B''',clean:=false)), 'OK', 'OK Creating the topology');
SELECT is((SELECT count(*)::INTEGER FROM incr_table WHERE source is NULL), 0, '0 edges are missing source');
SELECT is((SELECT count(*)::INTEGER FROM incr_table WHERE target is NULL), 0, '0 edges are missing target');


SELECT is((SELECT pgr_createTopology('incr_table',0.0001,rows_where:='dir=''B''')), 'OK', 'OK Creating the topology');
SELECT is((SELECT count(*)::INTEGER FROM incr_table WHERE source is NULL), 0, '0 edges are missing source');
SELECT is((SELECT count(*)::INTEGER FROM incr_table WHERE target is NULL), 0, '0 edges are missing target');

SELECT finish();
ROLLBACK;
