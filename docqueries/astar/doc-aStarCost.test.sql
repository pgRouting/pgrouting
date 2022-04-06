/* -- q2 */
SELECT * FROM pgr_aStarCost(
    'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2 FROM edge_table',
    2, 12,
    directed := true, heuristic := 2);
/* -- q3 */
SELECT * FROM pgr_aStarCost(
    'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2 FROM edge_table',
    2, ARRAY[3, 12], heuristic := 2);
/* -- q4 */
SELECT * FROM pgr_aStarCost(
    'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2 FROM edge_table',
    ARRAY[2, 7], 12, heuristic := 0);
/* -- q5 */
SELECT * FROM pgr_aStarCost(
    'SELECT id, source, target, cost, reverse_cost, x1, y1, x2, y2 FROM edge_table',
    ARRAY[2, 7], ARRAY[3, 12], heuristic := 2);
/* -- q51 */
SELECT source, target FROM combinations_table;
/* -- q52 */
SELECT * FROM pgr_aStarCost(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  'SELECT source, target FROM combinations_table',
  false);
/* -- q6 */
SELECT * FROM pgr_aStarCost(
  'select id, source, target, cost, reverse_cost from edge_table',
  ARRAY[5, 3, 4, 3, 3, 4], ARRAY[3, 5, 3, 4]);
/* -- q7 */
SELECT * FROM pgr_aStarCost(
  'select id, source, target, cost, reverse_cost from edge_table',
  ARRAY[5, 3, 4], ARRAY[5, 3, 4]);
/* -- q8 */
SELECT * FROM pgr_aStarCost(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  'SELECT * FROM (VALUES (2, 3), (2, 5), (11, 3)) AS combinations (source, target)');
/* -- q9 */
