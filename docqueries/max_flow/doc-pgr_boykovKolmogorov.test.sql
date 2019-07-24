
\echo -- q1
SELECT * FROM pgr_boykovKolmogorov(
    'SELECT id,
            source,
            target,
            capacity,
            reverse_capacity
    FROM edge_table'
    , 6, 11
);

\echo -- q2
SELECT * FROM pgr_boykovKolmogorov(
    'SELECT id,
            source,
            target,
            capacity,
            reverse_capacity
    FROM edge_table'
    , 6, ARRAY[1, 3, 11]
);

\echo -- q3
SELECT * FROM pgr_boykovKolmogorov(
    'SELECT id,
            source,
            target,
            capacity,
            reverse_capacity
    FROM edge_table'
    , ARRAY[6, 8, 12], 11
);

\echo -- q4
SELECT * FROM pgr_boykovKolmogorov(
    'SELECT id,
            source,
            target,
            capacity,
            reverse_capacity
    FROM edge_table'
    , ARRAY[6, 8, 12], ARRAY[1, 3, 11]
);

\echo -- q5
