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
create or replace function pgr_pointtoedgenode(edges text, pnt geometry, tol float8)
    returns integer as
$body$
/*
 *  pgr_pointtoedgenode(edges text, pnt geometry, tol float8)
 *
 *  Given and table of edges with a spatial index on the_geom
 *  and a point geometry search for the closest edge within tol distance to the edges
 *  then compute the projection of the point onto the line segment and select source or target
 *  based on whether the projected point is closer to the respective end and return source or target.
 *  If no edge is within tol distance then return -1
*/
declare
    rr record;
    pct float;
    debuglevel text;

begin
    -- find the closest edge within tol distance
    execute 'select * from ' || _pgr_quote_ident(edges) ||
            ' where st_dwithin(''' || pnt::text ||
            '''::geometry, the_geom, ' || tol || ') order by st_distance(''' || pnt::text ||
            '''::geometry, the_geom) asc limit 1' into rr;

    if rr.the_geom is not null then
        -- deal with MULTILINESTRINGS
        if geometrytype(rr.the_geom)='MULTILINESTRING' THEN
            rr.the_geom := ST_GeometryN(rr.the_geom, 1);
        end if;

        -- project the point onto the linestring
        execute 'show client_min_messages' into debuglevel;
        SET client_min_messages='ERROR';
        pct := st_line_locate_point(rr.the_geom, pnt);
        execute 'set client_min_messages  to '|| debuglevel;

        -- return the node we are closer to
        if pct < 0.5 then
            return rr.source;
        else
            return rr.target;
        end if;
    else
        -- return a failure to find an edge within tol distance
        return -1;
    end if;
end;
$body$
  language plpgsql volatile
  cost 5;


----------------------------------------------------------------------------

create or replace function pgr_flipedges(ga geometry[])
    returns geometry[] as
$body$
/*
 *  pgr_flipedges(ga geometry[])
 *
 *  Given an array of linestrings that are supposedly connected end to end like the results
 *  of a route, check the edges and flip any end for end if they do not connect with the
 *  previous seegment and return the array with the segments flipped as appropriate.
 *
 *  NOTE: no error checking is done for conditions like adjacent edges are not connected.
*/
declare
    nn integer;
    i integer;
    g geometry;

begin
    RAISE NOTICE 'Deperecated function: pgr_flipEdges';
    -- get the count of edges, and return if only one edge
    nn := array_length(ga, 1);
    if nn=1 then
        return ga;
    end if;

    -- determine if first needs to be flipped
    g := _pgr_startpoint(ga[1]);

    -- if the start of the first is connected to the second then it needs to be flipped
    if _pgr_startpoint(ga[2])=g or _pgr_endpoint(ga[2])=g then
        ga[1] := st_reverse(ga[1]);
    end if;
    g := _pgr_endpoint(ga[1]);

    -- now if  the end of the last edge matchs the end of the current edge we need to flip it
    for i in 2 .. nn loop
        if _pgr_endpoint(ga[i])=g then
            ga[i] := st_reverse(ga[i]);
        end if;
        -- save the end of this edge into the last end for the next cycle
        g := _pgr_endpoint(ga[i]);
    end loop;

    return ga;
end;
$body$
    language plpgsql immutable;


------------------------------------------------------------------------------

create or replace function pgr_texttopoints(pnts text, srid integer DEFAULT(4326))
    returns geometry[] as
$body$
/*
 *  pgr_texttopoints(pnts text, srid integer DEFAULT(4326))
 *
 *  Given a text string of the format "x,y;x,y;x,y;..." and the srid to use,
 *  split the string and create and array point geometries
*/
declare
    a text[];
    t text;
    p geometry;
    g geometry[];

begin
    RAISE NOTICE 'Deperecated function: pgr_textToPoints';
    -- convert commas to space and split on ';'
    a := string_to_array(replace(pnts, ',', ' '), ';');
    -- convert each 'x y' into a point geometry and concattenate into a new array
    for t in select unnest(a) loop
        p := st_pointfromtext('POINT(' || t || ')', srid);
        g := g || p;
    end loop;

    return g;
end;
$body$
    language plpgsql immutable;

-----------------------------------------------------------------------

create or replace function pgr_pointstovids(pnts geometry[], edges text, tol float8 DEFAULT(0.01))
    returns integer[] as
$body$
/*
 *  pgr_pointstovids(pnts geometry[], edges text, tol float8 DEFAULT(0.01))
 *
 *  Given an array of point geometries and an edge table and a max search tol distance
 *  convert points into vertex ids using pgr_pointtoedgenode()
 *
 *  NOTE: You need to check the results for any vids=-1 which indicates if failed to locate an edge
*/
declare
    v integer[];
    g geometry;

begin
    RAISE NOTICE 'Deperecated function: pgr_pointsToVids';
    -- cycle through each point and locate the nearest edge and vertex on that edge
    for g in select unnest(pnts) loop
        v := v || pgr_pointtoedgenode(edges, g, tol);
    end loop;

    return v;
end;
$body$
    language plpgsql stable;

