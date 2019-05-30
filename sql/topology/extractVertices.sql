/*PGR-GNU*****************************************************************
File: extractVertices.sql

Copyright (c) 2018 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; WITHout even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along WITH this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/

---------------------------
-- pgr_extractVertices
---------------------------


CREATE OR REPLACE FUNCTION pgr_extractVertices(
    TEXT,  -- SQL inner query (required)

    OUT id BIGINT,
    OUT x FLOAT,
    OUT y FLOAT,
    OUT the_geom geometry
)
RETURNS SETOF RECORD AS
$BODY$
DECLARE
    edges_sql TEXT;
    quoted TEXT;
    query TEXT;
    has_geom BOOLEAN := TRUE;
    fnName TEXT;

BEGIN
    fnName = 'pgr_extractVertices';
    RAISE DEBUG 'PROCESSING: %(''%'')', fnName, edges_sql;

    BEGIN
        quoted = '.*' || $1 || '.*AS';
        query = format($$
            SELECT regexp_replace(regexp_replace(statement, %1$L,'','i'),';$','') FROM pg_prepared_statements WHERE name = %2$L$$,
            quoted, $1);
        EXECUTE query INTO edges_sql;

        EXCEPTION WHEN OTHERS
            THEN edges_sql := $1;
    END;

    IF edges_SQL IS NULL THEN
        edges_sql := $1;
    END IF;

    BEGIN
        query = 'SELECT the_geom FROM ('||edges_sql||' ) AS __a__ limit 1';
        EXECUTE query;

        EXCEPTION WHEN OTHERS THEN
            has_geom := FALSE;
    END;

    IF has_geom THEN
        query := $q$
        SELECT DISTINCT source, ST_X(geom), ST_Y(geom), geom FROM (
            SELECT source, ST_startpoint(the_geom) AS geom
            FROM ( $q$ || edges_sql || $q$) AS __a__

            UNION
            SELECT target, ST_endpoint(the_geom)
            FROM ( $q$ || edges_sql || $q$) AS __b__ ) AS __c__ $q$;
    ELSE
        query := $q$
        SELECT DISTINCT source, NULL::FLOAT, NULL::FLOAT, NULL::geometry FROM (
            SELECT source
            FROM ( $q$ || edges_sql || $q$) AS a

            UNION
            SELECT target
            FROM ( $q$ || edges_sql || $q$) AS b ) AS c $q$;
    END IF;

    RAISE DEBUG 'query %', query;
    RETURN QUERY EXECUTE query;

    EXCEPTION WHEN OTHERS THEN
    RAISE EXCEPTION '%', SQLERRM
        USING HINT = 'Please check query: '|| $1;

END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;


-- COMMENTS


COMMENT ON FUNCTION pgr_extractVertices(TEXT)
IS 'pgr_extractVertices
- PROPOSED
- Parameters
  - Edges SQL with columns: source, target [, the_geom]
- Documentation:
  - ${PGROUTING_DOC_LINK}/pgr_extractVertices.html
';
