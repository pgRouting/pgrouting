\i setup.sql
SET client_min_messages=WARNING;

SELECT plan(2);

PREPARE q1 AS
SELECT id1, id2, round(cost::numeric, 2) as cost
FROM pgr_apspWarshall('select id::INTEGER, source::INTEGER, target::INTEGER, cost from edge_table', false, false);

PREPARE q2 AS
SELECT  id1, id2, round(cost::numeric, 2) as cost
FROM pgr_apspWarshall('select id::INTEGER, source::INTEGER, target::INTEGER, cost from edge_table', TRUE, false);

PREPARE q11 AS
SELECT start_vid::INTEGER AS id1, end_vid::INTEGER AS id2, round(agg_cost::numeric, 2) as cost
FROM pgr_floydWarshall('select id, source, target, cost from edge_table', false);

PREPARE q12 AS
SELECT start_vid::INTEGER AS id1, end_vid::INTEGER AS id2, round(agg_cost::numeric, 2) as cost
FROM pgr_floydWarshall('select id, source, target, cost from edge_table', true);

SELECT set_eq('q1', 'q11','1: With undirected: Compare with pgr_floydWarshall -> must give the same results');
SELECT set_eq('q2', 'q12','2: With directed: Compare with pgr_floydWarshall -> must give the same results');

-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

