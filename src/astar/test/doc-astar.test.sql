\echo --q1
SELECT * FROM pgr_astar(
    'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2 FROM edge_table',
    2, 12);
\echo --q2
SELECT * FROM pgr_astar(
    'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2 FROM edge_table',
    2, 12, heuristic := 1);
SELECT * FROM pgr_astar(
    'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2 FROM edge_table',
    2, 12, heuristic := 2);
\echo --q3
SELECT * FROM pgr_astar(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost, x1, y1, x2, y2 FROM edge_table',
    2, 12, true, true);
\echo --q4
