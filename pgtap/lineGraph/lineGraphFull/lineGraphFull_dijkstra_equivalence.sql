/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/

\i setup.sql

SELECT plan(1);

DROP TABLE IF EXISTS result2;
CREATE TABLE result2(
  seq integer,
  source bigint,
  target bigint,
  cost float,
  edge bigint);

DROP TABLE IF EXISTS result2_vertices_pgr;
CREATE TABLE result2_vertices_pgr(
  id bigint, 
  original_id bigint);

CREATE or REPLACE FUNCTION lineGraphFullDijkstraEquivalence(cant INTEGER default 17)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
turnpenalty_sql TEXT;
original_sql TEXT;
BEGIN

  INSERT INTO result2 SELECT * FROM pgr_lineGraphFull(
      $$SELECT id, source, target, cost, reverse_cost
      FROM edge_table$$
  );

  WITH foo AS (SELECT source AS id FROM result2
      UNION
      SELECT target FROM result2) 
  INSERT INTO result2_vertices_pgr SELECT *, NULL::BIGINT AS original_id
  FROM foo
  ORDER BY id;
 
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

  ALTER TABLE result2 ADD COLUMN original_source_vertex BIGINT;
  ALTER TABLE result2 ADD COLUMN original_target_vertex BIGINT;
  ALTER TABLE result2 ADD COLUMN original_source_edge BIGINT;
  ALTER TABLE result2 ADD COLUMN original_target_edge BIGINT;

  UPDATE result2 AS edges SET original_source_vertex = vertices.original_id
  FROM result2_vertices_pgr AS vertices WHERE edges.source = vertices.id;

  UPDATE result2 AS edges SET original_target_vertex = vertices.original_id
  FROM result2_vertices_pgr AS vertices WHERE edges.target = vertices.id;

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

  turnpenalty_sql :=
  'WITH q_result_1 AS (SELECT * FROM pgr_dijkstra($$SELECT seq AS id, * FROM result2$$,
      (SELECT array_agg(id) FROM result2_vertices_pgr where original_id = 2),
      (SELECT array_agg(id) FROM result2_vertices_pgr where original_id = 3)
  )),
  a AS (
      SELECT * FROM q_result_1
      WHERE start_vid = -3 AND end_vid = 3 AND (cost != 0 OR edge = -1))
  SELECT agg_cost FROM a';

  original_sql :=
  'SELECT agg_cost from pgr_dijkstra($$SELECT id, * FROM edge_table$$, ARRAY[2], ARRAY[3])';

  RETURN query SELECT set_eq(original_sql, turnpenalty_sql, original_sql);

END
$BODY$
language plpgsql;

SELECT * from lineGraphFullDijkstraEquivalence();

