
/* -- q1 */
SELECT * FROM pgr_chinesePostmanCost(
    'SELECT id,
     source, target,
     cost, reverse_cost FROM edge_table where id < 17'
);

/* -- q2 */
SELECT * FROM pgr_chinesePostmanCost(
    'SELECT id,
     source, target,
     cost, reverse_cost FROM edge_table where id = 9 or id = 11 or id = 13 or id = 15'
);

/* -- q3 */
