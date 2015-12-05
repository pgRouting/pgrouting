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

/*
This test is for the issue 244
only the posted query
Other tests check for all comibations of the sample data
*/

\set ECHO none
\set QUIET 1
-- Turn off echo and keep things quiet.

-- Format the output for nice TAP.
\pset format unaligned
\pset tuples_only true
\pset pager
\set VERBOSITY terse

-- Revert all changes on failure.
\set ON_ERROR_ROLLBACK true
\set ON_ERROR_STOP true
\set QUIET 1

BEGIN;
    SELECT plan(1);

    PREPARE q1 AS
    SELECT seq, id1, id2, cost::text FROM pgr_trsp('SELECT id, source, target, cost FROM edge_table_i244',7, 12, FALSE, false);

    PREPARE q2 AS
    (SELECT seq-1, node::INTEGER, edge::INTEGER, cost::text
      FROM pgr_dijkstra('SELECT id, source, target, cost FROM edge_table_i244 order by id',7, 12, FALSE))
    UNION ALL
    (SELECT seq-1, node::INTEGER, edge::INTEGER, cost::text
      FROM pgr_dijkstra('SELECT id, source, target, cost FROM edge_table_i244 order by source',7, 12, FALSE));

    SELECT bag_has('q2', 'q1', 'path found');

-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

