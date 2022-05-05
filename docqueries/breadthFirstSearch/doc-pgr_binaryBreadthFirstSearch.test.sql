/* -- q1 */
SELECT * FROM pgr_binaryBreadthFirstSearch(
  'SELECT id, source, target, cost, reverse_cost from edge_table',
  2, 3, true);
/* -- q2 */
SELECT * FROM pgr_binaryBreadthFirstSearch(
  'SELECT id, source, target, cost, reverse_cost from edge_table',
  2, ARRAY[3, 12]);
/* -- q3 */
SELECT * FROM pgr_binaryBreadthFirstSearch(
  'SELECT id, source, target, cost, reverse_cost from edge_table',
  ARRAY[2, 7], 12);
/* -- q4 */
SELECT * FROM pgr_binaryBreadthFirstSearch(
  'SELECT id, source, target, cost, reverse_cost from edge_table',
  ARRAY[2, 7], ARRAY[3, 12],
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
  'SELECT * FROM (VALUES (2, 3), (2, 5), (11, 3)) AS combinations (source, target)');
/* -- q7 */
