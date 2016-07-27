--These tests used the sample data provided here: http://docs.pgrouting.org/2.2/en/doc/src/developer/sampledata.html#sampledata

BEGIN;
SET client_min_messages TO NOTICE;

\echo -- q1
SELECT * FROM pgr_edgedisjointpaths(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
    3, 5
);

\echo -- q2
SELECT * FROM pgr_edgedisjointpaths(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
    3, 5,
    FALSE
);

\echo -- q3
SELECT * FROM pgr_edgedisjointpaths(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
    ARRAY[10,12,4,2]::INTEGER[], 6
);

\echo -- q4
SELECT * FROM pgr_edgedisjointpaths(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
    ARRAY[10,12,4,2]::INTEGER[], 6,
    FALSE
);

\echo -- q5
SELECT * FROM pgr_edgedisjointpaths(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
    6, ARRAY[7,1,12,3]::INTEGER[]
);

\echo -- q6
SELECT * FROM pgr_edgedisjointpaths(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
    6, ARRAY[7,1,12,3]::INTEGER[],
    FALSE
);

\echo -- q7
SELECT * FROM pgr_edgedisjointpaths(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
    ARRAY[3,11]::INTEGER[], ARRAY[4,6,10]::INTEGER[]
);

\echo -- q8
SELECT * FROM pgr_edgedisjointpaths(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
    ARRAY[3,11]::INTEGER[], ARRAY[4,6,10]::INTEGER[],
    FALSE
);

\echo -- q9
ROLLBACK;