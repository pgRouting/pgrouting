/* -- q1 */
SELECT * FROM pgr_edgeDisjointPaths(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  3, 5
);
/* -- q2 */
SELECT * FROM pgr_edgeDisjointPaths(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  3, 5,
  directed := false
);
/* -- q3 */
SELECT * FROM pgr_edgeDisjointPaths(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  3, ARRAY[4, 5, 10]
);
/* -- q4 */
SELECT * FROM pgr_edgeDisjointPaths(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  ARRAY[3, 6], 5
);
/* -- q5 */
SELECT * FROM pgr_edgeDisjointPaths(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  ARRAY[3, 6], ARRAY[4, 5, 10]
);
/* -- q6 */
SELECT * FROM pgr_edgeDisjointPaths(
  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
  'SELECT * FROM ( VALUES (3, 4), (6, 5), (3, 10) ) AS t(source, target)'
);
/* -- q7 */
