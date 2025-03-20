/*PGR-GNU*****************************************************************

Copyright (c) 2023  pgRouting developers
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

CREATE OR REPLACE PROCEDURE create_contracted_graph(directed BOOLEAN, fn TEXT, methods INTEGER[] DEFAULT NULL::INTEGER[]) AS
$BODY$

BEGIN

  IF NOT min_version('3.8.0') THEN RETURN; END IF;

  DELETE FROM edges WHERE id >= 19;
  ALTER SEQUENCE edges_id_seq RESTART WITH 19;
  UPDATE edges SET (is_new, contracted_vertices)=(false,NULL);
  UPDATE vertices SET (is_contracted, contracted_vertices)=(false,NULL);

  -- add extra columns to the edges and vertices table

  DROP TABLE IF EXISTS contraction_info;
  IF fn = 'pgr_contraction' THEN
    EXECUTE format($q$
    CREATE TABLE contraction_info AS
    SELECT * FROM %s(
      'SELECT id, source, target, cost, reverse_cost FROM edges ORDER BY id',
      %s, '%s'::INTEGER[]);
    $q$, fn, directed::TEXT, methods);
  ELSE
    EXECUTE format($q$
    CREATE TABLE contraction_info AS
    SELECT * FROM %s(
      'SELECT id, source, target, cost, reverse_cost FROM edges ORDER BY id',
      '%s');
    $q$, fn, directed::TEXT);
  END IF;

  -- add the new edges
  INSERT INTO edges(source, target, cost, reverse_cost, contracted_vertices, is_new)
  SELECT source, target, cost, -1, contracted_vertices, true
  FROM contraction_info
  WHERE type = 'e';

  -- Indicate vertices that were contracted
  UPDATE vertices
  SET is_contracted = true
  WHERE id IN (SELECT  unnest(contracted_vertices) FROM  contraction_info);

  -- add the contracted vertices on the vertices table
  UPDATE vertices
  SET contracted_vertices = contraction_info.contracted_vertices
  FROM contraction_info
  WHERE type = 'v' AND vertices.id = contraction_info.id;

  DROP VIEW IF EXISTS contracted_graph;
  CREATE VIEW contracted_graph AS
  SELECT id,source, target, cost, reverse_cost, contracted_vertices FROM edges
  WHERE
    EXISTS (SELECT id FROM vertices AS v WHERE NOT is_contracted AND v.id = edges.source)
    AND
    EXISTS (SELECT id FROM vertices AS v WHERE NOT is_contracted AND v.id = edges.target);

END
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION path_cost_contraction(BIGINT, BIGINT, BOOLEAN)
RETURNS SETOF TEXT AS
$BODY$
DECLARE q TEXT;
BEGIN

    RETURN QUERY
      SELECT round(agg_cost::numeric, 3)::TEXT FROM pgr_dijkstra(
      'WITH cul_de_sac AS (
        SELECT contracted_vertices || id as v
        FROM vertices WHERE ' || $1 ||' = ANY(contracted_vertices) OR ' || $2 ||' = ANY(contracted_vertices)),
      linears_to_expand AS (
        SELECT id, contracted_vertices
        FROM edges WHERE is_new AND (' || $1 ||' = ANY(contracted_vertices) OR '|| $2 ||'  = ANY(contracted_vertices))
      ),
      verts AS (SELECT * FROM cul_de_sac UNION SELECT contracted_vertices FROM linears_to_expand)
      SELECT id, source, target, cost, reverse_cost FROM edges, verts WHERE source = ANY(v) OR target = ANY(v)

      UNION

      SELECT id, source, target, cost, reverse_cost FROM contracted_graph LEFT JOIN linears_to_expand c USING (id) WHERE c.id IS NULL',
      $1, $2, $3) WHERE edge = -1;

END;
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION compare_dijkstra_contraction(
    BIGINT, BIGINT, BOOLEAN)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
    q TEXT;
BEGIN

  RETURN QUERY
  SELECT set_eq(
    $$SELECT * FROM path_cost_contraction($$|| $1 || $$,$$ || $2 || $$,$$ || $3::BOOLEAN || $$)$$,
    $$SELECT  round(agg_cost::numeric, 3)::TEXT
    FROM pgr_dijkstra(
      'SELECT id, source, target, cost, reverse_cost FROM edges WHERE id < 19',
      $$|| $1 || $$,$$ || $2 || $$,$$ || $3::BOOLEAN || $$) WHERE edge = -1
    $$,
    'From ' || $1 || ' to ' || $2 || 'Directed = ' || $3::BOOLEAN);

END
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION compare_dijkstra_contraction(BOOLEAN)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
    i INTEGER;
    j INTEGER;

BEGIN

  IF NOT min_version('3.8.0') THEN
    RETURN QUERY
    SELECT skip(1, 'contraction function/signature is new on 3.8.0');
    RETURN;
  END IF;

  FOR i IN 1..18 LOOP
    FOR j IN 1..18 LOOP
      RETURN QUERY
      SELECT compare_dijkstra_contraction(i, j, $1);
    END LOOP;
  END LOOP;

END
$BODY$
LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION test_ids_size(pow INTEGER, directed BOOLEAN)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
vids BIGINT[] = ARRAY[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18];
x BIGINT;
q TEXT;
mainq TEXT;
otherq TEXT;
expected TEXT;
BEGIN
  UPDATE edges
  SET id = id + POWER(10, pow), source = source + POWER(10, pow), target = target + POWER(10, pow);

  FOR i IN 1..18 LOOP
    vids[i] := POWER(10, pow) + vids[i];
  END LOOP;

  mainq = $d$ SELECT * FROM pgr_contraction(
      $$SELECT id, source, target, cost, reverse_cost FROM edges WHERE id  = ANY('%1$s'::BIGINT[]) $$,
      ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], '%2$s'::BOOLEAN)$d$;

  q = format(mainq, ARRAY[vids[5]], directed);

  RETURN QUERY
  SELECT set_eq(q,
    $$SELECT
    'v'::CHAR AS type,
    $$ || vids[11] || $$::BIGINT AS id,
    ARRAY[ $$ || vids[10] || $$]::BIGINT[] AS contracted_vertices,
    -1::BIGINT AS source,
    -1::BIGINT AS target,
    -1::FLOAT AS cost$$);

  q = format(mainq, ARRAY[vids[2],vids[4],vids[5],vids[8]], directed);

  RETURN QUERY
  SELECT is_empty(q);

  q = format(mainq, ARRAY[vids[1]], directed);

  RETURN QUERY
  SELECT set_eq(q,
    $$SELECT
    'v'::CHAR AS type,
    $$ || vids[6] || $$::BIGINT AS id,
    ARRAY[ $$ || vids[5] || $$]::BIGINT[] AS contracted_vertices,
    -1::BIGINT AS source,
    -1::BIGINT AS target,
    -1::FLOAT AS cost$$);

  otherq = $d$ SELECT type, id, unnest(contracted_vertices) FROM pgr_contraction(
      $$SELECT id, source, target, cost, reverse_cost FROM edges WHERE id  = ANY('%1$s'::BIGINT[]) $$,
      ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], '%2$s'::BOOLEAN)$d$;

  q = format(otherq, ARRAY[vids[1], vids[2]], directed);

  expected = format($d$
    SELECT type, id, unnest
    FROM (VALUES
      ('v'::TEXT, %1$s::BIGINT, %2$s::BIGINT),
      ('v', %1$s, %3$s)
    ) AS t(type, id, unnest)$d$,
    vids[10], vids[5], vids[6]) ;

  RETURN QUERY SELECT set_eq(q, expected);

  q = format(otherq, ARRAY[vids[1], vids[2], vids[3]], directed);

  expected = format($d$
    SELECT type, id, unnest
    FROM (VALUES
      ('v'::TEXT, %1$s::BIGINT, %2$s::BIGINT),
      ('v', %1$s, %3$s),
      ('v', %1$s, %4$s)
    ) AS t(type, id, unnest)$d$,
    vids[6], vids[5], vids[10], vids[15]) ;

  RETURN QUERY
  SELECT set_eq(q, expected);

  q = format($d$ SELECT type, id, unnest(contracted_vertices) FROM pgr_contraction(
      $$SELECT id, source, target, cost, reverse_cost FROM edges$$,
      ARRAY[1]::integer[], 1, ARRAY[]::BIGINT[], '%1$s'::BOOLEAN)$d$, directed);

  expected = format($d$
    SELECT type, id, unnest
    FROM (VALUES
      ('v'::CHAR, %1$s::BIGINT, %2$s),
      ('v', %3$s, %4$s),
      ('v', %5$s, %6$s),
      ('v', %5$s, %7$s),
      ('v', %8$s, %9$s),
      ('v', %10$s, %11$s)
    ) AS t(type, id, unnest)$d$,
    vids[4], vids[2],
    vids[6], vids[5],
    vids[7], vids[1], vids[3],
    vids[8], vids[9],
    vids[14], vids[13]);

    RETURN QUERY
    SELECT set_eq(q, expected);

    UPDATE edges
    SET id = id - POWER(10, pow), source = source - POWER(10, pow), target = target - POWER(10, pow);
END;
$BODY$
LANGUAGE plpgSQL;
