/* --q1
SELECT * FROM pgr_breadthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    2
);
/* --q2
SELECT * FROM pgr_breadthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[11,12], max_depth := 2
);
/* --q3
SELECT * FROM pgr_breadthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    ARRAY[11,12], max_depth := 2, directed := false
);
/* --q4
SELECT * FROM pgr_breadthFirstSearch(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    -10
);
/* --q5
