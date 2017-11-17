
-- this is a hand made test
UPDATE edge_table SET id = id * 100, source = 1000 * source, target = 1000 * target;
UPDATE edge_table_vertices_pgr SET id = id * 1000;

DROP TABLE IF EXISTS result1;
SELECT  *, 0 AS edge  INTO result1 FROM pgr_turnPenaltyGraph(
    $$SELECT id, source, target, cost, reverse_cost
    FROM edge_table$$
);
SELECT * FROM result1;

DROP TABLE IF EXISTS temp1;
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
    ELSE r2.edge END AS edge,
    r1.original_source_vertex, r1.original_source_edge,
    r1.original_target_vertex, r1.original_target_edge
INTO temp1
FROM r2 JOIN result1 r1 USING(seq) ORDER by seq;
SELECT * FROM temp1;

-- result2_vertices_pgr  == to v_table
-- This is the v_table 
DROP TABLE IF EXISTS v_table;
WITH a AS (
    SELECT source AS id, original_source_vertex AS original_id FROM temp1
    UNION
    SELECT target, original_target_vertex FROM temp1)
SELECT *
INTO v_table
FROM a
ORDER BY id;

DROP TABLE IF EXISTS result2;
SELECT seq, source, target, cost, edge INTO result2 FROM temp1;

-- turn penalties
UPDATE result2 SET cost = 100 WHERE source IN (SELECT target FROM result2 WHERE edge = 900) AND target IN (SELECT source FROM result2 WHERE edge = -1600);

DROP TABLE IF EXISTS result2_vertices_pgr;
WITH foo AS (SELECT source AS id FROM result2
    UNION
    SELECT target FROM result2) 
SELECT *, NULL::BIGINT AS original_id
INTO result2_vertices_pgr
FROM foo
ORDER BY id;

SELECT * from v_table JOIN result2_vertices_pgr USING (id);

UPDATE result2_vertices_pgr AS r SET original_id = v.id
FROM edge_table_vertices_pgr AS v WHERE v.id = r.id;

WITH a AS (SELECT e.id, e.original_id FROM result2_vertices_pgr AS e WHERE original_id IS NOT NULL),
b AS (SELECT * FROM result2 WHERE cost = 0 and target IN (SELECT id FROM a)),
c AS (SELECT * FROM b JOIN result2_vertices_pgr ON(target = id)),
d AS (SELECT c.target, v.original_id FROM c JOIN result2_vertices_pgr as v ON (source=v.id)),
e AS (SELECT DISTINCT c.source, c.original_id FROM c JOIN result2_vertices_pgr AS r ON(source = r.id AND r.original_id IS NULL))
UPDATE result2_vertices_pgr SET original_id = e.original_id FROM e WHERE e.source = id;

WITH a AS (SELECT e.id, e.original_id FROM result2_vertices_pgr AS e WHERE original_id IS NOT NULL),
b AS (SELECT * FROM result2 WHERE cost = 0 and source IN (SELECT id FROM a)),
c AS (SELECT * FROM b JOIN result2_vertices_pgr ON(source = id)),
d AS (SELECT c.source, v.original_id FROM c JOIN result2_vertices_pgr as v ON (target=v.id)),
e AS (SELECT DISTINCT c.target, c.original_id FROM c JOIN result2_vertices_pgr AS r ON(target = r.id AND r.original_id IS NULL))
UPDATE result2_vertices_pgr SET original_id = e.original_id FROM e WHERE e.target = id;




-- original result
SELECT * from pgr_dijkstra($$SELECT id, * FROM edge_table$$,
    2000, 3000);

DROP TABLE IF EXISTS q_result_1;
SELECT * INTO q_result_1
FROM pgr_dijkstra($$SELECT seq AS id, * FROM result2$$,
    (SELECT array_agg(id) FROM result2_vertices_pgr where original_id = 2000),
    (SELECT array_agg(id) FROM result2_vertices_pgr where original_id = 3000)
);
SELECT * FROM q_result_1;

