/* -- q1 */
SELECT * FROM pgr_pushRelabel(
  'SELECT id, source, target, capacity, reverse_capacity
  FROM edge_table',
  11, 12);
/* -- q2 */
SELECT * FROM pgr_pushRelabel(
  'SELECT id, source, target, capacity, reverse_capacity
  FROM edge_table',
  11, ARRAY[5, 10, 12]);
/* -- q3 */
SELECT * FROM pgr_pushRelabel(
  'SELECT id, source, target, capacity, reverse_capacity
  FROM edge_table',
  ARRAY[11, 3, 17], 12);
/* -- q4 */
SELECT * FROM pgr_pushRelabel(
  'SELECT id, source, target, capacity, reverse_capacity
  FROM edge_table',
  ARRAY[11, 3, 17], ARRAY[5, 10, 12]);
/* -- q5 */
SELECT source, target FROM combinations_table
WHERE target NOT IN (5, 6);
/* -- q51 */
SELECT * FROM pgr_pushRelabel(
  'SELECT id, source, target, capacity, reverse_capacity
  FROM edge_table',
  'SELECT * FROM combinations_table WHERE target NOT IN (5, 6)');
/* -- q6 */
SELECT * FROM pgr_pushRelabel(
  'SELECT id, source, target, capacity, reverse_capacity
  FROM edge_table',
  'SELECT * FROM (VALUES (5, 10), (6, 15), (6, 14)) AS t(source, target)');
/* -- q7 */
