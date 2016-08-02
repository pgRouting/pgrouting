--These tests used the sample data provided here: http://docs.pgrouting.org/2.2/en/doc/src/developer/sampledata.html#sampledata

BEGIN;
SET client_min_messages TO NOTICE;

\echo -- q1
SELECT * FROM pgr_edgeDisjointPaths(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
    3, 5
);

\echo -- q2
SELECT * FROM pgr_edgeDisjointPaths(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
    3, 5,
    directed := false
);

\echo -- q3
SELECT * FROM pgr_edgeDisjointPaths(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
    3, ARRAY[4, 5, 10]
);

\echo -- q4
SELECT * FROM pgr_edgeDisjointPaths(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
    ARRAY[3, 6], 5
);

\echo -- q5
SELECT * FROM pgr_edgeDisjointPaths(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
    ARRAY[3, 6], ARRAY[4, 5, 10]
);

\echo -- q9
ROLLBACK;
