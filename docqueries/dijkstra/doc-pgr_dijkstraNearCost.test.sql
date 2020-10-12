
\echo -- q1
SELECT * FROM pgr_dijkstraNearCost(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    2, ARRAY[3, 6, 7]
);

\echo -- q2
SELECT * FROM pgr_dijkstraNearCost(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[3, 6, 7], 2,
    true,
    cap => 2
);
\echo -- q3
SELECT * FROM pgr_dijkstraNearCost(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    ARRAY[4, 9], ARRAY[3, 6, 7],
    directed => false
);
\echo -- q4
SELECT * FROM pgr_dijkstraNearCost(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT unnest(ARRAY[3, 6, 7]) as source, target FROM (SELECT unnest(ARRAY[4, 9]) AS target) a
    UNION
    SELECT unnest(ARRAY[4, 9]), target FROM (SELECT unnest(ARRAY[3, 6, 7]) AS target) b',
    global => false
);
\echo -- q5
