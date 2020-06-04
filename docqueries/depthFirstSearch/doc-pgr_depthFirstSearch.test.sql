\echo -- q1
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    2
);
\echo -- q2
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[11,12], max_depth := 2
);
\echo -- q3


-- Examples for :ref:`fig1-direct-Cost-Reverse`
-------------------------------------------------------------------------------

\echo -- q4
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    6
);
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[8,15]
);
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[8,15], max_depth := 2
);
\echo -- q5


-- Examples for :ref:`fig2-undirect-Cost-Reverse`
-------------------------------------------------------------------------------

\echo -- q6
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    6, directed := false
);
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[8,15], directed := false
);
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[8,15], max_depth := 2, directed := false
);
\echo -- q7


-- Example for Vertex Out of Graph
-------------------------------------------------------------------------------

\echo -- q8
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[-10,20]
);
\echo -- q9


-- Equivalences for :ref:`fig1-direct-Cost-Reverse`
-------------------------------------------------------------------------------

\echo -- q10
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    16
);
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    16, directed := true
);
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[16]
);
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    16, max_depth := 1
);
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[16], max_depth := 5
);
\echo -- q11


-- Equivalences for :ref:`fig2-undirect-Cost-Reverse`
-------------------------------------------------------------------------------

\echo -- q12
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    16, directed := false
);
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[16], directed := false
);
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    16, max_depth := 1, directed := false
);
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[16], max_depth := 5, directed := false
);
\echo -- q13
