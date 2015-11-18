
SELECT * FROM pgr_dijkstraViaCompleteEdges(
    'SELECT id , source, target, cost, reverse_cost FROM edge_table',
    ARRAY[6,10,4]::bigint[], false);


