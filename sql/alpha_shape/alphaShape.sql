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
    geometry, -- geometry
    alpha FLOAT DEFAULT 0
)
RETURNS geometry AS
$BODY$

DECLARE
geom      geometry;
delauny_query   TEXT;

BEGIN
    delauny_query = format($$
        WITH
        original AS (
            SELECT %1$L::geometry AS geom
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
        $$, $1);

    --RAISE NOTICE '%', delauny_query;
    -- RETURN;

    WITH a AS (SELECT 'GEOMETRYCOLLECTION(' || string_agg(textgeom,',') || ')' as geome
        FROM _pgr_alphaShape(delauny_query, $2))
    SELECT ST_GeomFromText(geome) FROM a
    INTO geom;
    RETURN geom;

END

$BODY$
LANGUAGE plpgsql VOLATILE STRICT
COST 100;

COMMENT ON FUNCTION pgr_alphashape(geometry, FLOAT)
IS 'pgr_alphaShape
- Parameters
	- An SQL with columns: geom
- Optional Parameters
	- alpha := 0
- Documentation:
    - ${PGROUTING_DOC_LINK}/pgr_alphashape.html
';
