

\i tools/testers/pickDeliver_pgtap_data.sql
SET extra_float_digits=-3;


SELECT * FROM _pgr_pickDeliverEuclidean(
    'SELECT * FROM orders ORDER BY id',
    'SELECT * from vehicles'
);
