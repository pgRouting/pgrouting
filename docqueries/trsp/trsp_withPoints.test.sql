SET extra_float_digits=-3;

/* --e1 */
SELECT * FROM pgr_trsp_withPoints(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id$$,
  $$SELECT id, path, cost FROM restrictions$$,
  $$SELECT pid, edge_id, fraction, side FROM pointsOfInterest$$,
  -1, -3);
/* --e2 */
SELECT * FROM pgr_trsp_withPoints(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id$$,
  $$SELECT id, path, cost FROM restrictions$$,
  $$SELECT pid, edge_id, fraction, side FROM pointsOfInterest$$,
  -1, 3,
  details := true);
/* --e3 */
SELECT * FROM pgr_trsp_withPoints(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id$$,
  $$SELECT id, path, cost FROM restrictions$$,
  $$SELECT pid, edge_id, fraction, side FROM pointsOfInterest$$,
  -1, ARRAY[-3,5]);
/* --e4 */
SELECT * FROM pgr_trsp_withPoints(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id$$,
  $$SELECT id, path, cost FROM restrictions$$,
  $$SELECT pid, edge_id, fraction, side FROM pointsOfInterest$$,
  ARRAY[-1,2], -3);
/* --e5 */
SELECT * FROM pgr_trsp_withPoints(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id$$,
  $$SELECT id, path, cost FROM restrictions$$,
  $$SELECT pid, edge_id, fraction, side FROM pointsOfInterest$$,
  ARRAY[-1,2], ARRAY[-3,7]);

/* --q2 */
SELECT ($$($$ || start_vid || $$ => $$ || end_vid ||$$) at $$ || path_seq || $$th step:$$)::TEXT AS path_at,
CASE WHEN edge = -1 THEN $$ visits$$
ELSE $$ passes in front of$$
  END as status,
  CASE WHEN node < 0 THEN $$Point$$
  ELSE $$Vertex$$
  END as is_a,
  abs(node) as id
  FROM pgr_trsp_withPoints(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id$$,
    $$SELECT id, path, cost FROM restrictions$$,
    $$SELECT pid, edge_id, fraction, side FROM pointsOfInterest$$,
    ARRAY[1,-1], ARRAY[-2,-3,-6,3,6],
    driving_side := $$r$$,
    details := true)
  WHERE node IN (-6,6);

/* --q3 */
SELECT ($$($$ || start_vid || $$ => $$ || end_vid ||$$) at $$ || path_seq || $$th step:$$)::TEXT AS path_at,
CASE WHEN edge = -1 THEN $$ visits$$
ELSE $$ passes in front of$$
  END as status,
  CASE WHEN node < 0 THEN $$Point$$
  ELSE $$Vertex$$
  END as is_a,
  abs(node) as id
  FROM pgr_trsp_withPoints(
    $$SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id$$,
    $$SELECT id, path, cost FROM restrictions$$,
    $$SELECT pid, edge_id, fraction, side FROM pointsOfInterest$$,
    ARRAY[1,-1], ARRAY[-2,-3,-6,3,6],
    driving_side := $$l$$,
    details := true)
  WHERE node IN (-6,6);
/* --q4 */
SELECT * FROM pgr_trsp_withPoints(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id$$,
  $$SELECT id, path, cost FROM restrictions$$,
  $$SELECT pid, edge_id, fraction, side FROM pointsOfInterest$$,
  ARRAY[-1,2], ARRAY[-3,7],
  directed := false,
  details := true);
/* --q5 */
SELECT * FROM pgr_trsp_withPoints(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table ORDER BY id$$,
  $$SELECT id, path, cost FROM restrictions$$,
  $$SELECT pid, edge_id, fraction, side FROM pointsOfInterest$$,
  $$SELECT * FROM ( VALUES (-1, 3), (2, -3) ) AS t(source, target)$$,
  driving_side => $$r$$,
  details => true);
/* --q6 */
