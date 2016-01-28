
BEGIN;

    \echo --q1
    SELECT * FROM pgr_withPointsKSP(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from pointsOfInterest',
        1, 3, 2);
    \echo --q2
    SELECT * FROM pgr_withPointsKSP(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from pointsOfInterest',
        1, 3, 2, directed:=false);
    \echo --q3
    SELECT * FROM pgr_withPointsKSP(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from pointsOfInterest',
        1, 3, 2,
        driving_side := 'l', details := true);
    \echo --q4
    SELECT * FROM pgr_withPointsKSP(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
        'SELECT pid, edge_id, fraction, side from pointsOfInterest',
        1, 3, 2,
        driving_side := 'r', details := true);
    \echo --q5



    ROLLBACK;


