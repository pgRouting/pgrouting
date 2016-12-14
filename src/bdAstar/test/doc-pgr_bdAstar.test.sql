
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
    false
);
\echo -- q3
SET client_min_messages TO notice;
SELECT * FROM pgr_bdAstar(
    'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost::FLOAT,
        x1::FLOAT, y1::FLOAT, x2::FLOAT, y2::FLOAT
     FROM edge_table',
    2, 3,
    false, -- undirected
    false  -- no reverse_cost
);
\echo -- q4

