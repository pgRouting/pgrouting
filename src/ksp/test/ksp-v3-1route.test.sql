--------------------------------------------------------------------------------
--              PGR_ksp V3 only
--------------------------------------------------------------------------------




-- Examples to handle the one flag to choose signatures using :ref:`fig1-direct-Cost-Reverse`
------------------------------------------------------------------------------------------


   SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 1, 
      true   
   );

   SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 1,
      directed:=true   -- takes the new signature
   );

   SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 1
   );



--Examples using :ref:`fig1-direct-Cost-Reverse`
-------------------------------------------------------------------------------


   SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 1
   );
  

   SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 1, heap_paths:=true
   );

   SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 1, true, true
   );

--Example for :ref:`fig2-undirect-Cost-Reverse` 
-------------------------------------------------------------------------------


  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 1, directed:=false
   );

  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 1, directed:=false, heap_paths:=true
   );

  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 1, false, true
   );



-- Examples for :ref:`fig3-direct-Cost` 
-------------------------------------------------------------------------------


  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 3, 2
   );


  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 1
   );


  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 1, heap_paths:=true
   );

  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 1, true, true
   );

--Examples for :ref:`fig4-undirect-Cost` 
-------------------------------------------------------------------------------


  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 1, directed:=false
   );
  
  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 1, directed:=false, heap_paths:=true
   );

  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 1, false, true
   );
