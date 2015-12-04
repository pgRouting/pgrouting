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
-- test for issue 353

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
    SELECT plan(3);

    PREPARE q1 AS
    SELECT * FROM pgr_dijkstra( 'select id, source, target, cost from edge_table',
        array[2,7], array[5,6]);

    PREPARE q2 AS
    SELECT * FROM pgr_dijkstra( 'select id, source, target, cost from edge_table', 
        array[2,7], array[17,18]);

    Prepare q3 AS
    (SELECT * FROM pgr_dijkstra( 'select id, source, target, cost from edge_table',
            array[2,7], array[5,6]))
    UNION
    (SELECT * FROM pgr_dijkstra( 'select id, source, target, cost from edge_table', 
            array[2,7], array[17,18]));

    PREPARE q4 AS
    SELECT * FROM pgr_dijkstra( 'select id, source, target, cost from edge_table', 
        array[2,7], array[5,6,17,18]);

    SELECT is_empty( 'q2', '1');
    SELECT set_eq( 'q1', 'q3', '2');
    SELECT set_eq( 'q1', 'q4', '3');


    -- Finish the tests and clean up.
    SELECT * FROM finish();
    ROLLBACK;
