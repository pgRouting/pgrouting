/* -- q2 */
SELECT * FROM pgr_aStarCost(
  'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2
  FROM edge_table',
  6, 12,
  directed => true, heuristic => 2
);
/* -- q3 */
SELECT * FROM pgr_aStarCost(
  'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2
  FROM edge_table',
  6, ARRAY[10, 12],
  heuristic => 3, factor := 3.5
);
/* -- q4 */
SELECT * FROM pgr_aStarCost(
  'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2
  FROM edge_table',
  ARRAY[6, 50], 10,
  false, heuristic => 4
);
/* -- q5 */
SELECT * FROM pgr_aStarCost(
  'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2
  FROM edge_table',
  ARRAY[6, 50], ARRAY[10, 12],
  factor => 0.5
);
/* -- q51 */
SELECT * FROM combinations_table;
/* -- q52 */
SELECT * FROM pgr_aStarCost(
  'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2
  FROM edge_table',
  'SELECT * FROM combinations_table',
  factor => 0.5
);
/* -- q6 */
 SELECT * FROM pgr_aStarCost(
  'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2
  FROM edge_table',
  ARRAY[7, 10, 15, 10, 10, 15], ARRAY[10, 7, 10, 15]);
/* -- q7 */
SELECT * FROM pgr_aStarCost(
  'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2
  FROM edge_table',
  ARRAY[7, 10, 15], ARRAY[7, 10, 15]);
/* -- q8 */
SELECT * FROM pgr_aStarCost(
  'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2
  FROM edge_table',
  'SELECT * FROM (VALUES (6, 10), (6, 7), (12, 10)) AS combinations (source, target)');
-/* -- q9 */
