SET extra_float_digits=-3;
/* --q1 */
SELECT * FROM pgr_pickDeliverEuclidean(
    'SELECT * FROM orders ORDER BY id',
    'SELECT * from vehicles'
);
/* --q2 */
