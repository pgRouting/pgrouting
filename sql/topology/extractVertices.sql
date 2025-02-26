/*PGR-GNU*****************************************************************

Copyright (c) 2019 ~ pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2019 ~ Celia Virginia Vergara Castillo
mail: vicky@georepublic.de

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
CREATE FUNCTION pgr_extractVertices(
    TEXT,  -- SQL inner query (required)

    dryrun BOOLEAN DEFAULT false,

    OUT id BIGINT,
    OUT in_edges BIGINT[],
    OUT out_edges BIGINT[],
    OUT x FLOAT,
    OUT y FLOAT,
    OUT geom geometry
)
RETURNS SETOF RECORD AS
$BODY$
DECLARE
    edges_SQL TEXT;
    quoted TEXT;
    query TEXT;
    has_geom BOOLEAN := TRUE;
    has_st BOOLEAN := TRUE;
    has_source BOOLEAN := TRUE;
    has_target BOOLEAN := TRUE;
    has_points BOOLEAN := TRUE;
    has_start BOOLEAN := TRUE;
    has_end BOOLEAN := TRUE;
    has_id BOOLEAN := TRUE;
    rec RECORD;

    sqlhint TEXT;

BEGIN
  BEGIN
    edges_sql := _pgr_checkQuery($1);
    EXCEPTION WHEN OTHERS THEN
      GET STACKED DIAGNOSTICS sqlhint = PG_EXCEPTION_HINT;
      RAISE EXCEPTION '%', SQLERRM USING HINT = sqlhint, ERRCODE = SQLSTATE;
  END;

  has_id := _pgr_checkColumn(edges_sql, 'id', 'ANY-INTEGER', true, dryrun => $2);
  has_source := _pgr_checkColumn(edges_sql, 'source', 'ANY-INTEGER', true, dryrun => $2);
  has_target := _pgr_checkColumn(edges_sql, 'target', 'ANY-INTEGER', true, dryrun => $2);

  has_geom := _pgr_checkColumn(edges_sql, 'geom', 'geometry', true, dryrun => $2);
  has_start := _pgr_checkColumn(edges_sql, 'startpoint', 'geometry', true, dryrun => $2);
  has_end   := _pgr_checkColumn(edges_sql, 'endpoint', 'geometry', true, dryrun => $2);
  has_points := has_start AND has_end;
  has_st := has_source AND has_target;

  IF (NOT has_geom) THEN
    IF (has_target AND NOT has_source) THEN
        RAISE EXCEPTION 'column "source" does not exist' USING HINT = $1, ERRCODE = 42703;
    ELSIF (NOT has_target AND has_source) THEN
        RAISE EXCEPTION 'column "target" does not exist' USING HINT = $1, ERRCODE = 42703;
    ELSIF (has_start AND NOT has_end) THEN
      RAISE EXCEPTION 'column "endpoint" does not exist' USING HINT = $1, ERRCODE = 42703;
    ELSIF (NOT has_start AND has_end) THEN
      RAISE EXCEPTION 'column "startpoint" does not exist' USING HINT = $1, ERRCODE = 42703;
    ELSIF (NOT has_st AND NOT has_points AND NOT has_geom) THEN
      RAISE EXCEPTION 'column "geom" does not exist' USING HINT = $1, ERRCODE = 42703;
    END IF;
  END IF;


    IF has_geom AND has_id THEN
      -- SELECT id, geom
      query := $q$
        WITH

        main_sql AS (
          $q$ || edges_sql || $q$
        ),

        the_out AS (
          SELECT id::BIGINT AS out_edge, ST_StartPoint(geom) AS geom
          FROM main_sql
        ),

        agg_out AS (
          SELECT array_agg(out_edge ORDER BY out_edge) AS out_edges, ST_x(geom) AS x, ST_Y(geom) AS y, geom
          FROM the_out
          GROUP BY geom
        ),

        the_in AS (
          SELECT id::BIGINT AS in_edge, ST_EndPoint(geom) AS geom
          FROM main_sql
        ),

        agg_in AS (
          SELECT array_agg(in_edge ORDER BY in_edge) AS in_edges, ST_x(geom) AS x, ST_Y(geom) AS y, geom
          FROM the_in
          GROUP BY geom
        ),

        the_points AS (
          SELECT in_edges, out_edges, coalesce(agg_out.geom, agg_in.geom) AS geom
          FROM agg_out
          FULL OUTER JOIN agg_in USING (x, y)
        )

        SELECT row_number() over(ORDER BY ST_X(geom), ST_Y(geom)) AS id, in_edges, out_edges, ST_X(geom), ST_Y(geom), geom
        FROM the_points$q$;

    ELSIF has_geom AND NOT has_id THEN
      -- SELECT startpoint, endpoint
      -- can not get the ins and outs
      query := $q$
        WITH

        main_sql AS (
          $q$ || edges_sql || $q$
        ),

        sub_main AS (
          SELECT ST_StartPoint(geom) AS startpoint, ST_EndPoint(geom) AS endpoint
          FROM main_sql
        ),

        the_out AS (
          SELECT  DISTINCT ST_X(startpoint) AS x, ST_Y(startpoint) AS y, startpoint AS geom
          FROM sub_main
        ),

        the_in AS (
            SELECT DISTINCT ST_X(endpoint) AS x, ST_Y(endpoint) AS y, endpoint AS geom
          FROM sub_main
        ),

        the_points AS (
          SELECT x, y, coalesce(the_out.geom, the_in.geom) AS geom
          FROM the_out
          FULL OUTER JOIN the_in USING (x, y)
        )

        SELECT row_number() over(ORDER BY  ST_X(geom), ST_Y(geom)) AS id, NULL::BIGINT[], NULL::BIGINT[], x, y, geom
        FROM the_points$q$;

    ELSIF has_points AND has_id THEN
      -- SELECT id, startpoint, endpoint
      query := $q$
        WITH

        main_sql AS (
          $q$ || edges_sql || $q$
        ),

        the_out AS (
          SELECT id::BIGINT AS out_edge, startpoint AS geom
          FROM main_sql
        ),

        agg_out AS (
          SELECT array_agg(out_edge ORDER BY out_edge) AS out_edges, ST_x(geom) AS x, ST_Y(geom) AS y, geom
          FROM the_out
          GROUP BY geom
        ),

        the_in AS (
          SELECT id::BIGINT AS in_edge, endpoint AS geom
          FROM main_sql
        ),

        agg_in AS (
          SELECT array_agg(in_edge ORDER BY in_edge) AS in_edges, ST_x(geom) AS x, ST_Y(geom) AS y, geom
          FROM the_in
          GROUP BY geom
        ),

        the_points AS (
          SELECT in_edges, out_edges, coalesce(agg_out.geom, agg_in.geom) AS geom
          FROM agg_out
          FULL OUTER JOIN agg_in USING (x, y)
        )

        SELECT row_number() over(ORDER BY  ST_X(geom), ST_Y(geom)) AS id, in_edges, out_edges, ST_X(geom), ST_Y(geom), geom
        FROM the_points$q$;

    ELSIF has_points AND NOT has_id THEN
      -- SELECT startpoint, endpoint
      -- can not get the ins and outs
      query := $q$
        WITH

        main_sql AS (
          $q$ || edges_sql || $q$
        ),

        the_out AS (
          SELECT DISTINCT ST_X(startpoint) AS x, ST_Y(startpoint) AS y, startpoint AS geom
          FROM main_sql
        ),

        the_in AS (
            SELECT DISTINCT ST_X(endpoint) AS x, ST_Y(endpoint) AS y, endpoint AS geom
          FROM main_sql
        ),

        the_points AS (
          SELECT x, y, coalesce(the_out.geom, the_in.geom) AS geom
          FROM the_out
          FULL OUTER JOIN the_in USING (x, y)
        )

        SELECT row_number() over(ORDER BY  ST_X(geom), ST_Y(geom)) AS id, NULL::BIGINT[], NULL::BIGINT[], x, y, geom
        FROM the_points$q$;

    ELSIF has_st AND has_id THEN
      -- SELECT id, source, target
      query := $q$ WITH

        main_sql AS (
          $q$ || edges_sql || $q$
        ),

        agg_out AS (
          SELECT source AS vid, array_agg(id::BIGINT) AS out_edges
          FROM main_sql
          GROUP BY source
        ),

        agg_in AS (
          SELECT target AS vid, array_agg(id::BIGINT) AS in_edges
          FROM main_sql
          GROUP BY target
        ),

        the_points AS (
          SELECT vid, in_edges, out_edges
          FROM agg_out
          FULL OUTER JOIN agg_in USING (vid)
        )

        SELECT vid::BIGINT AS id, in_edges, out_edges, NULL::FLOAT, NULL::FLOAT, NULL::geometry
        FROM the_points$q$;


    ELSIF has_st AND NOT has_id THEN
      -- SELECT source, target
      query := $q$
        WITH

        main_sql AS (
          $q$ || edges_sql || $q$
        ),


        the_points AS (
          SELECT source AS vid FROM main_sql
          UNION
          SELECT target FROM main_sql
        )

        SELECT DISTINCT vid::BIGINT AS id, NULL::BIGINT[], NULL::BIGINT[], NULL::FLOAT, NULL::FLOAT, NULL::geometry
        FROM the_points$q$;

    END IF;

    IF dryrun THEN
      RAISE NOTICE '%', query || ';';
    ELSE
      RETURN QUERY EXECUTE query;
    END IF;

END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


-- COMMENTS


COMMENT ON FUNCTION pgr_extractVertices(TEXT, BOOLEAN)
IS 'pgr_extractVertices
- Parameters
  - Edges SQL with columns: [id,] startpoint, endpoint
        OR
  - Edges SQL with columns: [id,] source, target
        OR
  - Edges SQL with columns: [id,] geom
- Documentation:
- ${PROJECT_DOC_LINK}/pgr_extractVertices.html
';
