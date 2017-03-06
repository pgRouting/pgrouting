\echo --q2

SELECT * FROM _pgr_pickDeliverEuclidean(
    'SELECT * FROM orders ORDER BY id',
    'SELECT * FROM vehicles',
    30);
\echo --q3
