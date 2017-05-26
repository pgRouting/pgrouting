\echo --q1
SELECT * FROM _pgr_pickDeliverEuclidean(
    'SELECT * FROM orders ORDER BY id',
    'SELECT * from vehicles'
);
\echo --q2
