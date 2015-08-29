--------------------------------------------------------------------------------
--              PGR_ksp V3 only
--------------------------------------------------------------------------------

-- data
SELECT id, source, target, cost, reverse_cost FROM edge_table order by id;


-- Examples to handle the one flag to choose signatures using :ref:`fig1-direct-Cost-Reverse`
------------------------------------------------------------------------------------------


   SELECT 1, * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 1, 
      true   
   );

   SELECT 2, * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 1,
      directed:=true   -- takes the new signature
   );

   SELECT 3, * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 1
   );



--Examples using :ref:`fig1-direct-Cost-Reverse`
-------------------------------------------------------------------------------


   SELECT 4, * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 1
   );
  

   SELECT 5, * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 1, heap_paths:=true
   );

   SELECT 6, * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 1, true, true
   );

--Example for :ref:`fig2-undirect-Cost-Reverse` 
-------------------------------------------------------------------------------


  SELECT 7, * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 1, directed:=false
   );

  SELECT 8, * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 1, directed:=false, heap_paths:=true
   );

  SELECT 9, * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 1, false, true
   );



-- Examples for :ref:`fig3-direct-Cost` 
-------------------------------------------------------------------------------


  SELECT 10, * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 3, 2
   );


  SELECT 11, seq, path_id, cost, agg_cost  FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 1
   );


  SELECT 12, seq, path_id,  cost, agg_cost FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 1, heap_paths:=true
   );

  SELECT 13, seq, path_id,  cost, agg_cost FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 1, true, true
   );

--Examples for :ref:`fig4-undirect-Cost` 
-------------------------------------------------------------------------------


  SELECT 14, * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 1, directed:=false
   );
  
  SELECT 15, * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 1, directed:=false, heap_paths:=true
   );

  SELECT 16, * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 1, false, true
   );
