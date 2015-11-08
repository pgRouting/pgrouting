-- test for issue 353

select * from pgr_dijkstra( 'select id, source, target, cost from edge_table',
     array[2,7], array[5,6]);
select * from pgr_dijkstra( 'select id, source, target, cost from edge_table', 
    array[2,7], array[17,18]);
select * from pgr_dijkstra( 'select id, source, target, cost from edge_table', 
array[2,7], array[5,6,17,18]);
