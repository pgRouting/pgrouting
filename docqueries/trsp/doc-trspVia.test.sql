/* --q1 */
SELECT * FROM pgr_trspVia(
  'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
  $$SELECT * FROM new_restrictions$$,
  ARRAY[2,7,11]::INTEGER[],
  false)
/* --q2 */
