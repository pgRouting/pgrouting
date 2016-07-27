--These tests used the sample data provided here: http://docs.pgrouting.org/2.2/en/doc/src/developer/sampledata.html#sampledata

BEGIN;
SET client_min_messages TO NOTICE;
\i sample_data_categories.sql

\echo -- q1
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id,
            source,
            target,
            c1.capacity as capacity,
            c2.capacity as reverse_capacity
    FROM edge_table AS edges, category as c1, category as c2
    WHERE edges.category = c1.category AND edges.reverse_category = c2.category'
    , 6, 11
);

\echo -- q2
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id,
            source,
            target,
            c1.capacity as capacity,
            c2.capacity as reverse_capacity
    FROM edge_table AS edges, category as c1, category as c2
    WHERE edges.category = c1.category AND edges.reverse_category = c2.category'
    , 6, ARRAY[12,1,13]::INTEGER[]
);

\echo -- q3
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id,
            source,
            target,
            c1.capacity as capacity,
            c2.capacity as reverse_capacity
    FROM edge_table AS edges, category as c1, category as c2
    WHERE edges.category = c1.category AND edges.reverse_category = c2.category'
    , ARRAY[4,8,11]::INTEGER[], 10
);

\echo -- q4
SELECT * FROM pgr_maxflowboykovkolmogorov(
    'SELECT id,
            source,
            target,
            c1.capacity as capacity,
            c2.capacity as reverse_capacity
    FROM edge_table AS edges, category as c1, category as c2
    WHERE edges.category = c1.category AND edges.reverse_category = c2.category'
    , ARRAY[2,5,10]::INTEGER[], ARRAY[4,9,12]::INTEGER[]
);

\echo -- q5
ROLLBACK;