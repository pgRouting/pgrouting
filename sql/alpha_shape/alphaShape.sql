/*PGR-GNU*****************************************************************

Copyright (c) 2018 Celia Virginia Vergara Castillo
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
--------------
--------------
-- alpha_shape
--------------
--------------

CREATE OR REPLACE FUNCTION pgr_alphaShape(
    geometry[], -- geometry
    alpha FLOAT DEFAULT 0,
    is_delauny BOOLEAN DEFAULT false,

    OUT geom geometry)
AS
$BODY$

DECLARE
info_query      TEXT;
delauny_query   TEXT;
results RECORD;

BEGIN
    info_query = $$
        SELECT
            id,
            seq AS source,
            -1  AS target,
            1 AS cost,
            ST_X(geom)::FLOAT AS x1,
            ST_Y(geom)::FLOAT AS y1,
            0::FLOAT AS x2,
            0::FLOAT AS y2
        FROM delauny_info WHERE seq != 4;

        $$;
        /*
        foo AS (
            SELECT id, seq, source, geom
            FROM the_points
            JOIN delauny_info USING (geom)
            WHERE seq != 4
        )
        SELECT
            id,
            one.source,
            two.source as target,
            1 AS cost,
            ST_X(one.geom)::FLOAT AS x1,
            ST_Y(one.geom)::FLOAT AS y1,
            ST_X(two.geom)::FLOAT AS x2,
            ST_Y(two.geom)::FLOAT AS y2
        FROM foo AS one JOIN foo as two USING(id)
        WHERE one.source < two.source;
*/
    if is_delauny THEN
        -- TODO check the geometries are polygons
        -- TODO check the polygons have 3 points
        delauny_query = format($$
            WITH
            original_data AS (
                SELECT unnest(%1$L::geometry[]) AS geom
            ),
            data AS (
                SELECT row_number() over() AS id, geom FROM original_data
            ),
            delauny_info AS (
                SELECT id,
                (ST_DumpPoints(geom)).path[2] as seq,
                (ST_DumpPoints(geom)).geom
                FROM data
            ),
            the_unique_points AS (
                SELECT DISTINCT(geom) FROM delauny_info
            ),
            the_points AS (SELECT row_number() over() AS source, geom
                FROM the_unique_points
            ),
            %2$s
            $$, $1, info_query);

    ELSE
            --SELECT DISTINCT (ST_DumpPoints(ST_Collect(%1$L::geometry[]))).geom
    delauny_query = format($$
        WITH
        original AS (
            SELECT ST_Collect(%1$L::geometry[]) AS geom
        ),
        the_unique_points AS (
            SELECT DISTINCT (ST_DumpPoints(geom)).geom FROM original
        ),
        the_points AS (SELECT row_number() over() AS source, geom
            FROM the_unique_points
        ),
        delauny AS (
            SELECT (ST_Dump(ST_DelaunayTriangles(geom, 0 , 0))).*
                FROM original
        ),
        delauny_info AS (
            SELECT delauny.path[1] AS id,
            (ST_DumpPoints(delauny.geom)).path[2] as seq,
            (ST_DumpPoints(delauny.geom)).geom
            FROM delauny
        )
        %2$s
        $$, $1, info_query);

    END IF;

    /*
    FOR results IN SELECT textgeom
        FROM _pgr_alphaShape(delauny_query, $2)
        WHERE NOT ST_isValid(ST_GeomFromText(textgeom))
    LOOP
        RAISE WARNING '%', results.textgeom;
    END LOOP;

    FOR results IN WITH a AS (SELECT 'GEOMETRYCOLLECTION(' || string_agg(textgeom,',') || ')' as geome
        FROM _pgr_alphaShape(delauny_query, $2))
        SELECT geome FROM a
        WHERE ST_isValid(ST_GeomFromText(geome))
    LOOP
        RAISE WARNING '%', results.geome;
    END LOOP;
    */


    --RAISE NOTICE '%', delauny_query;
    -- RETURN;

    WITH a AS (SELECT 'GEOMETRYCOLLECTION(' || string_agg(textgeom,',') || ')' as geome
        FROM _pgr_alphaShape(delauny_query, $2))
    SELECT ST_GeomFromText(geome) FROM a
    INTO geom;

END

$BODY$
LANGUAGE plpgsql VOLATILE STRICT
COST 100;


COMMENT ON FUNCTION pgr_alphashape(geometry[], FLOAT, BOOLEAN)
IS 'pgr_alphaShape
- Parameters
	- An SQL with columns: geom
- Optional Parameters
	- alpha := 0
- Documentation:
    - ${PGROUTING_DOC_LINK}/pgr_alphashape.html
';

CREATE OR REPLACE FUNCTION pgr_alphaShape(
    geometry, -- geometry
    alpha FLOAT DEFAULT 0,
    is_delauny BOOLEAN DEFAULT false,

    OUT geom geometry)
AS
$BODY$

DECLARE
info_query      TEXT;
delauny_query   TEXT;

BEGIN
    info_query = $$
        SELECT
            id,
            seq AS source,
            -1  AS target,
            1 AS cost,
            ST_X(geom)::FLOAT AS x1,
            ST_Y(geom)::FLOAT AS y1,
            0::FLOAT AS x2,
            0::FLOAT AS y2
        FROM delauny_info WHERE seq != 4;

        $$;
        /*
        foo AS (
            SELECT id, seq, source, geom
            FROM the_points
            JOIN delauny_info USING (geom)
            WHERE seq != 4
        )
        SELECT
            id,
            one.source,
            two.source as target,
            1 AS cost,
            ST_X(one.geom)::FLOAT AS x1,
            ST_Y(one.geom)::FLOAT AS y1,
            ST_X(two.geom)::FLOAT AS x2,
            ST_Y(two.geom)::FLOAT AS y2
        FROM foo AS one JOIN foo as two USING(id)
        WHERE one.source < two.source;
*/
    if is_delauny THEN
        -- TODO check the geometries are polygons
        -- TODO check the polygons have 3 points
        delauny_query = format($$
            WITH
            original_data AS (
                SELECT unnest(%1$L::geometry[]) AS geom
            ),
            data AS (
                SELECT row_number() over() AS id, geom FROM original_data
            ),
            delauny_info AS (
                SELECT id,
                (ST_DumpPoints(geom)).path[2] as seq,
                (ST_DumpPoints(geom)).geom
                FROM data
            ),
            the_unique_points AS (
                SELECT DISTINCT(geom) FROM delauny_info
            ),
            the_points AS (SELECT row_number() over() AS source, geom
                FROM the_unique_points
            ),
            %2$s
            $$, $1, info_query);

    ELSE
            --SELECT DISTINCT (ST_DumpPoints(ST_Collect(%1$L::geometry[]))).geom
    delauny_query = format($$
        WITH
        original AS (
            SELECT %1$L::geometry AS geom
        ),
        the_unique_points AS (
            SELECT DISTINCT (ST_DumpPoints(geom)).geom FROM original
        ),
        the_points AS (SELECT row_number() over() AS source, geom
            FROM the_unique_points
        ),
        delauny AS (
            SELECT (ST_Dump(ST_DelaunayTriangles(geom, 0 , 0))).*
                FROM original
        ),
        delauny_info AS (
            SELECT delauny.path[1] AS id,
            (ST_DumpPoints(delauny.geom)).path[2] as seq,
            (ST_DumpPoints(delauny.geom)).geom
            FROM delauny
        )
        %2$s
        $$, $1, info_query);

    END IF;

    --RAISE NOTICE '%', delauny_query;
    -- RETURN;

    WITH a AS (SELECT 'GEOMETRYCOLLECTION(' || string_agg(textgeom,',') || ')' as geome
        FROM _pgr_alphaShape(delauny_query, $2))
    SELECT ST_GeomFromText(geome) FROM a
    INTO geom;

END

$BODY$
LANGUAGE plpgsql VOLATILE STRICT
COST 100;

COMMENT ON FUNCTION pgr_alphashape(geometry, FLOAT, BOOLEAN)
IS 'pgr_alphaShape
- Parameters
	- An SQL with columns: geom
- Optional Parameters
	- alpha := 0
- Documentation:
    - ${PGROUTING_DOC_LINK}/pgr_alphashape.html
';
