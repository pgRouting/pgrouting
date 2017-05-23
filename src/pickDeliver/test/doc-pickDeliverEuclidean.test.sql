\echo --q1
SELECT * FROM _pgr_pickDeliverEuclidean(
    'SELECT * FROM orders',
    'SELECT * from vehicles'
);
\echo --q2
