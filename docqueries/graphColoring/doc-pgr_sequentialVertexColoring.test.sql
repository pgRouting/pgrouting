\echo -- q1
SELECT * FROM pgr_sequentialVertexColoring(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id'
);
\echo -- q2
