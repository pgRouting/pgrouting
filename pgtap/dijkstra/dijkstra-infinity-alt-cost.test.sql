\i setup.sql

SELECT plan(4);

PREPARE q0 AS
SELECT agg_cost FROM pgr_dijkstra( 'select id, source, target, cost from edge_table',
    7, 6) ORDER BY seq DESC LIMIT 1;

PREPARE update2infinity9 AS
UPDATE edge_table SET cost = 'Infinity' WHERE id = 9;

PREPARE update2k13 AS
UPDATE edge_table SET cost = 10 WHERE id = 13;

PREPARE q1 AS
SELECT agg_cost FROM pgr_dijkstra( 'select id, source, target, cost from edge_table',
    5, 12) ORDER BY seq DESC LIMIT 1;

-- test for infinity if alternative is available
SELECT results_eq('q0', 'SELECT cast(3 as double precision) as agg_cost;');
SELECT lives_ok('update2infinity9', 'updating an edge to ''Infinity'' should be possible for edge 9');
SELECT lives_ok('update2k13', 'updating an edge to 1000 should be possible for edge 13');
SELECT results_eq('q1', 'SELECT cast(12 as double precision) as agg_cost;', 'Routing around edge 9 should be possible');

-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;
