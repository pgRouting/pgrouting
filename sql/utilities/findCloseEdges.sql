/*PGR-GNU*****************************************************************

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
CREATE FUNCTION pgr_findCloseEdges(
  TEXT,
  geometry[],
  FLOAT,
  cap INTEGER DEFAULT 1,
  dryrun BOOLEAN default false,

  OUT edge_id BIGINT,
  OUT fraction FLOAT,
  OUT side CHAR,
  OUT distance FLOAT,
  OUT geom geometry,
  OUT edge geometry)
returns SETOF RECORD AS
$BODY$
DECLARE
  geom_arr geometry[] := $2;
  tolerance FLOAT := $3;
  cap INTEGER := $4;
  dryrun BOOLEAN := $5;
  edges_SQL TEXT;
  has_id BOOLEAN;
  has_geom BOOLEAN;
  ret_query TEXT;
  ret_query_end TEXT;

  sqlhint TEXT;

BEGIN

  IF (tolerance < 0) THEN
    RAISE EXCEPTION 'Invalid value for "tolerance"';
  END IF;

  IF (cap < 0) THEN
    RAISE EXCEPTION 'Invalid value for "cap"';
  END IF;

  BEGIN
    edges_sql := _pgr_checkQuery($1);
    has_id := _pgr_checkColumn(edges_sql, 'id', 'ANY-INTEGER', false, dryrun => dryrun);
    has_geom := _pgr_checkColumn(edges_sql, 'geom', 'geometry', false, dryrun => dryrun);
    EXCEPTION WHEN OTHERS THEN
      GET STACKED DIAGNOSTICS sqlhint = PG_EXCEPTION_HINT;
      RAISE EXCEPTION '%', SQLERRM USING HINT = sqlhint, ERRCODE = SQLSTATE;
  END;

  ret_query = format(
    $q$
WITH
edges_sql AS (%1$s),
point_sql AS (SELECT unnest(%2$L::geometry[]) AS point),
results AS (
  SELECT
    id::BIGINT AS edge_id,
    ST_LineLocatePoint(geom, point) AS fraction,
    CASE WHEN ST_Intersects(ST_Buffer(geom, %3$s, 'side=right endcap=flat'), point)
         THEN 'r'
         ELSE 'l' END::CHAR AS side,
    geom <-> point AS distance,
    point,
    $q$, edges_sql, geom_arr, tolerance);

  ret_query_end = format(
    $q$
  FROM  edges_sql, point_sql
  WHERE ST_DWithin(geom, point, %1$s)
  ORDER BY geom <-> point),
prepare_cap AS (
  SELECT row_number() OVER (PARTITION BY point ORDER BY point, distance) AS rn, *
  FROM results)
SELECT edge_id, fraction, side, distance, point, new_line
FROM prepare_cap
WHERE rn <= %2$s
    $q$, tolerance, cap);

    ret_query = ret_query
      || $q$ST_MakeLine(point, ST_ClosestPoint(geom, point)) AS new_line $q$
      || ret_query_end;

  IF dryrun THEN
    RAISE NOTICE '%', ret_query;
    RETURN;
  END IF;

  RETURN query EXECUTE ret_query;

END;
$BODY$
LANGUAGE PLPGSQL VOLATILE STRICT
COST 5;

COMMENT ON FUNCTION pgr_findCloseEdges(TEXT, GEOMETRY[], FLOAT, INTEGER, BOOLEAN)
IS 'pgr_findCloseEdges(Many Points)
- Parameters:
  - Edges SQL with columns: id, geom
  - Array of POINT geometries
  - Maximum separation between geometries
- Optional Parameters
  - cap => 1: at most one answer
  - dryrun => false: do not output code
- Documentation:
   - ${PROJECT_DOC_LINK}/pgr_findCloseEdges.html
';

--v3.8
CREATE FUNCTION pgr_findCloseEdges(
  TEXT,
  geometry,
  FLOAT,
  cap INTEGER DEFAULT 1,
  dryrun BOOLEAN default false,
  OUT edge_id BIGINT,
  OUT fraction FLOAT,
  OUT side CHAR,
  OUT distance FLOAT,
  OUT geom geometry,
  OUT edge geometry)
returns SETOF RECORD AS
$BODY$
  SELECT edge_id, fraction, side, distance, geom, edge
  FROM pgr_findCloseEdges($1, ARRAY[$2]::GEOMETRY[], $3, cap, dryrun);
$BODY$
LANGUAGE SQL VOLATILE STRICT COST 5;

COMMENT ON FUNCTION pgr_findCloseEdges(TEXT, GEOMETRY, FLOAT, INTEGER, BOOLEAN)
IS 'pgr_findCloseEdges(One Point)
- Parameters:
  - Edges SQL with columns: id, geom
  - POINT geometry
  - Maximum separation between geometries
- Optional Parameters
  - cap => 1: at most one answer
  - dryrun => false: do not output code
- Documentation:
   - ${PROJECT_DOC_LINK}/pgr_findCloseEdges.html
';

--v3.4
CREATE FUNCTION pgr_findCloseEdges(
  TEXT,
  geometry,
  FLOAT,
  cap INTEGER,
  partial BOOLEAN,
  dryrun BOOLEAN,

  OUT edge_id BIGINT,
  OUT fraction FLOAT,
  OUT side CHAR,
  OUT distance FLOAT,
  OUT geom geometry,
  OUT edge geometry)
returns SETOF RECORD AS
$BODY$
  SELECT edge_id, fraction, side, distance, geom, edge
  FROM pgr_findCloseEdges($1, ARRAY[$2]::GEOMETRY[], $3, cap, dryrun);
$BODY$
LANGUAGE SQL VOLATILE STRICT COST 5;

COMMENT ON FUNCTION pgr_findCloseEdges(TEXT, GEOMETRY, FLOAT, INTEGER, BOOLEAN, BOOLEAN)
IS 'pgr_findCloseEdges deprecated signature on v3.8.0';

--v3.4
CREATE FUNCTION pgr_findCloseEdges(
  TEXT,
  geometry[],
  FLOAT,
  cap INTEGER,
  partial BOOLEAN,
  dryrun BOOLEAN,

  OUT edge_id BIGINT,
  OUT fraction FLOAT,
  OUT side CHAR,
  OUT distance FLOAT,
  OUT geom geometry,
  OUT edge geometry)
returns SETOF RECORD AS
$BODY$
  SELECT edge_id, fraction, side, distance, geom, edge
  FROM pgr_findCloseEdges($1, $2, $3, cap, dryrun);
$BODY$
LANGUAGE SQL VOLATILE STRICT COST 5;

COMMENT ON FUNCTION pgr_findCloseEdges(TEXT, GEOMETRY[], FLOAT, INTEGER, BOOLEAN, BOOLEAN)
IS 'pgr_findCloseEdges deprecated signature on v3.8.0';
