-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
SET extra_float_digits=-3;
SET client_min_messages TO WARNING;

/* -- q1 */
SELECT * FROM pgr_TSP(
  $$SELECT * FROM pgr_dijkstraCostMatrix(
    'SELECT id, source, target, cost, reverse_cost FROM edges',
    (SELECT array_agg(id) FROM vertices WHERE id NOT IN (2, 4, 13, 14)),
    directed => false) $$);
/* -- q2 */
SELECT * FROM pgr_TSP(
  $$SELECT * FROM pgr_dijkstraCostMatrix(
    'SELECT id, source, target, cost, reverse_cost FROM edges',
    (SELECT array_agg(id) FROM vertices WHERE id NOT IN (2, 4, 13, 14)),
    directed => false) $$,
  start_id => 1);
/* -- q3 */
SELECT * FROM pgr_TSP(
  $$SELECT * FROM pgr_withPointsCostMatrix(
    'SELECT id, source, target, cost, reverse_cost FROM edges ORDER BY id',
    'SELECT pid, edge_id, fraction from pointsOfInterest',
    array[-1, 10, 7, 11, -6],
    directed => true) $$,
  start_id => 7,
  end_id => 11);
/* -- q4 */
SELECT * FROM pgr_dijkstraCostMatrix(
  $q1$SELECT id, source, target, cost, reverse_cost FROM edges WHERE id IN (2, 4, 5, 8, 9, 15)$q1$,
  (SELECT ARRAY[6, 7, 10, 11, 16, 17]),
  directed => true);
/* -- q5 */
SELECT * FROM pgr_TSP(
  $$SELECT * FROM pgr_dijkstraCostMatrix(
  $q1$SELECT id, source, target, cost, reverse_cost FROM edges WHERE id IN (2, 4, 5, 8, 9, 15)$q1$,
  (SELECT ARRAY[6, 7, 10, 11, 16, 17]),
  directed => true)$$);
/* -- q6 */
