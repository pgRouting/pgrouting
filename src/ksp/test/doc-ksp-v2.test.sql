\echo --q1

SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost
 FROM pgr_ksp(
   'SELECT id, source, target, cost FROM edge_table order by id',
   7, 12, 2, false
 );

\echo --q2
SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost
 FROM pgr_ksp(
   'SELECT id, source, target, cost, reverse_cost FROM edge_table order by id',
   7, 12, 2, true
 );

\echo --q3
