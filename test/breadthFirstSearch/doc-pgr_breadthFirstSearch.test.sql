\echo --q1
SELECT * FROM pgr_breadthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    2
);
\echo --q2
SELECT * FROM pgr_breadthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[11,12], max_depth := 2
);
\echo --q3
SELECT * FROM pgr_breadthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[11,12], max_depth := 2, directed := false
);
\echo --q4
SELECT * FROM pgr_breadthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    -10
);
\echo --q5
