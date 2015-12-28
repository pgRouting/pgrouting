

BEGIN;
    SELECT PLAN(100);

    CREATE table points(
        pid SERIAL,
        x float,
        y float,
        edge_id bigint,
        side char,
        fraction float,
        the_geom geometry,
        newPoint geometry
    );

    INSERT INTO points (x,y) VALUES (1.7, 1.4);
    INSERT INTO points (x,y) VALUES (2.7, 1.4);
    INSERT INTO points (x,y) VALUES (2.7, 1.6);
    INSERT INTO points (x,y) VALUES (2.3, 1.6);
    INSERT INTO points (x,y) VALUES (2.5, 1.5);
    INSERT INTO points (x,y) VALUES (2, 1);

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
        FROM dump_values WHERE points.pid = dump_values.pid;


    UPDATE points SET
        edge_id = a.edge_id,
        fraction = a.fraction,
        side = a.side
    FROM (SELECT pid, (pgr_findClosestEdge('SELECT id, the_geom FROM edge_table', the_geom, 1)).* FROM points) a WHERE (a.pid = points.pid);


    PREPARE q1 AS
    SELECT seq, path_seq, node, agg_cost FROM pgr_withPoints(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from points',
        3, 1, 
        driving_side := 'r',
        directed := true,
        details := true);

    PREPARE q2 AS
    SELECT seq, path_seq, node, agg_cost FROM pgr_withPoints(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from points',
        3, 1, 
        driving_side := 'r',
        directed := true,
        details := false);

    SELECT set_eq('q1', 'q2', '1: Right: from point 3 to 1 it does not pass in front of any other point');


    PREPARE q3 AS
    SELECT seq, path_seq, node, agg_cost FROM pgr_withPoints(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from points',
        3, 4, 
        driving_side := 'r',
        directed := true,
        details := true);
    
    SELECT set_has('q3','q2','2: Right: from 3 to 4 it passes in front of point 1');

    PREPARE q4 AS
    SELECT seq, path_seq, node, agg_cost FROM pgr_withPoints(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from points',
        3, 4,
        driving_side := 'l',
        directed := true,
        details := true);

    PREPARE q5 AS
    SELECT seq, path_seq, node, agg_cost FROM pgr_withPoints(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from points',
        3, 4,
        driving_side := 'l',
        directed := true,
        details := false);

    SELECT set_eq('q4', 'q5', '3: Left: from point 3 to 4 it does not pass in front of any other point');


    PREPARE q6 AS
    SELECT seq, path_seq, node, agg_cost FROM pgr_withPoints(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from points',
        3, 1, 
        driving_side := 'l',
        directed := true,
        details := true);

    SELECT set_has('q6','q5','4: Left: from 3 to 1 it passes in front of point 4');


    PREPARE q7 AS
    SELECT seq, path_seq, node, agg_cost FROM pgr_withPoints(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from points',
        3, 1,
        driving_side := 'b',
        directed := true,
        details := true);

    PREPARE q8 AS
    SELECT seq, path_seq, node, agg_cost FROM pgr_withPoints(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from points',
        3, 4,
        driving_side := 'b',
        directed := true,
        details := true);

    SELECT set_has('q7','q8','Both: from 3 to 1 it passes in front of point 4');

    PREPARE q9 AS
    SELECT  -3 AS start_vid, -1 AS end_vid,  agg_cost FROM pgr_withPoints(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from points',
        3, 1,
        driving_side := 'r',
        directed := true,
        details := true) WHERE edge = -1;


    PREPARE q10 AS
    SELECT *  FROM pgr_withPointsCost(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from points',
        3, 1,
        driving_side := 'r',
        directed := true,
        details := true);

    SELECT set_eq('q10','q9','Right, directed: Cost is the last row');

    

        SELECT * FROM finish();
        ROLLBACK;
