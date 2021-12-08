/* --q1 */
SELECT * FROM pgr_trspVia(
  'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
  'SELECT to_cost, target_id::int4, from_edge ||
  coalesce('',''||via_path,'''') AS via_path FROM restrictions',
  ARRAY[2,7,11]::INTEGER[],
  false,  false)
/* --q2 */
