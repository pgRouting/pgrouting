\set QUIET 1

SET client_min_messages = NOTICE;


    ------------------------------------------------------------------------------------------------------
    ------------------------------------------------------------------------------------------------------
    --              SAMPLE DATA                
    ------------------------------------------------------------------------------------------------------
    ------------------------------------------------------------------------------------------------------
    drop table if exists edge_table;

    CREATE TABLE edge_table (
        id BIGSERIAL,
        dir character varying,
        source BIGINT,
        target BIGINT,
        cost FLOAT,
        reverse_cost FLOAT,
        x1 FLOAT,
        y1 FLOAT,
        x2 FLOAT,
        y2 FLOAT,
        the_geom geometry
    );

    INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  2,0,   2,1);
    INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES (-1, 1,  2,1,   3,1);
    INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES (-1, 1,  3,1,   4,1);
    INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  2,1,   2,2);
    INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1,-1,  3,1,   3,2);
    INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  0,2,   1,2);
    INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  1,2,   2,2);
    INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  2,2,   3,2);
    INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  3,2,   4,2);
    INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  2,2,   2,3);
    INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1,-1,  3,2,   3,3);
    INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1,-1,  2,3,   3,3);
    INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1,-1,  3,3,   4,3);
    INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  2,3,   2,4);
    INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  4,2,   4,3);
    INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  4,1,   4,2);
    INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  0.5,3.5,  1.999999999999,3.5);
    INSERT INTO edge_table (cost,reverse_cost,x1,y1,x2,y2) VALUES ( 1, 1,  3.5,2.3,  3.5,4);

    UPDATE edge_table SET the_geom = st_makeline(st_point(x1,y1),st_point(x2,y2)),
    dir = CASE WHEN (cost>0 and reverse_cost>0) THEN 'B'   -- both ways
               WHEN (cost>0 and reverse_cost<0) THEN 'FT'  -- direction of the LINESSTRING
               WHEN (cost<0 and reverse_cost>0) THEN 'TF'  -- reverse direction of the LINESTRING
               ELSE '' END;                                -- unknown

    SELECT pgr_createTopology('edge_table',0.001);

    DROP table if exists vertex_table;
    CREATE TABLE vertex_table (
        id serial,
        x double precision,
        y double precision
    );

    INSERT INTO vertex_table VALUES
    (1,2,0), (2,2,1), (3,3,1), (4,4,1), (5,0,2), (6,1,2), (7,2,2),
    (8,3,2), (9,4,2), (10,2,3), (11,3,3), (12,4,3), (13,2,4);



    DROP table if exists pointsOfInterest;
    CREATE TABLE pointsOfInterest(
        pid BIGSERIAL,
        x FLOAT,
        y FLOAT,
        edge_id BIGINT,
        side CHAR,
        fraction FLOAT,
        the_geom geometry,
        newPoint geometry
    );

    INSERT INTO pointsOfInterest (x,y) VALUES (1.7, 1.4);
    INSERT INTO pointsOfInterest (x,y) VALUES (2.7, 1.4);
    INSERT INTO pointsOfInterest (x,y) VALUES (2.7, 1.6);
    INSERT INTO pointsOfInterest (x,y) VALUES (2.3, 1.6);
    INSERT INTO pointsOfInterest (x,y) VALUES (2.5, 1.5);
    INSERT INTO pointsOfInterest (x,y) VALUES (2, 1);

    update pointsOfInterest set the_geom = st_makePoint(x,y);

    WITH
    close_ones AS
    (SELECT id, pid, a.the_geom AS line, b.the_geom AS point, ST_Distance(a.the_geom, b.the_geom) distance, ST_ClosestPoint(a.the_geom, b.the_geom) AS newPoint
        FROM edge_table AS a , pointsOfInterest AS b where ST_DWithin(a.the_geom, b.the_geom, 1)),
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
    UPDATE pointsOfInterest SET  --edge_id = id,
    newPoint = dump_values.newPoint
    --fraction = dump_values.fraction,
    --side = rightSide
    FROM dump_values WHERE pointsOfInterest.pid = dump_values.pid;


    UPDATE pointsOfInterest SET
    edge_id = a.edge_id,
    fraction = a.fraction,
    side = a.side
    FROM (SELECT pid, (pgr_findClosestEdge('SELECT id, the_geom FROM edge_table', the_geom, 1)).* FROM pointsOfInterest) a WHERE (a.pid = pointsOfInterest.pid);

    DROP TABLE IF EXISTS restrictions;
    CREATE TABLE restrictions (
        rid BIGINT NOT NULL,
        to_cost FLOAT,
        target_id BIGINT,
        from_edge BIGINT,
        via TEXT
    );

    COPY restrictions (rid, to_cost, target_id, from_edge, via) FROM stdin WITH NULL '__NULL__' DELIMITER ',';
1,100,7,4,__NULL__
1,100,11,8,__NULL__
1,100,10,7,__NULL__
2,4,8,3,5
3,100,9,16,__NULL__
\.

