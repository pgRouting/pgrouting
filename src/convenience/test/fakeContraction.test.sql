BEGIN;
\echo --q1
SELECT * FROM pgr_fakeContraction('edge_table'::text, ARRAY[1]);
\echo --q2
SELECT * FROM pgr_fakeContraction('edge_table'::text, ARRAY[1, 2]);
ROLLBACK;