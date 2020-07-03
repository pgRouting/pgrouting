\echo -- q1
SELECT * FROM pgr_sequentialVertexColoring(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table
    ORDER BY id'
);
\echo -- q2

-- Examples for :ref:`fig2-undirect-Cost-Reverse`
-------------------------------------------------------------------------------

\echo -- q3
SELECT * FROM pgr_sequentialVertexColoring(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id = 1
    ORDER BY id'
);
SELECT * FROM pgr_sequentialVertexColoring(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id < 5
    ORDER BY id'
);
SELECT * FROM pgr_sequentialVertexColoring(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id >= 4 AND id <= 6
    ORDER BY id'
);
SELECT * FROM pgr_sequentialVertexColoring(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id in (8, 10, 11, 12)
    ORDER BY id'
);
SELECT * FROM pgr_sequentialVertexColoring(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table
    WHERE id in (5, 8, 9, 11)
    ORDER BY id'
);
\echo -- q4
