
\qecho -- 0 edges tests

select id, source, target, cost, reverse_cost  from edge_table where id>18;

\qecho -- directed graph
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id>18 ',3, 2);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id>18 ',array[3], 2);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id>18 ',array[3], array[2]);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id>18 ',3, array[2]);

\qecho -- undirected graph
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id>18 ',3, 2, false);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id>18 ',array[3], 2, false);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id>18 ',array[3], array[2], false);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id>18 ',3, array[2], false);


\qecho -- 1 edges tests

\qecho -- edge doesnt have source
select id, source, target, cost, reverse_cost  from edge_table where id = 4;

\qecho -- directed graph
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 4 ',3, 2);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 4 ',array[3], 2);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 4 ',array[3], array[2]);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 4 ',3, array[2]);

\qecho -- undirected graph
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 4 ',3, 2, false);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 4 ',array[3], 2, false);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 4 ',array[3], array[2], false);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 4 ',3, array[2], false);

\qecho -- edge doesnt have target
select id, source, target, cost, reverse_cost  from edge_table where id = 3;

\qecho -- directed graph
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 3 ',3, 2);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 3 ',array[3], 2);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 3 ',array[3], array[2]);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 3 ',3, array[2]);

\qecho -- undirected graph
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 3 ',3, 2, false);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 3 ',array[3], 2, false);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 3 ',array[3], array[2], false);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 3 ',3, array[2], false);

\qecho -- edge has both
select id, source, target, cost, reverse_cost  from edge_table where id = 3;

\qecho -- directed graph
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 2 ',3, 2);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 2 ',array[3], 2);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 2 ',array[3], array[2]);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 2 ',3, array[2]);

\qecho -- undirected graph
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 2 ',3, 2, false);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 2 ',array[3], 2, false);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 2 ',array[3], array[2], false);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 2 ',3, array[2], false);

\qecho -- edge has both
select id, source, target, cost, reverse_cost  from edge_table where id = 3;

\qecho -- directed graph
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 2 ',2, 3);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 2 ',array[2], 3);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 2 ',array[2], array[3]);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 2 ',2, array[3]);

\qecho -- undirected graph

select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 2 ',2, 3, false);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 2 ',array[2], 3, false);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 2 ',array[2], array[3], false);
select * from pgr_dijkstra('select id, source, target, cost, reverse_cost  from edge_table where id = 2 ',2, array[3], false);
