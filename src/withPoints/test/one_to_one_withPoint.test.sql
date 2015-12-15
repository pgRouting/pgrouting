

BEGIN;
    create table points(
        pid SERIAL,
        x float,
        y float,
        edge_id bigint,
        side char,
        fraction float,
        the_geom geometry,
        newPoint geometry
    );

    insert into points (x,y) values (2.3, 1.4);
    insert into points (x,y) values (2.7, 1.4);
    insert into points (x,y) values (2.7, 1.6);
    insert into points (x,y) values (2.3, 1.6);
    insert into points (x,y) values (2.5, 1.5);
    insert into points (x,y) values (2, 1);

    update points set the_geom = st_makePoint(x,y);

    WITH 
    close_ones AS
    (SELECT id, pid, a.the_geom AS line, b.the_geom AS point, ST_Distance(a.the_geom, b.the_geom) distance, ST_ClosestPoint(a.the_geom, b.the_geom) AS newPoint
        FROM edge_table AS a , points AS b where ST_DWithin(a.the_geom, b.the_geom, 1)),
    closest_ones AS
    (SELECT pid, min(distance) min_dist FROM close_ones GROUP BY pid),
    third_q AS
    (SELECT  id, pid, distance, line, point, newPoint FROM close_ones JOIN closest_ones USING (pid) WHERE distance = min_dist),
    fourth_q AS
    (SELECT DISTINCT ON (pid) pid, id, distance, line, point, newPoint FROM  third_q ORDER BY pid, id, distance),
    dump_values AS
    (SELECT id, pid, newPoint,
        ST_length(ST_GeometryN(st_split(line, newpoint),1))/ST_length(line) AS fraction,
        line << point as rightSide
        FROM fourth_q)
    UPDATE points SET  --edge_id = id,
        newPoint = dump_values.newPoint
        --fraction = dump_values.fraction,
        --side = rightSide
        FROM dump_Values WHERE points.pid = dump_values.pid;


    UPDATE points SET
        edge_id = a.edge_id,
        fraction = a.fraction,
        side = a.side
    FROM (SELECT pid, (pgr_findClosestEdge('SELECT id, the_geom FROM edge_table', the_geom, 1)).* FROM points) a WHERE (a.pid = points.pid);

    SELECT * FROM points;

    SELECT * FROM pgr_withPoints(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from points',
        1, 3);

    /*
    SELECT * FROM pgr_withPoints(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side, right_side from points',
        1, 3);
*/


        ROLLBACK;
