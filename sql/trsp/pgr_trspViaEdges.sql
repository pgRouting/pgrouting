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


--v3.0
--v3.0
CREATE FUNCTION pgr_trspViaEdges(
    text,      -- SQL (required)
    integer[], -- eids (required)
    FLOAT[],   -- pcts (required)
    BOOLEAN,   -- directed (required)
    BOOLEAN,   -- has_rcost (requierd)

    turn_restrict_sql text DEFAULT NULL::text,

    OUT seq INTEGER,
    OUT id1 INTEGER,
    OUT id2 INTEGER,
    OUT id3 INTEGER,
    OUT cost FLOAT
)
RETURNS SETOF RECORD AS
$body$
/*
 *  pgr_trsp(sql text, eids integer[], pcts float8[], directed boolean, has_reverse_cost boolean, turn_restrict_sql text DEFAULT NULL::text)
 *
 *  Compute TRSP with edge_ids and pposition along edge. We compute the path between eids[i], pcts[i] and eids[i+1], pcts[i+1]
 *  and chain the results together.
 *
 *  NOTE: this is a prototype function, we can gain a lot of efficiencies by implementing this in C/C++
 *
*/
declare
    sql TEXT          := $1;
    eids INTEGER[]    := $2;
    pcts FLOAT[]      := $3;
    directed BOOLEAN  := $4;
    has_rcost BOOLEAN := $5;

    i integer;
    j INTEGER := 1;
    seq2 integer :=0;
    has_reverse BOOLEAN;
    point_is_vertex BOOLEAN := false;
    edges_sql TEXT;
    f float;
    point_q TEXT;
    points_sql TEXT := '';
    from_v INTEGER;
    to_v INTEGER;
    word TEXT := '';
    combinations TEXT := '';
    x INTEGER[];
    restrictions_query TEXT;

begin
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

    if array_length(eids, 1) != array_length(pcts, 1) then
        raise exception 'The length of arrays eids and pcts must be the same!';
    end if;

    FOREACH f IN ARRAY pcts LOOP
        IF f in (0,1) THEN
           point_is_vertex := true;
        END IF;
    END LOOP;

    IF (turn_restrict_sql IS NULL OR length(turn_restrict_sql) = 0) AND NOT point_is_vertex THEN
        -- no restrictions then its a _pgr_withPointsVia
        RETURN query SELECT a.seq::INTEGER, path_id::INTEGER AS id1, node::INTEGER AS id2, edge::INTEGER AS id3, a.cost
        FROM _pgr_withPointsVia(edges_sql, eids, pcts, directed) a;
        RETURN;
    END IF;


  FOR i IN 1 .. array_length(eids, 1) - 1 LOOP
    point_q := NULL;
    if (j != 1) THEN word = ' UNION '; ELSE word = ''; END IF;
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

    point_q := NULL;
    if (j != 1) THEN word = ' UNION '; ELSE word = ''; END IF;
    IF pcts[i+1] = 0 THEN
        EXECUTE '(SELECT source FROM (' || sql || ') c WHERE id = ' ||  eids[i+1] || ')' INTO to_v;
    ELSIF pcts[i+1] = 1 THEN
        EXECUTE '(SELECT target FROM (' || sql || ') c WHERE id = ' ||  eids[i+1] || ')' INTO to_v;
    ELSE
      to_v = -j;
      point_q =  '(SELECT '|| j ||' as pid, ' || eids[i+1] || ' as edge_id, ' || pcts[i+1] || '::float8 as fraction)';
      j := j + 1;
    END IF;

    IF point_q IS NOT NULL THEN points_sql = point_q || word || points_sql; END IF;
    RAISE WARNING '%->%', from_v, to_v;

    if (i != 1) THEN word = ' UNION '; ELSE word = ''; END IF;
    combinations := combinations || word || '(' || from_v || ' AS source, ' || to_v || ' AS target)';
  END LOOP;

  RAISE WARNING '%', points_sql;
  RAISE WARNING '%', combinations;

  restrictions_query = $$
  WITH old_restrictions AS ( $$ ||
    turn_restrict_sql || $$
  )
  SELECT ROW_NUMBER() OVER() AS id,
  _pgr_array_reverse(array_prepend(target_id, string_to_array(via_path::text, ',')::INTEGER[])) AS path,
  to_cost AS cost
  FROM old_restrictions;
  $$;

  FOR i IN 1 .. array_length(eids, 1) - 1 LOOP
    RETURN QUERY
    SELECT  a.seq-1 AS seq, i AS id1, node::INTEGER AS id2, edge::INTEGER AS id3, a.cost FROM pgr_trsp_withpoints(
      sql,
      restrictions_query,
      points_sql,
      combinations,
      directed
      ) AS a;
  END LOOP;

end;
$body$
language plpgsql VOLATILE STRICT
cost 100
rows 1000;


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
