/* -- q2 */
SELECT * FROM pgr_bdAstar(
  'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2
  FROM edge_table',
  2, 11,
  directed => true, heuristic => 2
);
/* -- q3 */
SELECT * FROM pgr_bdAstar(
  'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2
  FROM edge_table',
  2, ARRAY[3, 11],
  heuristic => 3, factor := 3.5
);
/* -- q4 */
SELECT * FROM pgr_bdAstar(
  'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2
  FROM edge_table',
  ARRAY[2, 10], 3,
  false, heuristic => 4
);
/* -- q5 */
SELECT * FROM pgr_bdAstar(
  'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2
  FROM edge_table',
  ARRAY[2, 10], ARRAY[3, 11],
  factor => 0.5
);
/* -- q51 */
SELECT * FROM combinations_table;
/* -- q52 */
SELECT * FROM pgr_bdAstar(
  'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2
  FROM edge_table',
  'SELECT * FROM combinations_table',
  factor => 0.5
);
/* -- q6 */
 SELECT * FROM pgr_bdAstar(
  'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2
  FROM edge_table',
  ARRAY[5, 3, 4, 3, 3, 4], ARRAY[3, 5, 3, 4]);
/* -- q7 */
SELECT * FROM pgr_bdAstar(
  'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2
  FROM edge_table',
  ARRAY[5, 3, 4], ARRAY[5, 3, 4]);
/* -- q8 */
SELECT * FROM pgr_bdAstar(
  'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2
  FROM edge_table',
  'SELECT * FROM (VALUES (2, 3), (2, 5), (11, 3)) AS combinations (source, target)');
-/* -- q9 */
