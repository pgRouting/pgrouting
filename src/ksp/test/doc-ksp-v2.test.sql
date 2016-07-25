SET client_min_messages TO NOTICE;

\echo --q1

SELECT * FROM pgr_ksp(
   'SELECT id, source, target, cost FROM edge_table order by id',
   7, 12, 2, false
 );

\echo --q2
SELECT * FROM pgr_ksp(
   'SELECT id, source, target, cost, reverse_cost FROM edge_table order by id',
   7, 12, 2, true
 );

\echo --q3
