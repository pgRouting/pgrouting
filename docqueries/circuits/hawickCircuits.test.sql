-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/
/* -- q1 */
SELECT * FROM pgr_hawickCircuits(
    'SELECT id, source, target, cost, reverse_cost FROM edges'
);
/* -- q2 */