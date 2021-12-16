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

    i INTEGER;
    j INTEGER := 1;
    edge INTEGER;
    fraction FLOAT;

    union_sql1 TEXT;
    union_sql TEXT := '';
    combination INTEGER[][];
    from_v INTEGER;
    to_v INTEGER;
    rr RECORD;
    lrr RECORD;
    first boolean := true;
    seq1 integer := 0;
    seq2 integer :=0;
    has_reverse BOOLEAN;
    point_is_vertex BOOLEAN := false;
    edges_sql TEXT;
    f float;

begin


    SELECT 0::INTEGER AS seq, NULL::INTEGER AS id1, NULL::INTEGER AS id2, NULL::INTEGER AS id3, NULL::FLOAT AS cost INTO lrr;
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

    FOR i IN 1 .. array_length(eids, 1) - 1 LOOP
      union_sql1 := NULL;
      IF pcts[i] = 0 THEN
        EXECUTE '(SELECT source FROM (' || sql || ') b WHERE id = ' ||  eids[i] || ')' INTO from_v;
      ELSIF pcts[i] = 1 THEN
        EXECUTE '(SELECT target FROM (' || sql || ') b WHERE id = ' || eids[i] || ')' INTO from_v;
      ELSE
        from_v = -j;
        union_sql1 =  '(SELECT '|| j ||' as pid, ' || eids[i] || ' as edge_id, ' || pcts[i] || '::float8 as fraction)';
        j := j + 1
      END IF;
      IF union_sql1 IS NOT NULL THEN union_sql = union_sql1 || ' UNION ' || union_sql; END IF;

      union_sql1 := NULL;
      IF pcts[i+1] = 0 THEN
          EXECUTE '(SELECT source FROM (' || sql || ') c WHERE id = ' ||  eids[i+1] || ')' INTO to_v;
      ELSIF pcts[i+1] = 1 THEN
          EXECUTE '(SELECT target FROM (' || sql || ') c WHERE id = ' ||  eids[i+1] || ')' INTO to_v;
      ELSE
        to_v = -j;
        union_sql1 =  '(SELECT '|| j ||' as pid, ' || eids[i+1] || ' as edge_id, ' || pcts[i+1] || '::float8 as fraction)';
        j := j + 1
      END IF;

      IF union_sql1 IS NOT NULL THEN union_sql = union_sql1 || ' UNION ' || union_sql; END IF;
      RAISE WARNING '%->%', from_v, to_v;
    END LOOP;

    RAISE WARNING '%', union_sql;

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


    if array_length(eids, 1) != array_length(pcts, 1) then
        raise exception 'The length of arrays eids and pcts must be the same!';
    end if;

    FOR i IN 1 .. array_length(eids, 1) - 1
    LOOP
        RETURN QUERY
        SELECT a.seq, seq2 as id1, a.id1 as id2, a.id2 as id3, a.cost
        FROM _pgr_withPointsTRSP(edges_sql,
          eids[i], pcts[i],
          eids[i+1], pcts[i+1],
          directed,
          turn_restrict_sql) AS a;
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
