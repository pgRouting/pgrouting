-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
/* --q1 */
SELECT * FROM pgr_KSP(
  'SELECT id, source, target, cost, reverse_cost FROM edges',
  6, 17, 2);
/* --q2 */
SELECT * FROM pgr_KSP(
  'select id, source, target, cost, reverse_cost from edges',
  6, ARRAY[10, 17], 2);
/* --q3 */
SELECT * FROM pgr_KSP(
  'select id, source, target, cost, reverse_cost from edges',
  ARRAY[6, 1], 17, 2);
/* --q4 */
SELECT * FROM pgr_KSP(
  'select id, source, target, cost, reverse_cost from edges',
  ARRAY[6, 1], ARRAY[10, 17], 2);
/* --q5 */
SELECT * FROM pgr_KSP(
  'SELECT id, source, target, cost, reverse_cost FROM edges',
  'SELECT source, target FROM combinations', 2);
/* --q6 */
SELECT * FROM pgr_KSP(
  'SELECT id, source, target, cost, reverse_cost FROM edges',
  6, 17, 2,
  directed => false, heap_paths => true
);
/* --q7 */
SELECT * FROM pgr_KSP(
  'SELECT id, source, target, cost, reverse_cost FROM edges',
  'SELECT source, target FROM combinations', 2, directed => false, heap_paths => true);
/* --q8 */
SELECT * FROM pgr_KSP(
  'select id, source, target, cost, reverse_cost from edges',
  ARRAY[6, 1], 17, 2, directed => false);
/* --q9 */
SELECT * FROM pgr_KSP(
  'select id, source, target, cost, reverse_cost from edges',
  ARRAY[6, 1], ARRAY[10, 17], 2, heap_paths => true);
/* --q10 */

