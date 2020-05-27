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


CREATE OR REPLACE FUNCTION pgr_extractVertices(
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
    has_source BOOLEAN := TRUE;
    has_points BOOLEAN := TRUE;
    has_id BOOLEAN := TRUE;
    fnName TEXT;
    rec RECORD;

BEGIN
    fnName = 'pgr_extractVertices';

    -- get the query
    BEGIN
        quoted = '.*' || $1 || '\s*as';
        query = format($$
            SELECT regexp_replace(regexp_replace(statement, %1$L,'','i'),';$','') FROM pg_prepared_statements WHERE name = %2$L$$,
            quoted, $1);
        EXECUTE query INTO edges_SQL;

        EXCEPTION WHEN OTHERS
            THEN edges_sql := $1;
    END;

    IF edges_SQL IS NULL THEN
        edges_SQL := $1;
    END IF;

    -- query is executable
    BEGIN
        query = 'SELECT * FROM ('||edges_sql||' ) AS __a__ limit 1';
        EXECUTE query;

        EXCEPTION WHEN OTHERS THEN
            RAISE EXCEPTION '%', SQLERRM
            USING HINT = 'Please check query: '|| $1;
        RETURN;
    END;

    -- has edge identifier
    BEGIN
        query = 'SELECT id FROM ('||edges_sql||' ) AS __a__ limit 1';
        EXECUTE query;

        query = 'SELECT pg_typeof(id) FROM ('||edges_sql||' ) AS __a__ limit 1';
        EXECUTE query INTO rec;

        EXCEPTION WHEN OTHERS THEN
            has_id := FALSE;
    END;

    IF NOT dryrun AND has_id THEN
        IF  rec.pg_typeof NOT IN ('smallint','integer','bigint') THEN
            RAISE EXCEPTION 'Expected type of column "id" is ANY-INTEGER'
            USING HINT = 'Please check query: '|| $1;
        END IF;
    END IF;

    -- has geometry?
    BEGIN
        query = 'SELECT geom FROM ('||edges_sql||' ) AS __a__ limit 1';
        EXECUTE query;

        EXCEPTION WHEN OTHERS THEN
            has_geom := FALSE;
    END;

    -- has points?
    BEGIN
      query = 'SELECT startpoint, endpoint FROM ('||edges_sql||' ) AS __a__ limit 1';
      EXECUTE query;

      EXCEPTION WHEN OTHERS THEN
            has_points := FALSE;
    END;

    -- has source-target?
    BEGIN
      query = 'SELECT source, target FROM ('||edges_sql||' ) AS __a__ limit 1';
      EXECUTE query;

      query = 'SELECT pg_typeof(source) s_t, pg_typeof(target) t_t  FROM ('||edges_sql||' ) AS __a__ limit 1';
      EXECUTE query INTO rec;

      EXCEPTION WHEN OTHERS THEN
        has_source := FALSE;
    END;

    IF NOT dryrun AND has_source THEN
        IF  rec.s_t NOT IN ('smallint','integer','bigint') THEN
            RAISE EXCEPTION 'Expected type of column "source" is ANY-INTEGER'
            USING HINT = 'Please check query: '|| $1;
        END IF;

        IF  rec.t_t NOT IN ('smallint','integer','bigint') THEN
            RAISE EXCEPTION 'Expected type of column "target" is ANY-INTEGER'
            USING HINT = 'Please check query: '|| $1;
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

    ELSIF has_source AND has_id THEN
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

        SELECT vid AS id, in_edges, out_edges, NULL::FLOAT, NULL::FLOAT, NULL::geometry
        FROM the_points$q$;


    ELSIF has_source AND NOT has_id THEN
      -- SELECT id, source, target
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

        SELECT DISTINCT vid AS id, NULL::BIGINT[], NULL::BIGINT[], NULL::FLOAT, NULL::FLOAT, NULL::geometry
        FROM the_points$q$;

    ELSE
        RAISE EXCEPTION 'Missing column'
        USING HINT = 'Please check query: '|| $1;

    END IF;

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


COMMENT ON FUNCTION pgr_extractVertices(TEXT, BOOLEAN)
IS 'pgr_extractVertices
- EXPERIMENTAL
- Parameters
  - Edges SQL with columns: [id,] startpoint, endpoint
        OR
  - Edges SQL with columns: [id,] source, target
        OR
  - Edges SQL with columns: [id,] geom
- Documentation:
- ${PGROUTING_DOC_LINK}/pgr_extractVertices.html
';
