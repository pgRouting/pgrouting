

\i tools/testers/pickDeliver_pgtap_data.sql


SELECT * FROM _pgr_pickDeliverEuclidean(
    'SELECT * FROM orders ORDER BY id',
    'SELECT * from vehicles'
);
