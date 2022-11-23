-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
/* --q1 */
SELECT pid, edge_id, fraction, side FROM pointsOfInterest;
/* --q2 */
SELECT id, source, target, cost, reverse_cost
FROM edges WHERE id = 15;
/* --q3 */
SELECT pid, edge_id, fraction, side
FROM pointsOfInterest WHERE pid = 2;
/* --q4 */
