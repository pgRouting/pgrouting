

\i tools/testers/pickDeliver_pgtap_data.sql


SELECT * FROM pgr_pickDeliverEuclidean(
    'SELECT * FROM orders ORDER BY id',
    'SELECT * from vehicles'
);
