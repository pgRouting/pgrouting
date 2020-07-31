\echo -- q1
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table
    ORDER BY id',
    2
);
\echo -- q2
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table
    ORDER BY id',
    2, directed => false
);
\echo -- q3
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table
    ORDER BY id',
    2, max_depth => 2
);
\echo -- q4
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table
    ORDER BY id',
    ARRAY[11, 2], directed => false, max_depth => 2
);
\echo -- q5
UPDATE edge_table SET cost = cost + 0.001 * id * id, reverse_cost = reverse_cost + 0.001 * id * id;

SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost
    FROM edge_table
    WHERE id <= 4
    ORDER BY id DESC',
    1, directed => false
);
\echo -- q6
