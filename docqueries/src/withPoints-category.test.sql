/* --q1 */
SELECT pid, edge_id, fraction, side FROM pointsOfInterest;
/* --q2 */
SELECT id, source, target, cost, reverse_cost
FROM edge_table WHERE id = 15;
/* --q3 */
SELECT pid, edge_id, fraction, side
FROM pointsOfInterest WHERE pid = 2;
/* --q4 */
