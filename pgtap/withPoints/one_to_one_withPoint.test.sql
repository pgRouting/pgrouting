
\i setup.sql

SELECT PLAN(6);

PREPARE q1 AS
SELECT node, edge, round(agg_cost::numeric, 12) AS agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, -5,
    driving_side := 'r',
    directed := true,
    details := true);

PREPARE q2 AS
SELECT node, edge, round(agg_cost::numeric, 12) AS agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, -5,
    driving_side := 'r',
    directed := true,
    details := false);

PREPARE q21 AS
SELECT -6 AS node, 4 AS edge, round(2.1, 12) AS agg_cost;

SELECT set_has('q1', 'q2', '1: Right: from p1 to p5 pass in front of a point');
SELECT set_has('q1', 'q21', '2: Right: from p1 to p5 pass in front of  p6');

PREPARE q3 AS
SELECT seq, path_seq, node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, -5,
    driving_side := 'l',
    directed := true,
    details := true);


PREPARE q4 AS
SELECT  seq, path_seq, node, edge, round(cost::numeric, 12) AS cost, round(agg_cost::numeric, 12) AS agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, -5,
    driving_side := 'l',
    directed := true,
    details := false);

SELECT set_has('q1', 'q2', '3: Right: from p1 to p5 pass dont pass in front of points');

PREPARE q5 AS
SELECT node, edge, round(agg_cost::numeric, 12) AS agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, -5,
    driving_side := 'b',
    directed := true,
    details := true);

PREPARE q6 AS
SELECT node, edge, round(agg_cost::numeric, 12) AS agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, -5,
    driving_side := 'b',
    directed := true,
    details := false);

PREPARE q61 AS
SELECT -6::BIGINT AS node, 4::BIGINT AS edge, 1.3 AS agg_cost;

SELECT set_has('q5', 'q6', '4: both: from p1 to p5 pass in front of a point');
SELECT set_has('q5', 'q61', '5: both: from p1 to p5 pass in front of  p6');


PREPARE q7 AS
SELECT  -3 AS start_vid, -1 AS end_vid,  agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -3, -1,
    driving_side := 'r',
    directed := true,
    details := true) WHERE edge = -1;


PREPARE q8 AS
SELECT *  FROM pgr_withPointsCost(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -3, -1,
    driving_side := 'r',
    directed := true
    );

SELECT set_eq('q7','q8','Right, directed: Cost is the last row');


SELECT * FROM finish();
ROLLBACK;
