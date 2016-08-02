--These tests used the sample data provided here: http://docs.pgrouting.org/2.2/en/doc/src/developer/sampledata.html#sampledata

BEGIN;
SET client_min_messages TO NOTICE;

\echo -- q1
SELECT * FROM pgr_maximumcardinalitymatching(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table'
);

\echo -- q2
SELECT * FROM pgr_maximumcardinalitymatching(
    'SELECT id, source, target, cost AS going, reverse_cost AS coming FROM edge_table',
    directed := false
);

\echo -- q3
ROLLBACK;
