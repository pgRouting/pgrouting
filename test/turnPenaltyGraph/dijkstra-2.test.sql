
-- this is a hand made test
UPDATE edge_table SET id = id * 100, source = 1000 * source, target = 1000 * target;
UPDATE edge_table_vertices_pgr SET id = id * 1000;

SELECT  *, 0 AS edge  INTO result1 FROM pgr_turnPenaltyGraph(
    $$SELECT id, source, target, cost, reverse_cost
    FROM edge_table$$
);
SELECT * FROM result1;


WITH
rel AS (
    SELECT source, original_source_vertex  FROM result1
    UNION   
    SELECT target, original_target_vertex  FROM result1
),          
unique_rel AS (
    SELECT original_source_vertex, min(source) AS source 
    FROM rel            
    GROUP BY original_source_vertex
),
new_numbers AS (
    SELECT r.seq, u.original_source_vertex AS source, r.target, r.cost, r.edge FROM unique_rel AS u, result1 AS r WHERE u.source = r.source
    UNION               
    SELECT r.seq, r.source, u.original_source_vertex AS target, r.cost, r.edge FROM unique_rel AS u, result1 AS r WHERE u.source = r.target
),
r2 AS (
    SELECT * FROM new_numbers
    UNION
    SELECT r.seq, r.source, r.target, r.cost, r.edge FROM result1 AS r WHERE r.seq NOT IN (SELECT new_numbers.seq AS id FROM new_numbers)
)
SELECT r2.seq, r2.source, r2.target, r2.cost,
    CASE WHEN original_source_edge = original_target_edge THEN original_source_edge
    ELSE r2.edge END AS edge
INTO result2
FROM r2 JOIN result1 USING(seq) ORDER by seq;


-- original result
SELECT * from pgr_dijkstra($$SELECT id, * FROM edge_table$$,
    2000, 3000);
-- showing the 0 cost edges
SELECT * from pgr_dijkstra($$SELECT seq AS id, * FROM result2$$,
    2000, 3000);

-- not showing the 0 cost edges
SELECT * from pgr_dijkstra($$SELECT seq AS id, * FROM result2$$,
    2000, 3000) WHERE cost != 0 OR edge = -1 ORDER BY seq;

-- want the original result
SELECT * from pgr_dijkstra($$SELECT seq AS id, * FROM result2$$,
    1000, 2000) WHERE cost != 0 OR edge = -1 ORDER BY seq;
