--These tests used the sample data provided here: http://docs.pgrouting.org/2.2/en/doc/src/developer/sampledata.html#sampledata
--The edge in the edge table are augmented with road-like categories for capacity values.



\echo -- q1
SELECT * FROM pgr_maxFlow(
    'SELECT id,
            source,
            target,
            capacity,
            reverse_capacity
    FROM edge_table'
    , 6, 11
);

\echo -- q2
SELECT * FROM pgr_maxFlow(
    'SELECT id,
            source,
            target,
            capacity,
            reverse_capacity
    FROM edge_table'
    , 6, ARRAY[11, 1, 13]
);

\echo -- q3
SELECT * FROM pgr_maxFlow(
    'SELECT id,
            source,
            target,
            capacity,
            reverse_capacity
    FROM edge_table'
    , ARRAY[6, 8, 12], 11
);

\echo -- q4
SELECT * FROM pgr_maxFlow(
    'SELECT id,
            source,
            target,
            capacity,
            reverse_capacity
    FROM edge_table'
    , ARRAY[6, 8, 12], ARRAY[1, 3, 11]
);

\echo -- q5

