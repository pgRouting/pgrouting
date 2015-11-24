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


SELECT set_eq(
'SELECT *
FROM pgr_kdijkstraPath(
    ''SELECT id, source, target, cost FROM edge_table'',
    10, array[4,12], false, false)',

'SELECT seq-1, end_vid::integer, node::integer, edge::integer, cost 
FROM pgr_dijkstra(
    ''SELECT id, source, target, cost FROM edge_table'',
    10, array[4,12], false)'
);

-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

