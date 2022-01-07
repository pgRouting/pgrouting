/* --q1 */
SELECT * FROM pgr_trsp_withPointsVia(
  $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table$$,
  $$SELECT * FROM new_restrictions$$,
  $$SELECT pid, edge_id, fraction, side from pointsOfInterest$$,
  ARRAY[2,7,11]::INTEGER[], directed => true);
/* --q2 */
SELECT * FROM pgr_trsp_withPointsVia(
  $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table$$,
  $$SELECT * FROM new_restrictions$$,
  $$SELECT pid, edge_id, fraction, side from pointsOfInterest$$,
  ARRAY[2,7,11]::INTEGER[], directed => false);
SELECT * FROM pgr_trsp_withPointsVia(
  $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table$$,
  $$SELECT * FROM new_restrictions$$,
  $$SELECT pid, edge_id, fraction, side from pointsOfInterest$$,
  ARRAY[2,7,11]::INTEGER[], directed => false, driving_side => 'r');
SELECT * FROM pgr_trsp_withPointsVia(
  $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table$$,
  $$SELECT * FROM new_restrictions$$,
  $$SELECT pid, edge_id, fraction, side from pointsOfInterest$$,
  ARRAY[2,7,11]::INTEGER[], directed => false, driving_side => 'l');
SELECT * FROM pgr_trsp_withPointsVia(
  $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table$$,
  $$SELECT * FROM new_restrictions$$,
  $$SELECT pid, edge_id, fraction, side from pointsOfInterest$$,
  ARRAY[2,7,11]::INTEGER[], directed => false, driving_side => 'l', details => true);
SELECT * FROM pgr_trsp_withPointsVia(
  $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table$$,
  $$SELECT * FROM new_restrictions$$,
  $$SELECT pid, edge_id, fraction, side from pointsOfInterest$$,
  ARRAY[2,7,11]::INTEGER[], directed => false, driving_side => 'l', details => true, U_turn_on_edge => false);
SELECT * FROM pgr_trsp_withPointsVia(
  $$SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table$$,
  $$SELECT * FROM new_restrictions$$,
  $$SELECT pid, edge_id, fraction, side from pointsOfInterest$$,
  ARRAY[2,7,11]::INTEGER[], directed => false, driving_side => 'l', details => true, U_turn_on_edge => false, strict => true);
