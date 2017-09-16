
\echo -- q1
SELECT * FROM pgr_turnPenaltyGraph(
    'SELECT id, source, target, cost FROM edge_table'
);
\echo -- q2
