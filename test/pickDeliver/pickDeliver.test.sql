\echo --q1
SELECT * FROM _pgr_pickDeliver(
    'select * from customer order by id', 25, 200, 1, 30);
\echo --q2

SELECT * FROM _pgr_pickDeliverEuclidean(
    'SELECT * FROM orders ORDER BY id',
    'SELECT * FROM vehicles',
    30);
\echo --q3
