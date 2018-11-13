/*PGR-GNU*****************************************************************

Copyright (c) 2015 Celia Virginia Vergara Castillo
Copyright (c) 2006-2007 Anton A. Patrushev, Orkney, Inc.
Copyright (c) 2005 Sylvain Pasche,
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
	$$
	DECLARE
		r record;
		geoms geometry[];
		vertex_result record;
		i int;
		n int;
		spos int;
		q TEXT;
		x FLOAT8[];
		y FLOAT8[];

	BEGIN
		geoms := array[]::geometry[];
		i := 1;

		FOR vertex_result IN EXECUTE 'SELECT x, y FROM pgr_alphashape('''|| $1 || ''', ' || alpha || ')'
		LOOP
			x[i] = vertex_result.x;
			y[i] = vertex_result.y;
			i := i+1;
		END LOOP;

		n := i;
		IF n = 1 THEN
			RAISE NOTICE 'n = 1';
			RETURN NULL;
		END IF;

		spos := 1;
		q := 'SELECT ST_GeometryFromTEXT(''POLYGON((';
		FOR i IN 1..n LOOP
			IF x[i] IS NULL AND y[i] IS NULL THEN
				q := q || ', ' || x[spos] || ' ' || y[spos] || '))'',0) AS geom;';
				EXECUTE q INTO r;
				geoms := geoms || array[r.geom];
				q := '';
			ELSE
				IF q = '' THEN
					spos := i;
					q := 'SELECT ST_GeometryFromTEXT(''POLYGON((';
				END IF;
				IF i = spos THEN
					q := q || x[spos] || ' ' || y[spos];
				ELSE
					q := q || ', ' || x[i] || ' ' || y[i];
				END IF;
			END IF;
		END LOOP;

		RETURN ST_BuildArea(ST_Collect(geoms));
	END;
	$$
	LANGUAGE 'plpgsql' VOLATILE STRICT;

COMMENT ON FUNCTION pgr_pointsAsPolygon(VARCHAR, FLOAT8)
IS 'pgr_pointsAsPolygon
- Parameters
	- An SQL with columns: id, x, y 
- Optional Parameters
	- alpha := 0
- Documentation:
    - ${PGROUTING_DOC_LINK}/pgr_pointsAsPolygon.html
';