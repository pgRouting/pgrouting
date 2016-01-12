-- each ring will increas by 4 because it is on a square grid
-- to find the start node number
-- select *, st_distance(st_makepoint(25,25), the_geom) from vertices_tmp where st_dwithin(st_makepoint(25,25), the_geom, 1.0) order by st_distance(st_makepoint(25,25), the_geom) limit 1;
BEGIN;

    -- create 50x50 grid of overlapping lines horizontal and vertical


    SET client_min_messages TO WARNING;

    create table ddunnoded (
        id serial not null primary key
    );

    SELECT  addgeometrycolumn('ddunnoded', 'the_geom', ST_SRID('POINT(0 0)'::geometry), 'LINESTRING', 2);

    INSERT into ddunnoded (the_geom)
    select st_astext(st_makeline(st_makepoint(0,s1), st_makepoint(51,s1))) from (select generate_series(1,50) as s1) as foo
    union all
    select st_astext(st_makeline(st_makepoint(s1,0), st_makepoint(s1,51))) from (select generate_series(1,50) as s1) as foo;

    -- node the grid so we can use it
    select pgr_nodenetwork('ddunnoded',0.000001);

    -- copy the noded table into a table we can use for a graph
    -- and add the required columns


    create table ddnoded2 (
        gid serial not null primary key,
        id integer,
        source integer,
        target integer,
        cost float8
    );

    select addgeometrycolumn('ddnoded2', 'the_geom', ST_SRID('POINT(0 0)'::geometry), 'LINESTRING', 2);

    insert into ddnoded2 (id, cost, the_geom)
    select a.id, 1.0::float8 as cost, a.the_geom
    from ddunnoded_noded a order by id, sub_id;

    -- now create a topology
    select pgr_createtopology('ddnoded2', 0.000001, id:='gid');

    -- Total query runtime: 8080 ms.


    SET client_min_messages TO NOTICE;
    --this are equivalent
    select cost, count(*) from (
        select * from pgr_drivingdistance('select id, source, target, 1.0::float8 as cost from ddnoded2', 1274, 10, false, false)
    ) as foo group by cost order by cost asc;

    select agg_cost, count(*) from (
        select * from pgr_drivingdistance('select id, source, target, 1 as cost from ddnoded2', 1274, 10, false)
    ) as foo group by agg_cost order by agg_cost asc;

    --- this ones are equivalent
    select agg_cost, count(*) from (
        select * from pgr_drivingdistance('select id, source, target, 1 as cost from ddnoded2', 1274, 10, true)
    ) as foo group by agg_cost order by agg_cost asc;

    select agg_cost, count(*) from (
        select * from pgr_drivingdistance('select id, source, target, 1 as cost from ddnoded2', 1274, 10)
    ) as foo group by agg_cost order by agg_cost asc;

    ROLLBACK;
