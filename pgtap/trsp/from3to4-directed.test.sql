\i setup.sql

SELECT plan(3);

UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;


PREPARE q1 AS
SELECT * from pgr_trsp(
    'select id::INTEGER, source::INTEGER, target::INTEGER,cost, reverse_cost from edge_table',
    1, 1,
    true, true);

PREPARE q21 AS
SELECT seq, id1, id2, cost::text from pgr_trsp(
    'select id::INTEGER, source::INTEGER, target::INTEGER,cost, reverse_cost from edge_table',
    3, 4,
    true, true);

PREPARE q22 AS
SELECT seq-1, node::INTEGER, edge::INTEGER, cost::text from pgr_dijkstra(
    'select id, source, target, cost, reverse_cost from edge_table',
    3, 4, true);

PREPARE q31 AS
SELECT seq, id1, id2, cost::text from pgr_trsp(
    'select id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost from edge_table',
    3, 4,
    true, true,
    'select 9::INTEGER as target_id, ''5''::TEXT as via_path,  100.2::FLOAT to_cost');

PREPARE q32 AS
SELECT (row_number() over() -1)::INTEGER, node::INTEGER,
(CASE WHEN edge = -2 THEN -1 ELSE edge END)::INTEGER, cost::text
FROM pgr_dijkstraVia(
    'select id, source, target, cost, reverse_cost from edge_table',
    ARRAY[3, 2, 4],
    true) where edge != -1;


SELECT is_empty('q1', '1: No path from 1 to 1');
SELECT set_eq('q21','q22','2: without retrictions returns the same as pgr_dijkstra');
SELECT set_eq('q31','q32','3: with retrictions returns expected path');

-- Finish the tests and clean up.

SELECT * FROM finish();


ROLLBACK
