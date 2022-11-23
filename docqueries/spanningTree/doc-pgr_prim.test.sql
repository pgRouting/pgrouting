-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
/* -- q1 */
SELECT edge, cost FROM pgr_prim(
  'SELECT id, source, target, cost, reverse_cost
  FROM edges WHERE id < 14'
) ORDER BY edge;
/* -- q2 */
