\echo -- expected result = empty set because edge 2 cost < 0
SELECT * FROM pgr_dijkstraViaEdges(
    'SELECT id , source, target, cost FROM edge_table ',
    ARRAY[2,2]::integer[], ARRAY[0.25,0.75]::float8[], true);  

\echo -- expected result = empty set because edge 2 cost < 0
SELECT * FROM pgr_dijkstraViaEdges(
    'SELECT id , source, target, cost FROM edge_table ',
    ARRAY[2,2]::integer[], ARRAY[0.25,0.75]::float8[], false);  


\echo -- expected total agg-cost = 6.5
\echo -- vertices: -1 2 5 6 9 4 3 -2
\echo -- edges: 2 4 8 9 16 3 2 -1
 SELECT * FROM pgr_dijkstraViaEdges(
    'SELECT id , source, target, cost, reverse_cost FROM edge_table ',
    ARRAY[2,2]::integer[], ARRAY[0.25,0.75]::float8[], true);  

\echo -- expected total agg-cost = 0.5
SELECT * FROM pgr_dijkstraViaEdges(
    'SELECT id , source, target, cost FROM edge_table ',
    ARRAY[6,6]::integer[], ARRAY[0.25,0.75]::float8[], false);  

\echo -- expected total agg-cost = 0.5
SELECT * FROM pgr_dijkstraViaEdges(
    'SELECT id , source, target, cost FROM edge_table ',
    ARRAY[6,6]::integer[], ARRAY[0.25,0.75]::float8[], true);  

\echo -- expected total agg-cost = 3 
SELECT * FROM pgr_dijkstraViaEdges(
    'SELECT id , source, target, cost, reverse_cost FROM edge_table',
    ARRAY[6,10,4]::integer[], ARRAY[0,0,0]::float8[], false);


