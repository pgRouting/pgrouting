/*PGR-GNU*****************************************************************
File: degree.sql

Copyright (c) 2022 ~ Celia Virginia Vergara Castillo
mail: vicky at georepublic.de

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


--v3.4
CREATE FUNCTION pgr_degree(
  TEXT,  -- Edges SQL
  TEXT,  -- Vertices SQL

  dryrun BOOLEAN DEFAULT false,

  OUT id BIGINT,
  OUT degree BIGINT
)
RETURNS SETOF RECORD AS
$BODY$
DECLARE
  edges_sql TEXT;
  vertices_sql TEXT;
  has_in_edges BOOLEAN := TRUE;
  has_out_edges BOOLEAN := TRUE;
  eids TEXT;
  query TEXT;

BEGIN

  edges_sql := _pgr_checkQuery($1);
  PERFORM _pgr_checkColumn(edges_sql, 'id', 'ANY-INTEGER', dryrun => $3);

  vertices_sql := _pgr_checkQuery($2);
  PERFORM _pgr_checkColumn(vertices_sql, 'id', 'ANY-INTEGER', dryrun => $3);
  has_in_edges := _pgr_checkColumn(vertices_sql, 'in_edges', 'ANY-INTEGER[]', true, dryrun => $3);
  has_out_edges := _pgr_checkColumn(vertices_sql, 'out_edges', 'ANY-INTEGER[]', true, dryrun => $3);

  IF has_in_edges THEN
    eids = $$coalesce(in_edges::BIGINT[], '{}'::BIGINT[])$$;
  END IF;

  IF has_out_edges THEN
    IF has_in_edges THEN
      eids = E'\n          ' || eids
            || E'\n          ||\n          '
            || $$coalesce(out_edges::BIGINT[], '{}'::BIGINT[])$$;
    ELSE
      eids = $$coalesce(out_edges::BIGINT[], '{}'::BIGINT[])$$;
    END IF;
  ELSE
    IF NOT has_in_edges THEN
      RAISE EXCEPTION 'Missing column'
      USING HINT = 'Column "in_edges" and/or "out_edges" is missing in'||E'\n'||vertices_sql;
    END IF;
  END IF;

  query := format($q$
    WITH

    -- a sub set of edges of the graph goes here
    g_edges AS (
      $q$ || edges_sql || $q$
    ),

    -- sub set of vertices of the graph goes here
    all_vertices AS (
      $q$ || vertices_sql || $q$
    ),

    g_vertices AS (
      SELECT id,
        unnest(%s) AS eid
      FROM all_vertices
    ),

    totals AS (
      SELECT v.id, count(*)
      FROM g_vertices AS v
      JOIN g_edges AS e ON (e.id = eid) GROUP BY v.id
    )

    SELECT id::BIGINT, coalesce(count, 0)::BIGINT FROM all_vertices LEFT JOIN totals USING (id)
    $q$, eids);

  IF dryrun THEN
    RAISE NOTICE '%', query || ';';
  ELSE
    RETURN QUERY EXECUTE query;
  END IF;

  EXCEPTION WHEN OTHERS THEN
    RAISE EXCEPTION '%', SQLERRM
    USING HINT = 'Please check query: '|| $1;

END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


-- COMMENTS
COMMENT ON FUNCTION pgr_degree(TEXT, TEXT, BOOLEAN)
IS 'pgr_degree
- PROPOSED
- Parameters
- Edges SQL with columns: id
- Vertices SQL with columns: id, in_edges, out_edges
- Documentation:
- ${PROJECT_DOC_LINK}/pgr_degree.html
';
