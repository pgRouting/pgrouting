/* -- q1 */
SELECT * FROM pgr_binaryBreadthFirstSearch(
  'SELECT id, source, target, cost, reverse_cost from edge_table',
  6, 10, true);
/* -- q2 */
SELECT * FROM pgr_binaryBreadthFirstSearch(
  'SELECT id, source, target, cost, reverse_cost from edge_table',
  6, ARRAY[10, 17]);
/* -- q3 */
SELECT * FROM pgr_binaryBreadthFirstSearch(
  'SELECT id, source, target, cost, reverse_cost from edge_table',
  ARRAY[6, 1], 17);
/* -- q4 */
SELECT * FROM pgr_binaryBreadthFirstSearch(
  'SELECT id, source, target, cost, reverse_cost from edge_table',
  ARRAY[6, 1], ARRAY[10, 17],
  directed => false);
/* -- q5 */
SELECT source, target FROM combinations_table;
/* -- q51 */
SELECT * FROM pgr_binaryBreadthFirstSearch(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  'SELECT source, target FROM combinations_table',
  false);
/* -- q6 */
SELECT * FROM pgr_binaryBreadthFirstSearch(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  'SELECT * FROM (VALUES (6, 10), (6, 7), (12, 10)) AS combinations (source, target)');
/* -- q7 */
