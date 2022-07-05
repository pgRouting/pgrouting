SET extra_float_digits=-3;
/* -- q1 */
SELECT * FROM pgr_withPoints(
  'SELECT id, source, target, cost, reverse_cost FROM edges ORDER BY id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  -1, 10,
  details => true);
/* -- q2 */
SELECT * FROM pgr_withPoints(
  'SELECT id, source, target, cost, reverse_cost FROM edges ORDER BY id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  -1, ARRAY[-3, 7],
  directed => false);
/* -- q3 */
SELECT * FROM pgr_withPoints(
  'SELECT id, source, target, cost, reverse_cost FROM edges ORDER BY id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  ARRAY[-1, 6], -3);
/* -- q4 */
SELECT * FROM pgr_withPoints(
  'SELECT id, source, target, cost, reverse_cost FROM edges ORDER BY id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  ARRAY[-1, 6], ARRAY[-3, 1]);
/* -- q5 */
SELECT * FROM pgr_withPoints(
  'SELECT id, source, target, cost, reverse_cost FROM edges ORDER BY id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  'SELECT * FROM (VALUES (-1, 10), (6, -3)) AS combinations(source, target)',
  driving_side => 'r', details => true);
/* -- q6 */
SELECT *
FROM pgr_withPoints(
  'SELECT id, source, target, cost, reverse_cost FROM edges ORDER BY id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  ARRAY[5, -1], ARRAY[-2, -3, -6, 10, 11],
  driving_side => 'r', details => true);
/* -- q7 */
SELECT (start_pid || ' -> ' || end_pid ||' at ' || path_seq || 'th step')::TEXT AS path_at,
  CASE WHEN edge = -1 THEN ' visits'
      ELSE ' passes in front of'
  END as status,
  CASE WHEN node < 0 THEN 'Point'
      ELSE 'Vertex'
  END as is_a,
  abs(node) as id
FROM pgr_withPoints(
  'SELECT id, source, target, cost, reverse_cost FROM edges ORDER BY id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  ARRAY[5, -1], ARRAY[-2, -3, -6, 10, 11],
  driving_side => 'r', details => true)
WHERE node IN (-6, 11);
/* -- q8 */
SELECT (start_pid || ' => ' || end_pid ||' at ' || path_seq || 'th step')::TEXT AS path_at,
  CASE WHEN edge = -1 THEN ' visits'
      ELSE ' passes in front of'
  END as status,
  CASE WHEN node < 0 THEN 'Point'
        ELSE 'Vertex'
    END as is_a,
    abs(node) as id
FROM pgr_withPoints(
  'SELECT id, source, target, cost, reverse_cost FROM edges ORDER BY id',
  'SELECT pid, edge_id, fraction, side from pointsOfInterest',
  ARRAY[5, -1], ARRAY[-2, -3, -6, 10, 11],
  driving_side => 'l', details => true)
WHERE node IN (-6, 11);
/* -- q9 */
