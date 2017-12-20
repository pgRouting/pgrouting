
\echo -- q1
SELECT * FROM pgr_lineGraphFull(
    'SELECT id, source, target, cost FROM edge_table'
);
\echo -- q2
