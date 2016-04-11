\echo --q1
SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT id from vertex_table',array[0],2,false);
\echo --q2

SELECT * FROM pgr_contractGraph(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT id from vertex_table',array[0],2,true);
\echo --q3