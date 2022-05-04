/* -- q1 */
SELECT * FROM pgr_edgeDisjointPaths(
  'SELECT id, source, target, cost, reverse_cost
  FROM edge_table',
  6, 11);
/* -- q2 */
SELECT * FROM pgr_edgeDisjointPaths(
  'SELECT id, source, target, cost, reverse_cost
  FROM edge_table',
  6, ARRAY[1, 3, 11]);
/* -- q3 */
SELECT * FROM pgr_edgeDisjointPaths(
  'SELECT id, source, target, cost, reverse_cost
  FROM edge_table',
  ARRAY[6, 8, 12], 11);
/* -- q4 */
SELECT * FROM pgr_edgeDisjointPaths(
  'SELECT id, source, target, cost, reverse_cost
  FROM edge_table',
  ARRAY[6, 8, 12], ARRAY[1, 3, 11]);
/* -- q5 */
SELECT source, target FROM combinations_table
WHERE target NOT IN (1, 2);
/* -- q51 */
SELECT * FROM pgr_edgeDisjointPaths(
  'SELECT id, source, target, cost, reverse_cost
  FROM edge_table',
  'SELECT * FROM combinations_table
   WHERE target NOT IN (1, 2)',
  directed => false);
/* -- q6 */
SELECT * FROM pgr_edgeDisjointPaths(
  'SELECT id, source, target, cost, reverse_cost
  FROM edge_table',
  'SELECT * FROM (VALUES (1, 3), (2, 4), (2, 17)) AS t(source, target)',
  directed => false);
/* -- q7 */
