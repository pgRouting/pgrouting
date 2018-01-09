
\i setup.sql

SELECT PLAN(7);



PREPARE q1 AS
SELECT  path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, ARRAY[-5],
    driving_side := 'r',
    directed := true,
    details := true);

PREPARE q2 AS
SELECT path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, -5,
    driving_side := 'r',
    directed := true,
    details := true);

SELECT set_eq('q1', 'q2', '1: Right: from point -1 to -5 same as one to one');

PREPARE q3 AS
SELECT path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, ARRAY[-4],
    driving_side := 'r',
    directed := true,
    details := true);

PREPARE q4 AS
SELECT path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, -4,
    driving_side := 'r',
    directed := true,
    details := true);

SELECT set_eq('q3','q4','2: Right: from -1 to -4 same as one to one');

PREPARE q5 AS
SELECT path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, ARRAY[-4, -5],
    driving_side := 'r',
    directed := true,
    details := true);

SELECT set_has('q5', 'q1', '3: Right: from point -1 to -4 & -5 has results of from point -1 to -5');
SELECT set_has('q5', 'q2', '4: Right: from point -1 to -4 & -5 has results of from point -1 to -5');
SELECT set_has('q5', 'q3', '5: Right: from point -1 to -4 & -5 has results of from point -1 to -4');
SELECT set_has('q5', 'q4', '6: Right: from point -1 to -4 & -5 has results of from point -1 to -5');



PREPARE q9 AS
SELECT  -1 AS start_vid, -5 AS end_vid,  agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, ARRAY[-5],
    driving_side := 'r',
    directed := true,
    details := true) WHERE edge = -1;


PREPARE q10 AS
SELECT *  FROM pgr_withPointsCost(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, -5,
    driving_side := 'r',
    directed := true);

SELECT set_eq('q10','q9','Right, directed: Cost is the last row');


SELECT * FROM finish();
ROLLBACK;
