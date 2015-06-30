--------------------------------------------------------------------------------
--              PGR_ksp V3
--------------------------------------------------------------------------------




-- Examples to handle the one flag to choose signatures using :ref:`fig1-direct-Cost-Reverse`
------------------------------------------------------------------------------------------


   SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2, 
      true   -- takes the (V2.0) signature (has_rcost = true and works on directed graph)
   );


   SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2,
      directed:=true   -- takes the new signature
   );

   SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2
   );



--Examples using :ref:`fig1-direct-Cost-Reverse`
-------------------------------------------------------------------------------


   SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2
   );
  

   SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2, heap_paths:=true
   );

   SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2, true, true
   );

--Example for :ref:`fig2-undirect-Cost-Reverse` 
-------------------------------------------------------------------------------


  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2, directed:=false
   );

  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2, directed:=false, heap_paths:=true
   );

  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2, false, true
   );



-- Examples for :ref:`fig3-direct-Cost` 
-------------------------------------------------------------------------------


  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 3, 2
   );


  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 2
   );


  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 2, heap_paths:=true
   );

  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 2, true, true
   );

--Examples for :ref:`fig4-undirect-Cost` 
-------------------------------------------------------------------------------


  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 2, directed:=false
   );
  
  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 2, directed:=false, heap_paths:=true
   );

  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 2, false, true
   );

