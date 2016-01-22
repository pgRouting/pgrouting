
\i setup.sql

SELECT PLAN(6);



PREPARE q1 AS
SELECT seq, path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    3, ARRAY[1], 
    driving_side := 'r',
    directed := true,
    details := true);

PREPARE q2 AS
SELECT seq, path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    3, ARRAY[1], 
    driving_side := 'r',
    directed := true,
    details := false);

SELECT set_eq('q1', 'q2', '1: Right: from point 3 to 1 it does not pass in front of any other point');

PREPARE q3 AS
SELECT seq, path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    3, ARRAY[4], 
    driving_side := 'r',
    directed := true,
    details := true);

SELECT set_has('q3','q2','2: Right: from 3 to 4 it passes in front of point 1');

PREPARE q4 AS
SELECT seq, path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    3, ARRAY[4],
    driving_side := 'l',
    directed := true,
    details := true);

PREPARE q5 AS
SELECT seq, path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    3, ARRAY[4],
    driving_side := 'l',
    directed := true,
    details := false);

SELECT set_eq('q4', 'q5', '3: Left: from point 3 to 4 it does not pass in front of any other point');


PREPARE q6 AS
SELECT seq, path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    3, ARRAY[1], 
    driving_side := 'l',
    directed := true,
    details := true);

SELECT set_has('q6','q5','4: Left: from 3 to 1 it passes in front of point 4');


PREPARE q7 AS
SELECT seq, path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    3, ARRAY[1],
    driving_side := 'b',
    directed := true,
    details := true);

PREPARE q8 AS
SELECT seq, path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    3, ARRAY[4],
    driving_side := 'b',
    directed := true,
    details := true);

SELECT set_has('q7','q8','Both: from 3 to 1 it passes in front of point 4');

PREPARE q9 AS
SELECT  -3 AS start_vid, -1 AS end_vid,  agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    3, ARRAY[1],
    driving_side := 'r',
    directed := true,
    details := true) WHERE edge = -1;


PREPARE q10 AS
SELECT *  FROM pgr_withPointsCost(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    3, ARRAY[1],
    driving_side := 'r',
    directed := true,
    details := true);

SELECT set_eq('q10','q9','Right, directed: Cost is the last row');


SELECT * FROM finish();
ROLLBACK;
