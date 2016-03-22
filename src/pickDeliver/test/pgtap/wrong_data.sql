
\i setup.sql
\i pickDeliver_pgtap_data.sql

SELECT plan(5);

PREPARE q1 AS
SELECT * FROM pgr_pickDeliver(
    'SELECT id, x,y, demand, etime, ltime, stime, pindex, dindex
    FROM customer ORDER BY id'::text, 25, 200, 30);


SELECT lives_ok('q1', 'Original query should not fail');

PREPARE q2 AS
SELECT * FROM pgr_pickDeliver(
    'SELECT id, x,y, demand, etime, ltime, stime, pindex, dindex
    FROM customer WHERE id != 0 ORDER BY id'::text, 25, 200, 30);

SELECT throws_ok('q2',
    'XX000',
    'Depot node not found',
    'Should fail: depot is not included in data');

/*
 11 | 35 | 69 |     10 |   448 |   505 |    90 |      0 |      1
*/
PREPARE q3 AS
SELECT * FROM pgr_pickDeliver(
    'SELECT id, x,y, demand, etime, ltime, stime, pindex, dindex
    FROM customer WHERE id != 11 ORDER BY id'::text, 25, 200, 30);

SELECT throws_ok('q3',
    'XX000',
    'A pickup was not found',
    'Should fail: 11 is a Pickup and is missing');

/*
  1 | 45 | 68 |    -10 |   912 |   967 |    90 |     11 |      0
*/
PREPARE q4 AS
SELECT * FROM pgr_pickDeliver(
    'SELECT id, x,y, demand, etime, ltime, stime, pindex, dindex
    FROM customer WHERE id != 1 ORDER BY id'::text, 25, 200, 30);

SELECT throws_ok('q4',
    'XX000',
    'For pickup 11 the corresponding delivery was not found',
    'Should fail: id 1 is a delivery and is missing');

UPDATE customer SET ltime = 500 where id =0;

PREPARE q5 AS
SELECT * FROM pgr_pickDeliver(
    'SELECT id, x,y, demand, etime, ltime, stime, pindex, dindex
    FROM customer where id in (0,1,11) ORDER BY id'::text, 25, 200, 30);

SELECT throws_ok('q5',
    'XX000',
    'Found time window violation for truck with (pickup, delivery) = (11, 1)',
    'Should fail: Closing time of depot is too small and (pick,deliver) pair generates TWV');

UPDATE customer SET ltime = 1236 where id =0;



SELECT finish();
ROLLBACK;
