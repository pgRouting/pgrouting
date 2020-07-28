\echo -- q1
SELECT * FROM pgr_makeConnected(
    'SELECT id, source, target, cost, reverse_cost
        FROM edge_table'
);

\echo -- q2
SELECT * FROM pgr_connectedComponents(
    'SELECT id, source, target, cost, reverse_cost
        FROM edge_table'
);

\echo -- q3

SELECT * FROM pgr_makeConnected(
    'SELECT id, source, target, cost, reverse_cost
        FROM edge_table'
);

\echo -- q4
