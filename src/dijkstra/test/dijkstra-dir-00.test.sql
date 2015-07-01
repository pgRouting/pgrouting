-- all this queries are equivlent (give the same results)

-- include reverse_costs
-- convert -1 costs to a large number
-- ignores -1 costs
-- directed = true (DIRECTED)
-- autodetect reverse_cost

select * from pgr_dijkstra(
    'select id as id, source::int4, target::int4, 
         case when cost<=0 then 999 else cost end as cost, 
         case when reverse_cost<=0 then 999 else reverse_cost end as reverse_cost 
       from edges1', 
    11, 5, true, true);

select * from pgr_dijkstra(
    'select id as id, source::int4, target::int4, 
         case when cost<=0 then 999 else cost end as cost, 
         case when reverse_cost<=0 then 999 else reverse_cost end as reverse_cost 
       from edges1', 
    11, 5, true);


select * from pgr_dijkstra(
    'select id, source, target, 
         case when cost<=0 then 999 else cost end as cost, 
         case when reverse_cost<=0 then 999 else reverse_cost end as reverse_cost 
       from edges1', 
    11, 5);

select * from pgr_dijkstra(
    'select id, source, target, 
         cost, 
         reverse_cost 
       from edges1', 
    11, 5);


select * from pgr_dijkstra(
    'select * 
       from edges1', 
    11, 5);

