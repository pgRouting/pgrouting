/* --q1 */
SELECT * FROM pgr_withPointsVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT pid, edge_id, fraction, side from pointsOfInterest$$,
  ARRAY[2,7,11], directed => true);
/* --q2 */
SELECT * FROM pgr_withPointsVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT pid, edge_id, fraction, side from pointsOfInterest$$,
  ARRAY[2,7,11], directed => false);
SELECT * FROM pgr_withPointsVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT pid, edge_id, fraction, side from pointsOfInterest$$,
  ARRAY[2,7,11], directed => false, driving_side => 'r');
SELECT * FROM pgr_withPointsVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT pid, edge_id, fraction, side from pointsOfInterest$$,
  ARRAY[2,7,11], directed => false, driving_side => 'l');
SELECT * FROM pgr_withPointsVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT pid, edge_id, fraction, side from pointsOfInterest$$,
  ARRAY[2,7,11], directed => false, driving_side => 'l', details => true);
SELECT * FROM pgr_withPointsVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT pid, edge_id, fraction, side from pointsOfInterest$$,
  ARRAY[2,7,11], directed => false, driving_side => 'l', details => true, allow_u_turn => false);
SELECT * FROM pgr_withPointsVia(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT pid, edge_id, fraction, side from pointsOfInterest$$,
  ARRAY[2,7,11], directed => false, driving_side => 'l', details => true, allow_u_turn => false, strict => true);
