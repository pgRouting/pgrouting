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
create or replace function pgr_pointstodmatrix(pnts geometry[], mode integer default (0), OUT dmatrix double precision[], OUT ids integer[])
    returns record as
$body$
/*
 *  pgr_pointstodmatrix(pnts geometry[], OUT dmatrix double precision[], OUT ids integer[])
 *
 *  Create a distance symmetric distance matrix suitable for TSP using Euclidean distances
 *  based on the st_distance(). You might want to create a variant of this the uses st_distance_sphere()
 *  or st_distance_spheriod() or some other function.
 *
*/
declare
    r record;

begin
    RAISE NOTICE 'Deprecated function pgr_pointsToDMatrix';
    dmatrix := array[]::double precision[];
    ids := array[]::integer[];

    -- create an id for each point in the array and unnest it into a table nodes in the with clause
    for r in with nodes as (select row_number() over()::integer as id, p from (select unnest(pnts) as p) as foo)
        -- compute a row of distances
        select i, array_agg(dist) as arow from (
            select a.id as i, b.id as j,
                case when mode=0
                    then st_distance(a.p, b.p)
                    else st_distance_sphere(a.p, b.p)
                end as dist
              from nodes a, nodes b
             order by a.id, b.id
           ) as foo group by i order by i loop

        -- you must concat an array[array[]] to make dmatrix[][]
        -- concat the row of distances to the dmatrix
        dmatrix := array_cat(dmatrix, array[r.arow]);
        ids := ids || array[r.i];
    end loop;
end;
$body$
    language plpgsql stable;


------------------------------------------------------------------------------

create or replace function pgr_vidstodmatrix(IN vids integer[], IN pnts geometry[], IN edges text, tol float8 DEFAULT(0.1), OUT dmatrix double precision[], OUT ids integer[])
    returns record as
$body$
/*
 *  pgr_vidstodmatrix(IN vids integer[], IN pnts geometry[], IN edges text, tol float8 DEFAULT(0.1),
 *                    OUT dmatrix double precision[], OUT ids integer[])
 *
 *  This function that's an array vertex ids, the original array of points, the edge table name and a tol.
 *  It then computes kdijkstra() distances for each vertex to all the other vertices and creates a symmetric
 *  distances matrix suitable for TSP. The pnt array and the tol are used to establish a BBOX for limiteding
 *  selection of edges.the extents of the points is expanded by tol.
 *
 *  NOTES:
 *  1. we compute a symmetric matrix because TSP requires that so the distances are better the Euclidean but
 *     but are not perfect
 *  2. kdijkstra() can fail to find a path between some of the vertex ids. We to not detect this other than
 *     the cost might get set to -1.0, so the dmatrix should be checked for this as it makes it invalid for TSP
 *
*/
declare
    i integer;
    j integer;
    nn integer;
    rr record;
    bbox geometry;
    t float8[];

begin
    RAISE NOTICE 'Deprecated function pgr_vidsToDMatrix';
    -- check if the input arrays has any -1 values, maybe this whould be a raise exception
    if vids @> ARRAY[-1] then
    raise notice 'Some vids are undefined (-1)!';
    dmatrix := null;
    ids := null;
    return;
    end if;

    ids := vids;

    -- get the count of nodes
    nn := array_length(vids,1);

    -- zero out a dummy row
    for i in 1 .. nn loop
        t := t || 0.0::float8;
    end loop;

    -- using the dummy row, zero out the whole matrix
    for i in 1 .. nn loop
    dmatrix := dmatrix || ARRAY[t];
    end loop;

    for i in 1 .. nn-1 loop
        j := i;
        -- compute the bbox for the point needed for this row
        select st_expand(st_collect(pnts[id]), tol) into bbox
          from (select generate_series as id from generate_series(i, nn)) as foo;

        -- compute kdijkstra() for this row
        for rr in execute 'select * from pgr_dijkstracost($1, $2, $3, false)'
                  using 'select id, source, target, cost from ' || edges ||
                        ' where the_geom && ''' || bbox::text || '''::geometry'::text, vids[i], vids[i+1:nn] loop

            -- TODO need to check that all node were reachable from source
            -- I think unreachable paths between nodes returns cost=-1.0

            -- populate the matrix with the cost values, remember this is symmetric
            j := j + 1;
            -- raise notice 'cost(%,%)=%', i, j, rr.agg_cost;
            dmatrix[i][j] := rr.agg_cost;
            dmatrix[j][i] := rr.agg_cost;
        end loop;
    end loop;

end;
$body$
    language plpgsql stable cost 200;

