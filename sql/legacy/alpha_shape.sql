/*PGR-GNU*****************************************************************

Copyright (c) 2019 Celia Virginia Vergara Castillo
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

-----------------------------------------------------------------------
-- legacy
-- The sql should return vertex x,y values.
   -- Before: id is compulsory
   -- Legacy: id is ignored
-- Return ordered vertex ids.
   -- Before: the sequence was does not represent a closed linestring
   -- Legacy: the sequence was represent a closed linestring
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION pgr_alphashape(
    text, -- SQL with x, y
    alpha float8 DEFAULT 0,
    OUT x float8,
    OUT y float8)
RETURNS SETOF record
AS
$BODY$
DECLARE
    rec record;
    old_poly_seq BIGINT = 1;
    old_ring     BIGINT = 1;
BEGIN
    FOR rec IN
        WITH
        multiPoly AS (SELECT pgr_pointsAsPolygon($1, sqrt(alpha)) AS geom),
        singlePoly AS (SELECT (ST_dump(geom)).path[1] AS poly_seq, (ST_dump(geom)).geom FROM multipoly),
        polydump AS (SELECT poly_seq, ST_DumpPoints(geom) dp FROM singlePoly)
        SELECT poly_seq, (dp).path[1] AS ring, (dp).path[2] AS point_seq, st_asText((dp).geom) AS geom
        FROM polydump
        ORDER BY poly_seq, ring, point_seq
    LOOP
        IF (old_poly_seq != rec.poly_seq OR old_ring != rec.ring) THEN
            RAISE NOTICE '% %', rec.poly_seq, rec.ring;
            x = NULL;
            y = NULL;
            RETURN NEXT;
            old_poly_seq := rec.poly_seq;
            old_ring := rec.ring;
        END IF;

        x = ST_X(rec.geom);
        y = ST_Y(rec.geom);

        RETURN NEXT;
    END LOOP;
END
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

COMMENT ON FUNCTION pgr_alphaShape(TEXT, FLOAT)
IS 'Legacy';
