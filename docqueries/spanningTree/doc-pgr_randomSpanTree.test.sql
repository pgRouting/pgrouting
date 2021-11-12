
/* -- q1 */
SELECT * FROM pgr_randomSpanTree(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table', 5
);

/* -- q2 */
SELECT * FROM pgr_randomSpanTree(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table', 4, false
);

/* -- q3 */
