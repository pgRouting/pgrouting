/* -- q1 */
SELECT * FROM pgr_strongComponents(
    'SELECT id, source, target, cost, reverse_cost FROM edges'
);
/* -- q2 */
