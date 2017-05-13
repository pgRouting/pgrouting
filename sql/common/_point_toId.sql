/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Author: Stephen Woodbridge <woodbri@imaptools.com>
Author: Vicky Vergara <vicky_vergara@hotmail,com>
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
/*
.. function:: _pgr_pointToId(point geometry, tolerance double precision,vname text,srid integer)
Using tolerance to determine if its an existing point:
    - Inserts a point into the vertices table "vertname" with the srid "srid",
and returns
    - the id of the new point
    - the id of the existing point.

Tolerance is the minimal distance between existing points and the new point to create a new point.

Last changes: 2013-03-22

HISTORY
Last changes: 2013-03-22
2013-08-19: handling schemas
*/

CREATE OR REPLACE FUNCTION _pgr_pointToId(
    point geometry,
    tolerance double precision,
    vertname text,
    srid integer)

  RETURNS bigint AS
$BODY$
DECLARE
    rec record;
    pid bigint;

BEGIN
    EXECUTE 'SELECT ST_Distance(
        the_geom,
        ST_GeomFromText(ST_AsText('
                || quote_literal(point::text)
                || '),'
            || srid ||')) AS d, id, the_geom
    FROM '||_pgr_quote_ident(vertname)||'
    WHERE ST_DWithin(
        the_geom,
        ST_GeomFromText(
            ST_AsText(' || quote_literal(point::text) ||'),
            ' || srid || '),' || tolerance||')
    ORDER BY d
    LIMIT 1' INTO rec ;
    IF rec.id IS NOT NULL THEN
        pid := rec.id;
    ELSE
        execute 'INSERT INTO '||_pgr_quote_ident(vertname)||' (the_geom) VALUES ('||quote_literal(point::text)||')';
        pid := lastval();
END IF;

RETURN pid;

END;
$BODY$
LANGUAGE plpgsql VOLATILE STRICT;

