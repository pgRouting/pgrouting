--------------------------------------------------------------------------------
--              PGR_KSP V3
--------------------------------------------------------------------------------

\echo --q1

   SELECT * FROM pgr_KSP(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2,
      directed:=true   -- takes the new signature
   );

   SELECT * FROM pgr_KSP(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2
   );

\echo --q2

   SELECT * FROM pgr_KSP(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2
   );


   SELECT * FROM pgr_KSP(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2, heap_paths:=true
   );

   SELECT * FROM pgr_KSP(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2, true, true
   );


\echo --q3

  SELECT * FROM pgr_KSP(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2, directed:=false
   );

  SELECT * FROM pgr_KSP(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2, false, true
   );


\echo --q4

  SELECT  * FROM pgr_KSP(
     'SELECT id, source, target, cost FROM edge_table',
      2, 3, 2
   );


  SELECT  * FROM pgr_KSP(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 2
   );

  SELECT   * FROM pgr_KSP(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 2, heap_paths:=true
   );

  SELECT  * FROM pgr_KSP(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 2, true, true
   );


\echo --q5

  SELECT  * FROM pgr_KSP(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 2, directed:=false
   );

  SELECT  * FROM pgr_KSP(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 2, directed:=false, heap_paths:=true
   );


\echo --q6
