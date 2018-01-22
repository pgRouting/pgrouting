
\echo -- q1
SELECT * FROM pgr_lineGraphFull(
    'SELECT id, source, target, cost, reverse_cost
      FROM edge_table
      WHERE id IN (4,7,8,10)'
);

\echo -- q2
CREATE TABLE lineGraph_edges AS SELECT * FROM pgr_lineGraphFull(
    $$SELECT id, source, target, cost, reverse_cost
    FROM edge_table WHERE id IN (4,7,8,10)$$
);

CREATE TABLE lineGraph_vertices AS
SELECT *, NULL::BIGINT AS original_id
FROM (SELECT source AS id FROM lineGraph_edges
    UNION
    SELECT target FROM lineGraph_edges) as foo
ORDER BY id;

\echo -- q3
UPDATE lineGraph_vertices AS r
  SET original_id = v.id
  FROM edge_table_vertices_pgr AS v
  WHERE v.id = r.id;

\echo -- q4
WITH
a AS (SELECT e.id, e.original_id
       FROM lineGraph_vertices AS e
       WHERE original_id IS NOT NULL),
b AS (SELECT *
        FROM lineGraph_edges
        WHERE cost = 0 and source IN (SELECT id FROM a)),
c AS (SELECT *
        FROM b
        JOIN lineGraph_vertices
        ON(source = id)),
d AS (SELECT c.source, v.original_id
        FROM c
        JOIN lineGraph_vertices as v
        ON (target=v.id)),
e AS (SELECT DISTINCT c.target, c.original_id
        FROM c
        JOIN lineGraph_vertices AS r
        ON(target = r.id AND r.original_id IS NULL))
UPDATE lineGraph_vertices
  SET original_id = e.original_id
  FROM e
  WHERE e.target = id;

WITH
a AS (SELECT e.id, e.original_id
        FROM lineGraph_vertices AS e
        WHERE original_id IS NOT NULL),
b AS (SELECT *
        FROM lineGraph_edges
        WHERE cost = 0 and target IN (SELECT id FROM a)),
c AS (SELECT *
        FROM b
        JOIN lineGraph_vertices
        ON(target = id)),
d AS (SELECT c.target, v.original_id
        FROM c
        JOIN lineGraph_vertices as v
        ON (source=v.id)),
e AS (SELECT DISTINCT c.source, c.original_id
        FROM c
        JOIN lineGraph_vertices AS r
        ON(source = r.id AND r.original_id IS NULL))
UPDATE lineGraph_vertices
  SET original_id = e.original_id
  FROM e
  WHERE e.source = id;

\echo -- q5
WITH
a AS (SELECT id
        FROM lineGraph_vertices
        WHERE original_id IS NULL),
b AS (SELECT source,edge
        FROM lineGraph_edges
        WHERE source IN (SELECT id FROM a)),
c AS (SELECT id,source
        FROM edge_table
        WHERE id IN (SELECT edge FROM b))
UPDATE lineGraph_vertices AS d
  SET original_id = (SELECT source
                       FROM c
                       WHERE c.id = (SELECT edge
                                       FROM b
                                       WHERE b.source = d.id))
  WHERE id IN (SELECT id FROM a);

WITH
a AS (SELECT id
        FROM lineGraph_vertices
        WHERE original_id IS NULL),
b AS (SELECT target,edge
        FROM lineGraph_edges
        WHERE target IN (SELECT id FROM a)),
c AS (SELECT id,target
        FROM edge_table
        WHERE id IN (SELECT edge FROM b))
UPDATE lineGraph_vertices AS d
  SET original_id = (SELECT target
                       FROM c
                       WHERE c.id = (SELECT edge
                                       FROM b
                                       WHERE b.target = d.id))
  WHERE id IN (SELECT id FROM a);

\echo -- q6
SELECT * FROM lineGraph_vertices;

\echo -- q7
UPDATE lineGraph_edges
  SET cost = 100
  WHERE source IN (SELECT target
                     FROM lineGraph_edges
                     WHERE edge = 4) AND target IN (SELECT source
                                                      FROM lineGraph_edges
                                                      WHERE edge = -7);

\echo -- q8
SELECT * FROM
  (SELECT * FROM
    (SELECT * FROM pgr_dijkstra($$SELECT seq AS id, * FROM lineGraph_edges$$,
      (SELECT array_agg(id) FROM lineGraph_vertices where original_id = 2),
      (SELECT array_agg(id) FROM lineGraph_vertices where original_id = 8)
    )) as a
  WHERE start_vid = 2 AND end_vid = 8 AND (cost != 0 OR edge = -1)) as b;

\echo -- q9

