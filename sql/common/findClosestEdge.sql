/*PGR-GNU*****************************************************************

Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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
 *  pgr_findClosestEdge(edges_sql text, pnt geometry, tol float8)
 *
 *  Given and table of edges with a spatial index on the_geom
 *  and a point geometry search for the closest edge within tol distance to the edges
 *  then compute the projection of the point onto the line segment and select source or target
 *  based on whether the projected point is closer to the respective end and return source or target.
 *  If no edge is within tol distance then return -1
  IF az > pi()/2 AND az < pi() OR az > 3 * pi()/2 THEN - then to the right
*/

CREATE OR REPLACE FUNCTION pgr_findClosestEdge(
    edges_sql text,
    point_geom geometry,
    tolerance float8,

    OUT edge_id BIGINT,
    OUT fraction FLOAT,
    OUT side CHAR)
    returns SETOF RECORD AS
$body$
DECLARE
    safe_sql text;
    sql text;
    rec RECORD;
BEGIN

    IF (tolerance < 0 ) THEN
        RAISE EXCEPTION 'Invalid value for tolerance';
    END IF;

    safe_sql =  'SELECT * from ('|| edges_sql ||' ) AS __a__';

    BEGIN
        EXECUTE safe_sql || ' LIMIT 1' into rec;
        EXCEPTION WHEN OTHERS THEN
            RAISE EXCEPTION 'Could not execute query please verify syntax of: '
            USING HINT = edges_sql;
    END;

    BEGIN
        EXECUTE 'SELECT id, the_geom  from ('||safe_sql||') as __b__' into rec;
        EXCEPTION
            WHEN OTHERS THEN
                RAISE EXCEPTION 'An expected column was not found in the query'
                  USING HINT = 'Please veryfy the column names: id, the_geom';
    END;

    sql ='
    WITH
    safesql AS (' || safe_sql || '),
    close_ones AS
    (SELECT id, the_geom AS line,
        ST_Distance(the_geom,' || quote_literal(point_geom::TEXT) ||'::geometry ) AS distance,
        ST_ClosestPoint(the_geom,' || quote_literal(point_geom::TEXT) || '::geometry) AS blade
        FROM  safesql
        WHERE ST_DWithin(the_geom,' || quote_literal(point_geom::TEXT) ||'::geometry, ' || tolerance || ')),
    closest_ones AS
    (SELECT min(distance) min_dist FROM close_ones),
    third_q AS
    (SELECT DISTINCT(id) id, distance, line, blade FROM close_ones WHERE distance = (select min_dist from closest_ones) ORDER BY id LIMIT 1),
    dump_values AS
    (SELECT id,
        (ST_length(ST_GeometryN(st_split(line, blade),1))/ST_length(line))::FLOAT AS fraction,
        (line << ' || quote_literal(point_geom::TEXT) || '::geometry) as rightSide
        FROM third_q)
    SELECT id::BIGINT,
           fraction::FLOAT,
           (CASE WHEN rightSide THEN ''r'' ELSE ''l'' END)::char FROM dump_values';

    RETURN query EXECUTE sql;

END;
$body$
  language plpgsql volatile
  cost 5;

