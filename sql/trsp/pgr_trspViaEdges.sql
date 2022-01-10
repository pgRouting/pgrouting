/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

HISTORY
-- 2022 Base the wrapper on the new code

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

-- TODO restrictions with one edge are wrong
--v4.0
CREATE FUNCTION pgr_trspViaEdges(
  TEXT,      -- SQL (required)
  integer[], -- eids (required)
  FLOAT[],   -- pcts (required)
  BOOLEAN,   -- directed (required)
  BOOLEAN,   -- has_rcost (requierd)

  turn_restrict_sql TEXT DEFAULT NULL::TEXT,

  OUT seq INTEGER,
  OUT id1 INTEGER,
  OUT id2 INTEGER,
  OUT id3 INTEGER,
  OUT cost FLOAT
)
RETURNS SETOF RECORD AS
$BODY$
DECLARE
  sql TEXT          := $1;
  eids INTEGER[]    := $2;
  pcts FLOAT[]      := $3;
  directed BOOLEAN  := $4;
  has_rcost BOOLEAN := $5;

  i integer;
  j INTEGER := 1;
  has_reverse BOOLEAN;
  point_is_vertex BOOLEAN := false;
  edges_sql TEXT;
  f FLOAT;
  point_q TEXT;
  points_sql TEXT := '';
  from_v INTEGER;
  to_v INTEGER;
  word TEXT := '';
  combinations TEXT := '';
  restrictions_query TEXT;
  vertices INTEGER[];

BEGIN
  IF $1 IS NULL OR $2 IS NULL OR $3 IS NULL OR $4 IS NULL OR $5 IS NULL THEN RETURN; END IF;


  has_reverse =_pgr_parameter_check('dijkstra', sql, false);
  edges_sql := sql;
  IF (has_reverse != has_rcost) THEN
    IF (NOT has_rcost) THEN
      -- user does not want to use reverse cost column
      edges_sql = 'SELECT id, source, target, cost FROM (' || sql || ') a';
    ELSE
      raise EXCEPTION 'has_rcost set to true but reverse_cost not found';
    END IF;
  END IF;

  IF array_length(eids, 1) != array_length(pcts, 1) THEN
    raise exception 'The length of arrays eids and pcts must be the same!';
  END IF;

  FOREACH f IN ARRAY pcts LOOP
    IF f in (0,1) THEN
      point_is_vertex := true;
    END IF;
  END LOOP;

  FOR i IN 1 .. array_length(eids, 1) i
  LOOP
    point_q := NULL;
    IF (j != 1) THEN word = ' UNION '; ELSE word = ''; END IF;
    IF pcts[i] = 0 THEN
      EXECUTE '(SELECT source FROM (' || sql || ') b WHERE id = ' ||  eids[i] || ')' INTO from_v;
    ELSIF pcts[i] = 1 THEN
      EXECUTE '(SELECT target FROM (' || sql || ') b WHERE id = ' || eids[i] || ')' INTO from_v;
    ELSE
      from_v = -j;
      point_q =  '(SELECT '|| j ||' as pid, ' || eids[i] || ' as edge_id, ' || pcts[i] || '::float8 as fraction)';
      j := j + 1;
    END IF;
    IF point_q IS NOT NULL THEN points_sql = point_q || word || points_sql; END IF;
    vertices := vertices || from_v;
  END LOOP;

  FOR i IN 1 .. array_length(vertices, 1) - 1
  LOOP
    IF (i != 1) THEN word = ' UNION '; ELSE word = ''; END IF;
    combinations := combinations || word || '(' || vertices[i] || ' AS source, ' || vertices[i+1] || ' AS target)';
  END LOOP;

  restrictions_query := format($$
    WITH
      o_restrictions AS (%1$s),
      old_restrictions AS (
        SELECT row_number() over () AS _id, to_cost, string_to_array(via_path,',')::INTEGER[]||target_id AS via_path
        FROM o_restrictions),
      the_values  AS (
        SELECT _id, to_cost, via_path AS array, s AS subscript, via_path[s] AS value
        FROM (SELECT generate_subscripts(via_path,1) AS s, _id, to_cost, via_path FROM old_restrictions) AS foo)
    SELECT _id AS id, to_cost AS cost, array_agg(value ORDER BY subscript DESC) AS path
    FROM the_values GROUP BY _id, to_cost;
    $$, turn_restrict_sql);

  IF (turn_restrict_sql IS NULL OR length(turn_restrict_sql) = 0) THEN
    -- no restrictions then its a pgr_withPointsVia
    RETURN query SELECT a.seq::INTEGER, path_id::INTEGER AS id1, node::INTEGER AS id2, edge::INTEGER AS id3, a.cost
    FROM pgr_withPointsVia(edges_sql, points_sql, vertices, directed) a;
    RETURN;
  END IF;

  RETURN QUERY
  SELECT a.seq-1::INTEGER, path_id::INTEGER, node::INTEGER, edge::INTEGER, a.cost::FLOAT FROM
  pgr_trsp_withPointsVia(edges_sql, restrictions_query, points_sql, vertices, directed) AS a;

END;
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100
ROWS 1000;


-- COMMENTS

COMMENT ON FUNCTION pgr_trspViaEdges(TEXT, INTEGER[], FLOAT[], BOOLEAN, BOOLEAN, TEXT)
IS 'pgr_trspViaEdges
- PROTOTYPE
- Parameters
  - edges SQL with columns: id, source, target, cost [,reverse_cost]
  - ARRAY[Via edge identifiers]
  - ARRAY[fraction position on via edges]
  - directed
  - has reverse cost
- Optional parameters
  - turn_restrict_sql := NULL
- Documentation:
  - ${PROJECT_DOC_LINK}/pgr_trsp.html
';
