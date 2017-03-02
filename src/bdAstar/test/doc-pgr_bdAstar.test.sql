
        -- x1, y1, x2, y2
\echo -- q1
SELECT * FROM pgr_bdAstar(
    'SELECT id, source, target, cost, reverse_cost, x1,y1,x2,y2
    FROM edge_table',
    2, 3
);

\echo -- q2
SELECT * FROM pgr_bdAstar(
    'SELECT id, source, target, cost, reverse_cost, x1,y1,x2,y2
    FROM edge_table',
    2, 3,
    true, heuristic := 2
);
\echo -- q3
SELECT * FROM pgr_bdAstar(
    'SELECT id, source, target, cost, reverse_cost, x1,y1,x2,y2
    FROM edge_table',
    2, ARRAY[3, 11],
    heuristic := 3, factor := 3.5
);
\echo -- q4
SELECT * FROM pgr_bdAstar(
    'SELECT id, source, target, cost, reverse_cost, x1,y1,x2,y2
    FROM edge_table',
    ARRAY[2, 7], 3,
    false, heuristic := 4
);
\echo -- q5
SELECT * FROM pgr_bdAstar(
    'SELECT id, source, target, cost, reverse_cost, x1,y1,x2,y2
    FROM edge_table',
    ARRAY[2, 7], ARRAY[3, 11],
    factor := 0.5
);
\echo -- q6
SELECT * FROM pgr_bdAstar(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT,
        x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT
     FROM edge_table',
    2, 3,
    false, -- undirected
    false  -- no reverse_cost
);
\echo -- q7

