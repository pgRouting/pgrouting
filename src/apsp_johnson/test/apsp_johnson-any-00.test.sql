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
    SELECT id1, id2, round(cost::numeric, 2) as cost
    FROM pgr_apspJohnson('select id, source, target, cost from edge_table');


    PREPARE q11 AS
    SELECT start_vid::INTEGER AS id1, end_vid::INTEGER AS id2, round(agg_cost::numeric, 2) as cost
    FROM pgr_johnson('select id, source, target, cost from edge_table', TRUE);


    SELECT set_eq('q1', 'q11','1: With directed: Compare with pgr_johnson -> must give the same results');

    -- Finish the tests and clean up.
    SELECT * FROM finish();
    ROLLBACK;

