
\i setup.sql

SELECT PLAN(13);

PREPARE q1 AS
SELECT  path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    ARRAY[-1], ARRAY[-5],
    driving_side := 'r',
    directed := true,
    details := true);

PREPARE q11 AS
SELECT  path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    ARRAY[-1], ARRAY[-3],
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

PREPARE q21 AS
SELECT path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, -3,
    driving_side := 'r',
    directed := true,
    details := true);

SELECT set_eq('q1', 'q2', '1: Right: from point ARRAY[-1] to -5 same as one to one');
SELECT set_eq('q11', 'q21', '2: Right: from point ARRAY[-1] to -3 same as one to one');

PREPARE q3 AS
SELECT path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    ARRAY[-4], ARRAY[-5],
    driving_side := 'r',
    directed := true,
    details := true);

PREPARE q31 AS
SELECT path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    ARRAY[-4], ARRAY[-3],
    driving_side := 'r',
    directed := true,
    details := true);

PREPARE q4 AS
SELECT path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -4, -5,
    driving_side := 'r',
    directed := true,
    details := true);

PREPARE q41 AS
SELECT path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -4, -3,
    driving_side := 'r',
    directed := true,
    details := true);

SELECT set_eq('q3','q4','3: Right: from ARRAY[-4] to -5 same as one to one');
SELECT set_eq('q31','q41','4: Right: from ARRAY[-4] to -3 same as one to one');

PREPARE q5 AS
SELECT path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    ARRAY[-1, -4], ARRAY[-5,-3],
    driving_side := 'r',
    directed := true,
    details := true);

SELECT set_has('q5', 'q1', '5: Right: from point ARRAY[-1,-4] to -5 has results of from point -1 to -5');
SELECT set_has('q5', 'q2', '6: Right: from point ARRAY[-1,-4] to -5 has results of from point -1 to -5');
SELECT set_has('q5', 'q3', '7: Right: from point ARRAY[-1,-4] to -5 has results of from point -4 to -5');
SELECT set_has('q5', 'q4', '8: Right: from point ARRAY[-1,-4] to -5 has results of from point -4 to -5');
SELECT set_has('q5', 'q11', '9: Right: from point ARRAY[-1,-4] to -5 has results of from point -1 to -3');
SELECT set_has('q5', 'q21', '10: Right: from point ARRAY[-1,-4] to -5 has results of from point -1 to -3');
SELECT set_has('q5', 'q31', '11: Right: from point ARRAY[-1,-4] to -5 has results of from point -4 to -3');
SELECT set_has('q5', 'q41', '12: Right: from point ARRAY[-1,-4] to -5 has results of from point -4 to -3');



PREPARE q9 AS
SELECT  -1 AS start_vid, -5 AS end_vid,  agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    ARRAY[-1], ARRAY[-5],
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
