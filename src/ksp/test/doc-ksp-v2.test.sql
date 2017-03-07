
\echo --q1

SELECT * FROM pgr_ksp(
   'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table order by id',
   7, 12, 2, false
 );

\echo --q2
SELECT * FROM pgr_ksp(
   'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table order by id',
   7, 12, 2, true
 );

\echo --q3
