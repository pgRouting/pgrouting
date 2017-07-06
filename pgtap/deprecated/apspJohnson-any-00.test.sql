\i setup.sql

set client_min_messages=WARNING;
SELECT plan(1);

PREPARE q1 AS
SELECT id1, id2, round(cost::numeric, 2) as cost
FROM pgr_apspJohnson('select id::INTEGER, source::INTEGER, target::INTEGER, cost from edge_table');


PREPARE q11 AS
SELECT start_vid::INTEGER AS id1, end_vid::INTEGER AS id2, round(agg_cost::numeric, 2) as cost
FROM pgr_johnson('select id::INTEGER, source::INTEGER, target::INTEGER, cost from edge_table', TRUE);


SELECT set_eq('q1', 'q11','1: With directed: Compare with pgr_johnson -> must give the same results');

-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;

