
\echo -- q1
SELECT * FROM pgr_chinesePostman(
    'SELECT id,
     source, target,
     cost, reverse_cost FROM edge_table where id < 17'
);

\echo -- q2
SELECT * FROM pgr_chinesePostman(
    'SELECT id,
     source, target,
     cost, reverse_cost FROM edge_table where id = 9 or id = 11 or id = 13 or id = 15'
);

\echo -- q3
