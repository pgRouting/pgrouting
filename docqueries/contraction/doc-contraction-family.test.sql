-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/

/* -- q00 */
SELECT id, source, target, cost, reverse_cost
FROM edges ORDER BY id;
/* -- q01 */
/* -- q1 */
ALTER TABLE vertices ADD is_contracted BOOLEAN DEFAULT false;
ALTER TABLE vertices ADD contracted_vertices BIGINT[];
ALTER TABLE edges ADD is_new BOOLEAN DEFAULT false;
ALTER TABLE edges ADD contracted_vertices BIGINT[];
/* -- q2 */
SELECT * FROM pgr_contraction(
  'SELECT id, source, target, cost, reverse_cost FROM edges',
  array[1, 2], directed => false);
/* -- q3 */
SELECT * INTO contraction_results
FROM pgr_contraction(
  'SELECT id, source, target, cost, reverse_cost FROM edges',
  array[1, 2], directed => false);
/* -- q4 */
UPDATE vertices
SET is_contracted = true
WHERE id IN (SELECT  unnest(contracted_vertices) FROM  contraction_results);
/* -- q5 */
SELECT id, is_contracted
FROM vertices
ORDER BY id;
/* -- q6 */
UPDATE vertices
SET contracted_vertices = contraction_results.contracted_vertices
FROM contraction_results
WHERE type = 'v' AND vertices.id = contraction_results.id;
/* -- q7 */
SELECT id, contracted_vertices, is_contracted
FROM vertices
ORDER BY id;
/* -- q8 */
INSERT INTO edges(source, target, cost, reverse_cost, contracted_vertices, is_new)
SELECT source, target, cost, -1, contracted_vertices, true
FROM contraction_results
WHERE type = 'e';
/* -- q9 */
SELECT id, source, target, cost, reverse_cost, contracted_vertices, is_new
FROM edges
ORDER BY id;
/* -- q10 */
SELECT id
FROM vertices
WHERE is_contracted = false
ORDER BY id;
/* -- q11 */
WITH
vertices_in_graph AS (
  SELECT id
  FROM vertices
  WHERE is_contracted = false
)
SELECT id, source, target, cost, reverse_cost, contracted_vertices
FROM edges
WHERE source IN (SELECT * FROM vertices_in_graph)
AND target IN (SELECT * FROM vertices_in_graph)
ORDER BY id;
/* -- case1 */
CREATE OR REPLACE FUNCTION my_dijkstra(
  departure BIGINT, destination BIGINT,
  OUT seq INTEGER, OUT path_seq INTEGER,
  OUT start_vid BIGINT, OUT end_vid BIGINT,
  OUT node BIGINT, OUT edge BIGINT,
  OUT cost FLOAT, OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
SELECT * FROM pgr_dijkstra(
  $$
  WITH
  vertices_in_graph AS (
    SELECT id
    FROM vertices
    WHERE is_contracted = false
  )
  SELECT id, source, target, cost, reverse_cost
  FROM edges
  WHERE source IN (SELECT * FROM vertices_in_graph)
  AND target IN (SELECT * FROM vertices_in_graph)
  $$,
  departure, destination, false);
$BODY$
LANGUAGE SQL VOLATILE;
/* -- use1 */
SELECT * FROM my_dijkstra(10, 12);
/* -- use1-1 */
SELECT * FROM my_dijkstra(15, 12);
/* -- use1-2 */
SELECT * FROM my_dijkstra(15, 1);
/* -- case2 */
CREATE OR REPLACE FUNCTION my_dijkstra(
  departure BIGINT, destination BIGINT,
  OUT seq INTEGER, OUT path_seq INTEGER,
  OUT start_vid BIGINT, OUT end_vid BIGINT,
  OUT node BIGINT, OUT edge BIGINT,
  OUT cost FLOAT, OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
SELECT * FROM pgr_dijkstra(
  $$
  WITH
  edges_to_expand AS (
    SELECT id
    FROM edges
    WHERE ARRAY[$$ || departure || $$]::BIGINT[] <@ contracted_vertices
    OR ARRAY[$$ || destination || $$]::BIGINT[] <@ contracted_vertices
  ),

  vertices_in_graph AS (
    SELECT id
    FROM vertices
    WHERE is_contracted = false

    UNION

    SELECT unnest(contracted_vertices)
    FROM edges
    WHERE id IN (SELECT id FROM edges_to_expand)
  )

  SELECT id, source, target, cost, reverse_cost
  FROM edges
  WHERE source IN (SELECT * FROM vertices_in_graph)
  AND target IN (SELECT * FROM vertices_in_graph)
  $$,
  departure, destination, false);
$BODY$
LANGUAGE SQL VOLATILE;
/* -- use2 */
SELECT * FROM my_dijkstra(10, 12);
/* -- use2-1 */
SELECT * FROM my_dijkstra(15, 12);
/* -- use2-2 */
SELECT * FROM my_dijkstra(15, 1);
/* -- case3 */
CREATE OR REPLACE FUNCTION my_dijkstra(
  departure BIGINT, destination BIGINT,
  OUT seq INTEGER, OUT path_seq INTEGER,
  OUT start_vid BIGINT, OUT end_vid BIGINT,
  OUT node BIGINT, OUT edge BIGINT,
  OUT cost FLOAT, OUT agg_cost FLOAT)
RETURNS SETOF RECORD AS
$BODY$
SELECT * FROM pgr_dijkstra(
  $$
  WITH
  edges_to_expand AS (
    SELECT id
    FROM edges
    WHERE ARRAY[$$ || departure || $$]::BIGINT[] <@ contracted_vertices
    OR ARRAY[$$ || destination || $$]::BIGINT[] <@ contracted_vertices
  ),

  vertices_to_expand AS (
    SELECT id
    FROM vertices
    WHERE ARRAY[$$ || departure || $$]::BIGINT[] <@ contracted_vertices
    OR ARRAY[$$ || destination || $$]::BIGINT[] <@ contracted_vertices
  ),

  vertices_in_graph AS (
    SELECT id
    FROM vertices
    WHERE is_contracted = false

    UNION

    SELECT unnest(contracted_vertices)
    FROM edges
    WHERE id IN (SELECT id FROM edges_to_expand)

    UNION

    SELECT unnest(contracted_vertices)
    FROM vertices
    WHERE id IN (SELECT id FROM vertices_to_expand)
  )

  SELECT id, source, target, cost, reverse_cost
  FROM edges
  WHERE source IN (SELECT * FROM vertices_in_graph)
  AND target IN (SELECT * FROM vertices_in_graph)
  $$,
  departure, destination, false);
$BODY$
LANGUAGE SQL VOLATILE;
/* -- use3 */
SELECT * FROM my_dijkstra(10, 12);
/* -- use3-1 */
SELECT * FROM my_dijkstra(15, 12);
/* -- use3-2 */
SELECT * FROM my_dijkstra(15, 1);
/* -- end */
