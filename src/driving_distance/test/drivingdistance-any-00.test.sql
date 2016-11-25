-- each ring will increase by 4 because it is on a square grid
-- to find the start node number
-- SELECT *, st_distance(st_makepoint(25,25), the_geom) from vertices_tmp where st_dwithin(st_makepoint(25,25), the_geom, 1.0) order by st_distance(st_makepoint(25,25), the_geom) limit 1;
BEGIN;

    SET client_min_messages TO NOTICE;
    -- create 50x50 grid of overlapping lines horizontal and vertical

    DO $$
    DECLARE debuglevel TEXT;
    BEGIN

        EXECUTE 'show client_min_messages' into debuglevel;

        SET client_min_messages TO WARNING;

        create table ddunnoded (
            id serial not null primary key
        );

        PERFORM  addgeometrycolumn('ddunnoded', 'the_geom', ST_SRID('POINT(0 0)'::geometry), 'LINESTRING', 2);

        INSERT into ddunnoded (the_geom)
        SELECT st_astext(st_makeline(st_makepoint(0,s1), st_makepoint(51,s1))) from (SELECT generate_series(1,50) as s1) as foo
        union all
        SELECT st_astext(st_makeline(st_makepoint(s1,0), st_makepoint(s1,51))) from (SELECT generate_series(1,50) as s1) as foo;

        -- node the grid so we can use it
        PERFORM pgr_nodenetwork('ddunnoded',0.000001);

        -- copy the noded table into a table we can use for a graph
        -- and add the required columns


        create table ddnoded2 (
            gid serial not null primary key,
            id integer,
            source integer,
            target integer,
            cost float8
        );

        PERFORM addgeometrycolumn('ddnoded2', 'the_geom', ST_SRID('POINT(0 0)'::geometry), 'LINESTRING', 2);

        insert into ddnoded2 (id, cost, the_geom)
        SELECT a.id, 1.0::float8 as cost, a.the_geom
        from ddunnoded_noded a order by id, sub_id;

        -- now create a topology
        PERFORM pgr_createtopology('ddnoded2', 0.000001, id:='gid');

        -- Total query runtime: 8080 ms.


        execute 'set client_min_messages  to '|| debuglevel;
    END $$;

    --this are equivalent
    SELECT seq, id1, id2, round(cost::numeric, 2) from pgr_drivingdistance('SELECT id, source, target, 1.0::float8 as cost from ddnoded2', 1274, 10, false, false);
    SELECT seq, node, edge, round(cost::numeric, 2), round(agg_cost::numeric,2) from pgr_drivingdistance('SELECT id, source, target, 1.0::float8 as cost from ddnoded2', 1274, 10, false);


    --- this ones are equivalent
    SELECT seq, node, edge, round(cost::numeric, 2), round(agg_cost::numeric,2) from pgr_drivingdistance('SELECT id, source, target, 1.0::float8 as cost from ddnoded2', 1274, 10, true);
    SELECT seq, node, edge, round(cost::numeric, 2), round(agg_cost::numeric,2) from pgr_drivingdistance('SELECT id, source, target, 1.0::float8 as cost from ddnoded2', 1274, 10);


    ROLLBACK;
