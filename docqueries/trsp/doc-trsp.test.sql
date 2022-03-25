/* -- q2 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT path, cost FROM restrictions$$,
  2, 3,
  false
);
/* -- q3 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost FROM edge_table$$,
  $$SELECT * FROM restrictions$$,
  2, ARRAY[3,7],
  false
);
/* -- q4 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT path, cost FROM restrictions$$,
  ARRAY[2,7], 10
);
/* -- q5 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT path, cost FROM restrictions$$,
  ARRAY[2,7], ARRAY[3,10],
  false
);
/* -- q6 */
SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost, reverse_cost FROM edge_table$$,
  $$SELECT path, cost FROM restrictions$$,
  $$SELECT * FROM (VALUES (2, 3), (2, 7), (2, 10), (7, 10)) AS combinations (source, target)$$
);
/* -- q7 */
