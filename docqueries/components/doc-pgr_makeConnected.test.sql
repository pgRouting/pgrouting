\echo -- q1
SELECT * FROM pgr_makeConnected(
    'SELECT id, source, target, cost, reverse_cost
     FROM edge_table'
);
\echo -- q2
