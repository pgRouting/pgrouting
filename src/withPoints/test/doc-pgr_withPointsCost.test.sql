
BEGIN;

    \echo --e1
    SELECT * FROM pgr_withPointsCost(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from pointsOfInterest',
        1, 3);
    \echo --e2
    SELECT * FROM pgr_withPointsCost(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from pointsOfInterest',
        1, 3,
        directed := false);
    \echo --e3
    SELECT * FROM pgr_withPointsCost(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from pointsOfInterest',
        1, ARRAY[3,5]);
    \echo --e4
    SELECT * FROM pgr_withPointsCost(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from pointsOfInterest',
        ARRAY[1,2], 3);
    \echo --e5
    SELECT * FROM pgr_withPointsCost(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from pointsOfInterest',
        ARRAY[1,2], ARRAY[3,5]);

    \echo --q2
    SELECT * FROM pgr_withPointsCost(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from pointsOfInterest',
        ARRAY[1,2], ARRAY[3,5],
        driving_side := 'l');
    \echo --q3
    SELECT * FROM pgr_withPointsCost(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from pointsOfInterest',
        ARRAY[1,2], ARRAY[3,5],
        driving_side := 'r');
    \echo --q4
    SELECT * FROM pgr_withPointsCost(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from pointsOfInterest',
        ARRAY[1,2], ARRAY[3,5],
        driving_side := 'b');
    \echo --q5



    ROLLBACK;


