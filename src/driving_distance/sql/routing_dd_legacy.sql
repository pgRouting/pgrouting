/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
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
-- Calculates the driving distance.
--
-- A delta-sized bounding box around the start is used for data clipping.
--
-- table_name        the table name to work on
-- x                 start x
-- x                 start y
-- distance          the max. cost
-- delta             delta for data clipping
-- cost              cost
-- reverse_cost      reverse_cost
-- directed          is graph directed
-- has_reverse_cost  use reverse_cost column
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION pgr_drivingDistance(
    table_name varchar, 
    x double precision, 
    y double precision,
    distance double precision,
    delta double precision,
    cost varchar,
    reverse_cost varchar,
    directed boolean,
    has_reverse_cost boolean)
RETURNS SETOF pgr_geomResult AS
$$
DECLARE
    q text;
    srid integer;
    r record;
    geom pgr_geomResult;
BEGIN
     
    EXECUTE 'SELECT srid FROM geometry_columns WHERE f_table_name = ''' ||
        table_name || '''' INTO r;
    srid := r.srid;
     
    -- RAISE NOTICE 'SRID: %', srid;

    q := 'SELECT * FROM pgr_pointsAsPolygon(''SELECT a.id1::integer AS id, b.x1::double precision AS x, b.y1::double precision AS y FROM pgr_drivingDistance(''''''''SELECT gid AS id,source::integer,target::integer, ' || cost || '::double precision AS cost, ' || reverse_cost || '::double precision as reverse_cost FROM ' || table_name || ' WHERE ST_SetSRID(''''''''''''''''BOX3D(' || x-delta || ' ' || y-delta || ', ' || x+delta || ' ' || y+delta || ')''''''''''''''''::BOX3D, ' || srid || ') && the_geom  '''''''', (SELECT id FROM pgr_findNodeByNearestLinkDwithin(''''''''POINT(' || x || ' ' || y || ')'''''''',' || distance/10 || ',''''''''' || table_name || ''''''''')),' || distance || ',true, true) a, (SELECT * FROM ' || table_name || ' WHERE ST_SetSRID(''''''''BOX3D(' || x-delta || ' ' || y-delta || ', ' || x+delta || ' ' || y+delta || ')''''''''::BOX3D, ' || srid || ') && the_geom) b WHERE a.id1 = b.source'')';

    -- RAISE NOTICE 'Query: %', q;
     
     EXECUTE q INTO r;
     geom.seq  := r.seq;
     geom.id1  := r.id1;
     geom.id2  := r.id2;
     geom.geom := r.geom;
     RETURN NEXT geom;
     
     RETURN;

END;
$$

LANGUAGE 'plpgsql' VOLATILE STRICT;



-----------------------------------------------------------------------
-- Calculates the driving distance.
--
-- A delta-sized bounding box around the start is used for data clipping.
--
-- This function differs from the pgr_drivingDistance in that the signature
-- is now similar to the shortest path delta functions and the delta is
-- passed as argument.
--
-- If you're accustomed to the shortest path delta functions, you probably
-- want to use this as the preferred way to get the driving distance.
--
-- table_name        the table name to work on
-- source_id         start id
-- distance          the max. cost
-- delta             delta for data clipping
-- directed          is graph directed
-- has_reverse_cost  use reverse_cost column
-----------------------------------------------------------------------
CREATE OR REPLACE FUNCTION pgr_drivingDistance(
    table_name varchar,
    source_id integer,
	distance double precision,
    delta float8,
    directed boolean,
    has_reverse_cost boolean)
RETURNS SETOF pgr_geomResult AS
$$
DECLARE
    q text;
    srid integer;
    r record;
    geom pgr_geomResult;
    source_x float8;
    source_y float8;
BEGIN

    EXECUTE 'select ST_X(PGR_StartPoint(the_geom)) as source_x from ' ||
            pgr_quote_ident(table_name) || ' where source = ' ||
            source_id || ' limit 1' INTO r;
    source_x := r.source_x;


    EXECUTE 'select ST_Y(PGR_StartPoint(the_geom)) as source_y from ' ||
            pgr_quote_ident(table_name) || ' where source = ' ||
            source_id || ' limit 1' INTO r;
    source_y := r.source_y;

    RETURN QUERY select * from pgr_drivingDistance(table_name,
        source_x, source_y, distance, delta, directed, has_reverse_cost);

END;
$$

LANGUAGE 'plpgsql' VOLATILE STRICT;

