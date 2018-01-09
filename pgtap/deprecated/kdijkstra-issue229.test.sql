\i setup.sql
SET client_min_messages=WARNING;

SELECT plan(1);


SELECT set_eq(
'SELECT *
FROM pgr_kdijkstraPath(
    ''SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table'',
    10, array[4,12], false, false)',

'SELECT seq-1, end_vid::integer, node::integer, edge::integer, cost
FROM pgr_dijkstra(
    ''SELECT id, source, target, cost FROM edge_table'',
    10, array[4,12], false)'
);

-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

