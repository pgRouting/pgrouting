
/* -- q1 */
SELECT * FROM new_restrictions;

/* -- q2 */
SELECT * FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions WHERE id < 3',
    2, 12, 3
);

/* -- q3 */
SELECT * FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions',
    2, 12, 3,
    heap_paths := true
);

/* -- q4 */
SELECT * FROM pgr_turnRestrictedPath(
    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
    'SELECT * FROM new_restrictions',
    2, 12, 3,
    false
);
/* -- q5 */
