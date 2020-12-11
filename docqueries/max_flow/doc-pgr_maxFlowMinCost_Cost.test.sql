
\echo -- q1
SELECT * FROM pgr_MaxFlowMinCost_Cost(
    'SELECT id,
     source, target,
     capacity, reverse_capacity,
     cost, reverse_cost FROM edge_table',
    2, 3
);

\echo -- q2
SELECT * FROM pgr_MaxFlowMinCost_Cost(
    'SELECT id,
     source, target,
     capacity, reverse_capacity,
     cost, reverse_cost FROM edge_table',
    ARRAY[1, 7, 14], 12
);

\echo -- q3
SELECT * FROM pgr_MaxFlowMinCost_Cost(
    'SELECT id,
     source, target,
     capacity, reverse_capacity,
     cost, reverse_cost FROM edge_table',
    13, ARRAY[7, 1, 4]
);

\echo -- q4
SELECT * FROM pgr_MaxFlowMinCost_Cost(
    'SELECT id,
     source, target,
     capacity, reverse_capacity,
     cost, reverse_cost FROM edge_table',
    ARRAY[7, 13], ARRAY[3, 9]
);

\echo -- q5
SELECT * FROM pgr_MaxFlowMinCost_Cost(
    'SELECT id,
     source, target,
     capacity, reverse_capacity,
     cost, reverse_cost FROM edge_table',
    'SELECT * FROM ( VALUES (7, 3), (13, 9) ) AS t(source, target)'
);

\echo -- q6
