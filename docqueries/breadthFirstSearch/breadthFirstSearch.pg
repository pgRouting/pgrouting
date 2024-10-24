-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
/* -- q1 */
SELECT * FROM pgr_breadthFirstSearch(
  'SELECT id, source, target, cost, reverse_cost
  FROM edges ORDER BY id',
  6);
/* -- q2 */
SELECT * FROM pgr_breadthFirstSearch(
  'SELECT id, source, target, cost, reverse_cost
  FROM edges ORDER BY id',
  ARRAY[12, 6], directed => false, max_depth => 2);
/* -- q3 */
SELECT * FROM pgr_breadthFirstSearch(
  'SELECT id, source, target, cost, reverse_cost
  FROM edges ORDER BY id DESC',
  6);
/* -- q4 */
