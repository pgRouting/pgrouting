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
\set ECHO none
\set QUIET 1
-- Turn off echo and keep things quiet.

-- Format the output for nice TAP.
\pset format unaligned
\pset tuples_only true
\pset pager
\set VERBOSITY terse

-- Revert all changes on failure.
\set ON_ERROR_ROLLBACK true
\set ON_ERROR_STOP true
\set QUIET 1

BEGIN;
    SELECT plan(3);


    create table kdgrid (
        gid serial not null primary key,
        source integer,
        target integer,
        cost float8
    );

    insert into kdgrid (source, target, cost)
    select source, target, cost from
    (
        (with i as (select generate_series(0,24,1) as n),
            j as (select generate_series(1,24,1) as n)
            select (i.n*24 + i.n + j.n) as source, (i.n*24 + i.n + j.n + 1) as target, 1.0::float8 as cost,
            j.n as x1, i.n as y1, j.n+1 as x2, i.n as y2
            from i, j)
        union all
        (with u as (select generate_series(0,24,1) as n),
            v as (select generate_series(1,25,1) as n)
            select (u.n*24 + u.n + v.n) as source, (u.n*24 + u.n + v.n + 25) as target, 1.0::float8 as cost,
            v.n as x1, u.n as y1, v.n as x2, u.n+25 as y2
            from u, v where u.n < 24)
    ) as foo;

    PREPARE q1 AS
    select id1, id2, cost from pgr_kdijkstracost('select gid as id, source, target, cost from kdgrid', 288, array[1,25,601,625], false, false);

    PREPARE q2 AS
    SELECT start_vid::INTEGER, end_vid::INTEGER, agg_cost
    FROM pgr_dijkstracost(
        'select gid as id, source, target, cost from kdgrid',
        288, array[1,25,601,625],
        false);
    SELECT set_has('q1','q2','comparing vs pgr_dijkstracost');



    PREPARE q3 AS
    select id1, id2, id3, cost from pgr_kdijkstrapath('select gid as id, source, target, cost from kdgrid', 288, array[1,25,601,625], false, false);

    PREPARE q4 AS
    SELECT end_vid::INTEGER, node::INTEGER, edge::INTEGER, cost
    FROM pgr_dijkstra(
        'select gid as id, source, target, cost from kdgrid',
        288, array[1,25,601,625],
        false);
    SELECT set_has('q3','q4','comparing vs pgr_dijkstracost');


    update kdgrid set cost=-1.0 where  source=2 and target=3;
    update kdgrid set cost=-1.0 where  source=12 and target=13;
    update kdgrid set cost=-1.0 where  source=22 and target=23;
    update kdgrid set cost=-1.0 where  source=32 and target=33;
    update kdgrid set cost=-1.0 where  source=42 and target=43;

    PREPARE q5 AS
    select id1, id2, id3, cost from pgr_kdijkstrapath('select gid as id, source, target, cost from kdgrid', 288, array[1,25,601,625], false, false);

    PREPARE q6 AS
    SELECT end_vid::INTEGER, node::INTEGER, edge::INTEGER, cost
    FROM pgr_dijkstra(
        'select gid as id, source, target, cost from kdgrid',
        288, array[1,25,601,625],
        false);
    SELECT set_has('q5','q6','comparing vs pgr_dijkstracost');


    SELECT * FROM FINISH();
    ROLLBACK
