-- all this queries are equivlent (give the same results)

-- DOES NOT include reverse_costs
-- convert -1 costs to a large number
-- ignores -1 costs
-- directed = false  (UNDIRECTED)
-- autodetect reverse_cost

-- no path found

select * from pgr_dijkstra(
    'select id as id, source::int4, target::int4, 
         case when cost<=0 then 999 else cost end as cost 
       from edges1', 
    11, 5, true, false);

select * from pgr_dijkstra(
    'select id as id, source::int4, target::int4, 
         case when cost<=0 then 999 else cost end as cost 
       from edges1', 
    11, 5, true);


select * from pgr_dijkstra(
    'select id, source, target, 
         case when cost<=0 then 999 else cost end as cost 
       from edges1', 
    11, 5);

select * from pgr_dijkstra(
    'select id, source, target, 
         cost
       from edges1', 
    11, 5);

