-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
/* -- q1 */
SELECT * FROM pgr_kruskalDD(
  'SELECT id, source, target, cost, reverse_cost FROM edges ORDER BY id',
  6, 3.5);
/* -- q2 */
SELECT * FROM pgr_kruskalDD(
  'SELECT id, source, target, cost, reverse_cost FROM edges ORDER BY id',
  ARRAY[9, 6], 3.5);
/* -- q3 */
