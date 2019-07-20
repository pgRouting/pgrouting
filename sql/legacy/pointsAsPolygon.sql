/*PGR-GNU*****************************************************************

Copyright (c) 2015~2019 Celia Virginia Vergara Castillo
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

----------------------
-- pgr_pointsAsPolygon
----------------------


/*
----------------------------------------------------------
-- Draws an alpha shape around given set of points.
-- ** This should be rewritten as an aggregate. **
----------------------------------------------------------
*/
CREATE OR REPLACE FUNCTION pgr_pointsAsPolygon(
    VARCHAR, -- query (required)
    alpha FLOAT8 DEFAULT 0)

RETURNS geometry AS
$BODY$
DECLARE
    geoms geometry[];
    vertex_result record;
    i BIGINT;
    q TEXT;

BEGIN
    q = format($$
        WITH
        a AS (
            %1$s
        ),
        b AS (SELECT ST_makePoint(x, y) AS geom FROM a)
        SELECT array_agg(geom) FROM b
        $$, $1);
    -- RAISE NOTICE '%', q;
    EXECUTE q INTO geoms;

    -- RAISE NOTICE 'length = %', array_length(geoms, 1);
    RETURN pgr_alphashape1(geoms, sqrt(alpha), false);
END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

COMMENT ON FUNCTION pgr_pointsAsPolygon(VARCHAR, FLOAT8)
IS 'pgr_pointsAsPolygon
- Parameters:
    - An SQL with columns: id, x, y
- Optional Parameters:
    - alpha := 0
- Documentation:
    - ${PGROUTING_DOC_LINK}/pgr_pointsAsPolygon.html
';
