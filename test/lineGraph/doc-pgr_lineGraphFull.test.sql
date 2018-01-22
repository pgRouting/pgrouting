
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
unassignedVertices
  AS (SELECT e.id, e.original_id
       FROM lineGraph_vertices AS e
       WHERE original_id IS NOT NULL),
edgesWithUnassignedSource
  AS (SELECT *
        FROM lineGraph_edges
        WHERE cost = 0 and source IN (SELECT id FROM unassignedVertices)),
edgesWithUnassignedSourcePlusVertices
  AS (SELECT *
        FROM edgesWithUnassignedSource
        JOIN lineGraph_vertices
        ON(source = id)),
verticesFromEdgesWithUnassignedSource
  AS (SELECT DISTINCT edgesWithUnassignedSourcePlusVertices.target, 
                      edgesWithUnassignedSourcePlusVertices.original_id
        FROM edgesWithUnassignedSourcePlusVertices
        JOIN lineGraph_vertices AS r
        ON(target = r.id AND r.original_id IS NULL))
UPDATE lineGraph_vertices
  SET original_id = verticesFromEdgesWithUnassignedSource.original_id
  FROM verticesFromEdgesWithUnassignedSource
  WHERE verticesFromEdgesWithUnassignedSource.target = id;

WITH
unassignedVertices
  AS (SELECT e.id, e.original_id
        FROM lineGraph_vertices AS e
        WHERE original_id IS NOT NULL),
edgesWithUnassignedTarget
  AS (SELECT *
        FROM lineGraph_edges
        WHERE cost = 0 and target IN (SELECT id FROM unassignedVertices)),
edgesWithUnassignedTargetPlusVertices
  AS (SELECT *
        FROM edgesWithUnassignedTarget
        JOIN lineGraph_vertices
        ON(target = id)),
verticesFromEdgesWithUnassignedTarget
  AS (SELECT DISTINCT edgesWithUnassignedTargetPlusVertices.source, 
                     edgesWithUnassignedTargetPlusVertices.original_id
        FROM edgesWithUnassignedTargetPlusVertices
        JOIN lineGraph_vertices AS r
        ON(source = r.id AND r.original_id IS NULL))
UPDATE lineGraph_vertices
  SET original_id = verticesFromEdgesWithUnassignedTarget.original_id
  FROM verticesFromEdgesWithUnassignedTarget
  WHERE verticesFromEdgesWithUnassignedTarget.source = id;

\echo -- q5
WITH
unassignedVertexIds
  AS (SELECT id
        FROM lineGraph_vertices
        WHERE original_id IS NULL),
edgesWithUnassignedSource
  AS (SELECT source,edge
        FROM lineGraph_edges
        WHERE source IN (SELECT id FROM unassignedVertexIds)),
originalEdgesWithUnassignedSource
  AS (SELECT id,source
        FROM edge_table
        WHERE id IN (SELECT edge FROM edgesWithUnassignedSource))
UPDATE lineGraph_vertices AS d
  SET original_id = (SELECT source
                       FROM originalEdgesWithUnassignedSource
                       WHERE originalEdgesWithUnassignedSource.id = 
                         (SELECT edge
                            FROM edgesWithUnassignedSource
                            WHERE edgesWithUnassignedSource.source = d.id))
  WHERE id IN (SELECT id FROM unassignedVertexIds);

WITH
unassignedVertexIds
  AS (SELECT id
        FROM lineGraph_vertices
        WHERE original_id IS NULL),
edgesWithUnassignedTarget
  AS (SELECT target,edge
        FROM lineGraph_edges
        WHERE target IN (SELECT id FROM unassignedVertexIds)),
originalEdgesWithUnassignedTarget
  AS (SELECT id,target
        FROM edge_table
        WHERE id IN (SELECT edge FROM edgesWithUnassignedTarget))
UPDATE lineGraph_vertices AS d
  SET original_id = (SELECT target
                       FROM originalEdgesWithUnassignedTarget
                       WHERE originalEdgesWithUnassignedTarget.id = 
                         (SELECT edge
                            FROM edgesWithUnassignedTarget
                            WHERE edgesWithUnassignedTarget.target = d.id))
  WHERE id IN (SELECT id FROM unassignedVertexIds);

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
    )) as shortestPaths
  WHERE start_vid = 2 AND end_vid = 8 AND (cost != 0 OR edge = -1)) as b;

\echo -- q9

