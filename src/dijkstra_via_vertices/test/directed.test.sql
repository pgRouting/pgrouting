
select * from pgr_dijkstra(
    'select id, source, target, cost, reverse_cost from edge_table', 
    1, 11, true);

select * from pgr_dijkstra(
    'select id, source, target, cost, reverse_cost from edge_table', 
    11, 4, true);

select * from pgr_dijkstraViaVertices(
    'select id, source, target, cost, reverse_cost from edge_table', 
    array[1,11,4], true);

select * from pgr_dijkstraViaVertices(
    'select id, source, target, cost, reverse_cost from edge_table', 
    array[1,11,4, 5, 7]);

select * from pgr_dijkstraViaVertices(
    'select id, source, target, cost, reverse_cost from edge_table', 
    array[1,6, 7, 11,4]);

select * from pgr_dijkstraViaVertices(
    'select id, source, target, cost, reverse_cost from edge_table', 
    array[1, 8, 10, 11, 4]);

select * from pgr_dijkstraViaVertices(
    'select id, source, target, cost, reverse_cost from edge_table', 
    array[1,3]);

