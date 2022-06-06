/* -- q1 */
SELECT * FROM pgr_johnson(
    'SELECT source, target, cost FROM edges WHERE id < 5'
);
/* -- q2 */
