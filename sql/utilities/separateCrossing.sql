/*PGR-GNU*****************************************************************

Copyright (c) 2025 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2025 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

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

--v3.8
CREATE FUNCTION pgr_separateCrossing(
  TEXT, -- edges SQL
  tolerance FLOAT DEFAULT 0.01, -- tolerance
  dryrun BOOLEAN DEFAULT false,

  OUT seq INTEGER,
  OUT id BIGINT,
  OUT sub_id INTEGER,
  OUT geom geometry)
RETURNS SETOF RECORD AS
$BODY$
DECLARE
  edges_sql TEXT := $1;

  the_query TEXT;
  sqlhint TEXT;
  has_geom BOOLEAN;
  has_id BOOLEAN;
BEGIN

  IF tolerance <= 0 THEN
    RAISE EXCEPTION $$'tolerance' must be a positive number (given %)$$, tolerance
      USING ERRCODE = '22023'; -- invalid_parameter_value
  END IF;

  BEGIN
    edges_sql := _pgr_checkQuery($1);
    EXCEPTION WHEN OTHERS THEN
      GET STACKED DIAGNOSTICS sqlhint = PG_EXCEPTION_HINT;
      RAISE EXCEPTION '%', SQLERRM USING HINT = sqlhint, ERRCODE = SQLSTATE;
  END;

  has_id := _pgr_checkColumn(edges_sql, 'id', 'ANY-INTEGER', false, dryrun);
  has_geom := _pgr_checkColumn(edges_sql, 'geom', 'geometry', false, dryrun);

  IF NOT has_id OR NOT has_geom THEN
    RAISE EXCEPTION $$'id' or 'geom' are missing$$ USING HINT = edges_sql;
  END IF;

  the_query := format(
    $$
    WITH
        edges_table AS (
          %s
        ),

    get_crossings AS (
      SELECT e1.id id1, e2.id id2, e1.geom AS g1, e2.geom AS g2, ST_Intersection(e1.geom, e2.geom) AS point
      FROM edges_table e1, edges_table e2
      WHERE e1.id < e2.id AND ST_Crosses(e1.geom, e2.geom)
    ),

    crossings AS (
      SELECT id1, g1, point FROM get_crossings
      UNION
      SELECT id2, g2, point FROM get_crossings
    ),

    blades AS (
      SELECT id1, g1, ST_UnaryUnion(ST_Collect(point)) AS blade
      FROM crossings
      GROUP BY id1, g1
    ),

    collection AS (
      SELECT id1, (st_dump(st_split(st_snap(g1, blade, %2$s), blade))).*
      FROM blades
    )

    SELECT row_number() over()::INTEGER AS seq, id1::BIGINT, path[1], geom
    FROM collection;
    $$,

    edges_sql, tolerance);

    IF dryrun THEN
      RAISE NOTICE '%', the_query || ';';
    ELSE
      RETURN QUERY EXECUTE the_query;
    END IF;

END;
$BODY$ LANGUAGE 'plpgsql' VOLATILE STRICT;

COMMENT ON FUNCTION pgr_separateCrossing(TEXT, FLOAT, BOOLEAN)
IS 'pgr_separateCrossing
- Parameters
  - Edges SQL with columns: id, geom
- Optional parameters
  - tolerance => 0.01
  - dryrun => true
- DOCUMENTATION:
  - ${PROJECT_DOC_LINK}/pgr_separateCrossing.html
';
