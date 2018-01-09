
\i setup.sql

SELECT PLAN(8);


PREPARE q1 AS
WITH the_union AS (
    (SELECT path_seq, -2 AS end_pid, node, agg_cost FROM pgr_withPoints(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
            'SELECT pid, edge_id, fraction, side from pointsOfInterest',
            -1, ARRAY[-2],
            driving_side := 'r',
            directed := true,
            details := true))
    UNION ALL
    (SELECT path_seq, -5 AS end_pid, node, agg_cost FROM pgr_withPoints(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
            'SELECT pid, edge_id, fraction, side from pointsOfInterest',
            -1, ARRAY[-5],
            driving_side := 'r',
            directed := true,
            details := true))
),
the_ordered AS (SELECT * FROM the_union ORDER BY end_pid, path_seq)
select row_number() OVER() AS seq, * FROM the_ordered;

PREPARE q2 AS
SELECT seq, path_seq, end_pid AS end_pid, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, ARRAY[-2, -5],
    driving_side := 'r',
    directed := true,
    details := true);

PREPARE q3 AS
SELECT seq, path_seq, end_pid AS end_pid, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, ARRAY[-5, -2],
    driving_side := 'r',
    directed := true,
    details := true);

PREPARE q4 AS
SELECT seq, path_seq, end_pid AS end_pid, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    'SELECT pid, edge_id, fraction, side from pointsOfInterest',
    -1, ARRAY[-5, -2, -2, -2, -5, -5, -2],
    driving_side := 'r',
    directed := true,
    details := true);


SELECT set_eq('q2', 'q1', '1: From point 1 to 2 and 5 the union vs 2,5 order');
SELECT set_eq('q3', 'q1', '2: From point 1 to 2 and 5 the union vs 5,2 order');
SELECT set_eq('q4', 'q1', '3: From point 1 to 2 and 5 the union vs 5,2 eliminate repetitions');
SELECT set_eq('q2', 'q3', '4: From point 1 to 2 and 5 the 2,5 vs 5,2 order');
SELECT set_eq('q2', 'q4', '5: From point 1 to 2 and 5 the 2,5 vs 5,2 eliminate repetitions');


PREPARE q5 AS
SELECT seq, path_seq, -end_pid AS end_pid, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    '(SELECT pid, edge_id, fraction, side from pointsOfInterest) UNION ALL (SELECT pid, edge_id, fraction, side from pointsOfInterest)',
    3, ARRAY[5, 1, 1, 5, 5, 1],
    driving_side := 'r',
    directed := true,
    details := true);

SELECT lives_ok('q5');

PREPARE q6 AS
SELECT seq, path_seq, -end_pid AS end_pid, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    '(SELECT pid, edge_id, fraction, side from pointsOfInterest) UNION ALL (SELECT 1 as pid, edge_id, fraction, side from pointsOfInterest)',
    3, ARRAY[5, 1, 1, 5, 5, 1],
    driving_side := 'r',
    directed := true,
    details := true);

SELECT throws_ok('q6', 'XX000',
    'Unexpected point(s) with same pid but different edge/fraction/side combination found.',
    'Same id with different data throws');

PREPARE q7 AS
SELECT seq, path_seq, node, agg_cost FROM pgr_withPoints(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id',
    '(SELECT pid, edge_id, fraction, side from pointsOfInterest) UNION ALL (SELECT 1 as pid, edge_id, fraction, side from pointsOfInterest)',
    3, 5,
    driving_side := 'r',
    directed := true,
    details := true);

SELECT throws_ok('q7', 'XX000',
    'Unexpected point(s) with same pid but different edge/fraction/side combination found.',
    'Same id with different data throws');

/*
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
 */

SELECT * FROM finish();
ROLLBACK;
