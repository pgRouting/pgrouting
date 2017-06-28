\echo -- q2
WITH
a AS (SELECT * FROM pgr_dijkstra(
  'SELECT * FROM edge_table', ARRAY[1,4,7,13],  ARRAY[1,4,7,13])
  WHERE edge<>-1) SELECT edge,node AS source,cost FROM a GROUP BY edge,source,cost;

\echo -- q3
WITH
a AS (SELECT * from pgr_dijkstra(
          'SELECT * FROM edge_table', ARRAY[1,4,7,13],  ARRAY[1,4,7,13]) where edge<>-1),
b AS (SELECT edge as id,node as source,cost from a group by id,source,cost),
c AS (SELECT id, source, target, cost, reverse_cost FROM edge_table),
d AS (SELECT b.id,b.source,target,c.cost,reverse_cost from b join c on b.id=c.id)
select * from d;
