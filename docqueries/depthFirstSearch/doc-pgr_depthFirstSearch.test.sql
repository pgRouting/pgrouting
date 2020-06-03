\echo --q1
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    2
);
\echo --q2
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[11,12], max_depth := 2
);
\echo --q3
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
\echo --q4
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
\echo --q5
SELECT * FROM pgr_depthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[-10,20]
);
\echo --q6
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
\echo --q7
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
\echo --q8
