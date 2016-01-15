\i setup.sql

SELECT plan(1);
UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

PREPARE q1 AS
SELECT seq, id1, id2, cost::TEXT FROM pgr_trsp(
    'select id, source::integer, target::integer, cost, reverse_cost from edge_table',
    1,     -- edge_id for start
    0.5,   -- midpoint of edge
    6,     -- edge_id of route end
    0.5,   -- midpoint of edge
    true,  -- directed graph?
    true,  -- has_reverse_cost?
    null); -- no turn restrictions


PREPARE q2 AS
SELECT seq-1, node, edge, cost::TEXT FROM pgr_withPointsVia(
    'select id, source::integer, target::integer, cost, reverse_cost from edge_table',
    ARRAY[1, 6],
    ARRAY[0.5, 0.5]) WHERE edge != -2;

SELECT set_eq('q2', 'q1', 'No turn restriction from 1 to 5 returns same as pgr_withPointsVia');

-- Finish the tests and clean up.
SELECT * FROM finish();
ROLLBACK;
