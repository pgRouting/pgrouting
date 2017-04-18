\echo --q1
SELECT * FROM _pgr_pickDeliverEuclidean(
    'SELECT * FROM orders',
    'SELECT * from vehicles'
);
\echo --q2
SELECT * FROM _pgr_pickDeliver(
    'select * from customer order by id',
    25, 200, 1, 30
);
\echo --q3
SELECT * FROM pgr_gsoc_vrppdtw(
    'select * from customer order by id',
    25, 200
);
\echo --q4
