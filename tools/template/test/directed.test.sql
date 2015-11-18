-- all this queries are equivlent (give the same results)

SELECT * FROM pgr_dijkstra(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table', 
    2, ARRAY[5, 3]);
SELECT * FROM pgr_funnyDijkstra(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table', 
    2, ARRAY[5, 3]);

