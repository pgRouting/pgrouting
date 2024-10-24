-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
/* -- q1 */
SELECT * FROM pgr_isPlanar(
  'SELECT id, source, target, cost, reverse_cost
  FROM edges'
);
/* -- q2 */
INSERT INTO edges (source, target, cost, reverse_cost) VALUES
  (10, 16, 1, 1), (10, 13, 1, 1),
  (15, 11, 1, 1), (15, 13, 1, 1),
  (11, 13, 1, 1), (16, 13, 1, 1);
/* -- q3 */
SELECT * FROM pgr_isPlanar(
  'SELECT id, source, target, cost, reverse_cost
  FROM edges');
/* -- q4 */
