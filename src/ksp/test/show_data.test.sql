--------------------------------------------------
-- Test to see that we are working with the same data 
--------------------------------------------------
\echo network
select * from network order by id;
\echo nodes
select * from nodes order by id;
--------------------------------------------------
\echo edge_table
select * from edge_table order by id;
\echo edge_table_vertices
select * from edge_table_vertices_pgr order by id;
--------------------------------------------------
\echo paralell
select * from parallel order by id;
\echo paralell_vertices
select * from parallel_vertices_pgr order by id;
 
