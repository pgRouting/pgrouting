
-- this is a hand made test
UPDATE edge_table SET id = id * 100, source = 1000 * source, target = 1000 * target;
UPDATE edge_table_vertices_pgr SET id = id * 1000;

DROP TABLE IF EXISTS result2;
SELECT  * INTO result2 FROM pgr_lineGraphFull(
    $$SELECT id, source, target, cost, reverse_cost
    FROM edge_table$$
);
SELECT * FROM result2;

DROP TABLE IF EXISTS result2_vertices_pgr;
WITH foo AS (SELECT source AS id FROM result2
    UNION
    SELECT target FROM result2) 
SELECT *, NULL::BIGINT AS original_id
INTO result2_vertices_pgr
FROM foo
ORDER BY id;

SELECT * FROM result2_vertices_pgr;

UPDATE result2_vertices_pgr AS r SET original_id = v.id
FROM edge_table_vertices_pgr AS v WHERE v.id = r.id;

WITH a AS (SELECT e.id, e.original_id FROM result2_vertices_pgr AS e WHERE original_id IS NOT NULL),
b AS (SELECT * FROM result2 WHERE cost = 0 and source IN (SELECT id FROM a)),
c AS (SELECT * FROM b JOIN result2_vertices_pgr ON(source = id)),
d AS (SELECT c.source, v.original_id FROM c JOIN result2_vertices_pgr as v ON (target=v.id)),
e AS (SELECT DISTINCT c.target, c.original_id FROM c JOIN result2_vertices_pgr AS r ON(target = r.id AND r.original_id IS NULL))
UPDATE result2_vertices_pgr SET original_id = e.original_id FROM e WHERE e.target = id;

WITH a AS (SELECT e.id, e.original_id FROM result2_vertices_pgr AS e WHERE original_id IS NOT NULL),
b AS (SELECT * FROM result2 WHERE cost = 0 and target IN (SELECT id FROM a)),
c AS (SELECT * FROM b JOIN result2_vertices_pgr ON(target = id)),
d AS (SELECT c.target, v.original_id FROM c JOIN result2_vertices_pgr as v ON (source=v.id)),
e AS (SELECT DISTINCT c.source, c.original_id FROM c JOIN result2_vertices_pgr AS r ON(source = r.id AND r.original_id IS NULL))
UPDATE result2_vertices_pgr SET original_id = e.original_id FROM e WHERE e.source = id;

WITH a AS (SELECT id FROM result2_vertices_pgr WHERE original_id IS NULL),
b AS (SELECT source,edge FROM result2 WHERE source IN (SELECT id FROM a)),
c AS (SELECT id,source FROM edge_table WHERE id IN (SELECT edge FROM b))
UPDATE result2_vertices_pgr AS d SET original_id = (SELECT source FROM c WHERE c.id = (SELECT edge FROM b WHERE b.source = d.id)) WHERE id IN (SELECT id FROM a);

WITH a AS (SELECT id FROM result2_vertices_pgr WHERE original_id IS NULL),
b AS (SELECT target,edge FROM result2 WHERE target IN (SELECT id FROM a)),
c AS (SELECT id,target FROM edge_table WHERE id IN (SELECT edge FROM b))
UPDATE result2_vertices_pgr AS d SET original_id = (SELECT target FROM c WHERE c.id = (SELECT edge FROM b WHERE b.target = d.id)) WHERE id IN (SELECT id FROM a);

SELECT * FROM result2_vertices_pgr;

ALTER TABLE result2 ADD COLUMN original_source_vertex BIGINT;
ALTER TABLE result2 ADD COLUMN original_target_vertex BIGINT;
ALTER TABLE result2 ADD COLUMN original_source_edge BIGINT;
ALTER TABLE result2 ADD COLUMN original_target_edge BIGINT;

-- restoring the original_foo_vertex 
UPDATE result2 AS edges SET original_source_vertex = vertices.original_id
FROM result2_vertices_pgr AS vertices WHERE edges.source = vertices.id;

UPDATE result2 AS edges SET original_target_vertex = vertices.original_id
FROM result2_vertices_pgr AS vertices WHERE edges.target = vertices.id;

-- restoring the original_foo_edges 
UPDATE result2
SET original_source_edge = edge,
    original_target_edge = edge
WHERE edge != 0;

UPDATE  result2 AS a
SET original_source_edge = b.edge
FROM result2 AS b
WHERE 
    a.original_source_edge IS NULL AND
    b.original_source_edge IS NOT NULL AND
    a.source = b.target;

UPDATE  result2 AS a
SET original_target_edge = b.edge
FROM result2 AS b
WHERE 
    a.original_target_edge IS NULL AND
    b.original_target_edge IS NOT NULL AND
    a.target = b.source;

-- all the results
DROP TABLE IF EXISTS q_result_1;
SELECT * INTO q_result_1
FROM pgr_dijkstra($$SELECT seq AS id, * FROM result2$$,
    (SELECT array_agg(id) FROM result2_vertices_pgr where original_id = 2000),
    (SELECT array_agg(id) FROM result2_vertices_pgr where original_id = 3000)
);
SELECT * FROM q_result_1;
-- choosing the best many to many
SELECT min(agg_cost) FROM q_result_1 WHERE edge = -1;
SELECT path_seq, start_vid, end_vid FROM q_result_1 WHERE edge = -1 and agg_cost = 5;
SELECT * FROM q_result_1 WHERE start_vid = -3 AND end_vid = 3000;
-- not showing the 0 cost edges
SELECT * FROM q_result_1 WHERE start_vid = -3 AND end_vid = 3000 AND (cost != 0 OR edge = -1);
-- not showing the 0 cost edges and showing the original edges
WITH a AS (
    SELECT * FROM q_result_1
    WHERE start_vid = -3 AND end_vid = 3000 AND (cost != 0 OR edge = -1))
SELECT
    a.seq, a.path_seq,
    (SELECT original_id FROM result2_vertices_pgr AS b WHERE a.start_vid = b.id) AS start_vid,
    (SELECT original_id FROM result2_vertices_pgr AS b WHERE a.end_vid = b.id) AS end_vid,
    (SELECT original_id FROM result2_vertices_pgr AS b WHERE a.node = b.id) AS node,
    edge, cost, agg_cost
FROM a;

-- A Dijkstra
SELECT * from pgr_dijkstra($$SELECT id, * FROM edge_table$$,
    ARRAY[2000], ARRAY[3000]);
/*
-- all the results
SELECT seq, path_seq, 
    (SELECT original_id FROM result2_vertices_pgr WHERE start_vid = id) AS start_vid, 
    (SELECT original_id FROM result2_vertices_pgr WHERE end_vid = id) AS end_vid,
    (SELECT original_id FROM result2_vertices_pgr WHERE node = id) AS node,
    coalesce((SELECT r.edge FROM result2 AS r WHERE q.edge = r.seq), -1) AS edge, cost, agg_cost
FROM q_result_1 AS q;

-- only from 4 to 10
SELECT seq, path_seq, 
    (SELECT original_id FROM result2_vertices_pgr WHERE start_vid = id) AS start_vid, 
    (SELECT original_id FROM result2_vertices_pgr WHERE end_vid = id) AS end_vid,
    (SELECT original_id FROM result2_vertices_pgr WHERE node = id) AS node,
    coalesce((SELECT abs(r.edge) FROM result2 AS r WHERE q.edge = r.seq), -1) AS edge, cost, agg_cost
FROM q_result_1 AS q
WHERE start_vid = 4 AND end_vid = 10;

-- only from 4 to 10 eliminating 0 cost rows
SELECT seq, path_seq, 
    (SELECT original_id FROM result2_vertices_pgr WHERE start_vid = id) AS start_vid, 
    (SELECT original_id FROM result2_vertices_pgr WHERE end_vid = id) AS end_vid,
    (SELECT original_id FROM result2_vertices_pgr WHERE node = id) AS node,
    coalesce((SELECT abs(r.edge) FROM result2 AS r WHERE q.edge = r.seq), -1) AS edge, cost, agg_cost
FROM q_result_1 AS q
WHERE start_vid = 4 AND end_vid = 10 AND (
    cost != 0 OR edge = -1 OR path_seq = 1);



-- original result
SELECT * from pgr_dijkstra($$SELECT id, * FROM edge_table$$,
    ARRAY[2000]::BIGINT[], ARRAY[3000]::BIGINT[]);

-- only from 4 to 10 eliminating 0 cost rows
SELECT seq, path_seq, 
    (SELECT original_id FROM result2_vertices_pgr WHERE node = id) AS node,
    coalesce((SELECT abs(r.edge) FROM result2 AS r WHERE q.edge = r.seq), -1) AS edge, cost, agg_cost
FROM q_result_1 AS q
WHERE start_vid = 4 AND end_vid = 10 AND (
    cost != 0 OR edge = -1 OR path_seq = 1);

-- original result
SELECT * from pgr_dijkstra($$SELECT id, * FROM edge_table$$,
    2000, 3000);


------------------------------------------------------------------------------------------------------

-- updating the original result due to the fact that it needs original vertices numbering vertex
UPDATE result1
SET source = result2.source,
    target = result2.target
FROM result2
WHERE result1.seq = result2.seq;

SELECT * FROM result1 JOIN result2 USING (seq, source, target);

-- Objective: generte in result2 the columns that were removed from result1
-- restrictions: using only data from the following tables
-- edge_table & edge_table_vertices_pgr
-- result2 & result2_vertices_pgr tables

-- comparing with original result
SELECT * FROM result1 JOIN result2
USING (seq, source, target, cost,
    original_source_vertex, original_target_vertex,
    original_source_edge, original_target_edge
);


*/




