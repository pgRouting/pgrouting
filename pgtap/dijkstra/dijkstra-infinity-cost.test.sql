\i setup.sql

SELECT plan(8);

PREPARE q0 AS
SELECT agg_cost FROM pgr_dijkstra( 'select id, source, target, cost from edge_table',
    7, 6) ORDER BY seq DESC LIMIT 1;

PREPARE update2infinity AS
UPDATE edge_table SET cost = 'Infinity' WHERE id = 7;

PREPARE q1 AS
SELECT agg_cost FROM pgr_dijkstra( 'select id, source, target, cost from edge_table',
    7, 8) ORDER BY seq DESC LIMIT 1;

PREPARE q2 AS
SELECT agg_cost FROM pgr_dijkstra( 'select id, source, target, cost from edge_table',
    8, 5) ORDER BY seq DESC LIMIT 1;

PREPARE q3 AS
SELECT agg_cost FROM pgr_dijkstra( 'select id, source, target, cost from edge_table',
    5, 6) ORDER BY seq DESC LIMIT 1;

PREPARE q4 AS
SELECT agg_cost FROM pgr_dijkstra( 'select id, source, target, cost from edge_table',
    7, 5) ORDER BY seq DESC LIMIT 1;

PREPARE q5 AS
SELECT agg_cost FROM pgr_dijkstra( 'select id, source, target, cost from edge_table',
    8, 6) ORDER BY seq DESC LIMIT 1;

PREPARE q6 AS
SELECT agg_cost FROM pgr_dijkstra( 'select id, source, target, cost from edge_table',
    7, 6) ORDER BY seq DESC LIMIT 1;

-- test for infinity if there is no alternative
SELECT results_eq('q0', 'SELECT cast(3 as double precision) as agg_cost;');
SELECT lives_ok('update2infinity', 'updating an edge to ''Infinity'' should be possible');
SELECT results_eq('q1', 'SELECT cast(1 as double precision) as agg_cost;');
SELECT results_eq('q2', 'SELECT cast(''Infinity'' as double precision) as agg_cost;', 'Routing through edge 7 should be ''Infinity''');
SELECT results_eq('q3', 'SELECT cast(1 as double precision) as agg_cost;');
SELECT results_eq('q4', 'SELECT cast(''Infinity'' as double precision) as agg_cost;', 'Routing through edge 7 should be ''Infinity''');
SELECT results_eq('q5', 'SELECT cast(''Infinity'' as double precision) as agg_cost;', 'Routing through edge 7 should be ''Infinity''');
SELECT results_eq('q6', 'SELECT cast(''Infinity'' as double precision) as agg_cost;', 'Routing through edge 7 should be ''Infinity''');

-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;
