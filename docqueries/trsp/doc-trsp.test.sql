/* -- q2 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT path, cost FROM restrictions$$,
  6, 10,
  false);
/* -- q3 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost FROM edge_table$$,
  $$SELECT * FROM restrictions$$,
  6, ARRAY[10, 1],
  false);
/* -- q4 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT path, cost FROM restrictions$$,
  ARRAY[6, 1], 8);
/* -- q5 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT path, cost FROM restrictions$$,
  ARRAY[6, 1], ARRAY[10, 8],
  false);
/* -- q6 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT path, cost FROM restrictions$$,
  $$SELECT * FROM (VALUES (6, 10), (6, 1), (6, 8), (1, 8)) AS combinations (source, target)$$);
/* -- q7 */
