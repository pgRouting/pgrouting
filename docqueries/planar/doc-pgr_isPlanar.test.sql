\echo -- q1
SELECT * FROM pgr_isPlanar(
    'SELECT id, source, target, cost, reverse_cost
     FROM edge_table'
);
\echo -- q2

INSERT INTO edge_table (source, target, cost, reverse_cost) VALUES
(3, 9, 1, 1), (3, 16, 1, 1),
(4, 6, 1, 1), (4, 16, 1, 1),
(6, 16, 1, 1),
(9, 16, 1, 1);

\echo -- q3
SELECT * FROM pgr_isPlanar(
    'SELECT id, source, target, cost, reverse_cost
     FROM edge_table'
);

\echo -- q4
