
\i setup.sql

SELECT plan(11);

PREPARE q1 AS
SELECT * FROM _pgr_pickDeliver(
    'SELECT *
    FROM customer ORDER BY id'::text, 25, 200, 1, 30);


SELECT lives_ok('q1', 'Original query should not fail');

/*
 id | x  | y  | demand | etime | ltime | stime | pindex | dindex 
----+----+----+--------+-------+-------+-------+--------+--------
  0 | 40 | 50 |      0 |     0 |  1236 |     0 |      0 |      0
*/
PREPARE q2 AS
SELECT * FROM _pgr_pickDeliver(
    'SELECT *
    FROM customer WHERE id != 0 ORDER BY id'::text, 25, 200, 30);

SELECT throws_ok('q2',
    'XX000',
    'Depot node not found',
    'Should fail: depot is not included in data');

/*
 11 | 35 | 69 |     10 |   448 |   505 |    90 |      0 |      1
*/
PREPARE q3 AS
SELECT * FROM _pgr_pickDeliver(
    'SELECT *
    FROM customer WHERE id != 11 ORDER BY id'::text, 25, 200, 30);

SELECT throws_ok('q3',
    'XX000',
    'A pickup was not found',
    'Should fail: 11 is a Pickup and is missing');

/*
  1 | 45 | 68 |    -10 |   912 |   967 |    90 |     11 |      0
*/
PREPARE q4 AS
SELECT * FROM _pgr_pickDeliver(
    'SELECT *
    FROM customer WHERE id != 1 ORDER BY id'::text, 25, 200, 30);

SELECT throws_ok('q4',
    'XX000',
    'For Pickup 11 the corresponding Delivery was not found',
    'Should fail: id 1 is a delivery and is missing');

UPDATE customer SET closetime = 500 WHERE id =0;

PREPARE q5 AS
SELECT * FROM _pgr_pickDeliver(
    'SELECT *
    FROM customer WHERE id in (0,1,11) ORDER BY id'::text, 25, 200, 30);

SELECT throws_ok('q5',
    'XX000',
    'The (pickup, delivery) = (11, 1) is not feasible',
    'Should fail: Closing time of depot is too small and (pick,deliver) pair generates TWV');

--------------------------------------
-- testing wrong data on DEPOT 
--------------------------------------
UPDATE customer SET opentime = 3000, closetime = 1236 WHERE id =0;

SELECT throws_ok('q5',
    'XX000',
    'Illegal values found on the starting site',
    'Should fail: Opens(DEPOT) > closes(DEPOT)');

UPDATE customer SET opentime = 0, demand = 20 WHERE id =0;

SELECT throws_ok('q5',
    'XX000',
    'Illegal values found on the starting site',
    'Should fail: Demand(DEPOT) != 0');

UPDATE customer SET demand = 0 WHERE id =0;

--------------------------------------
-- testing wrong data on pickup 
--------------------------------------
UPDATE customer SET opentime = 600 WHERE id =11;

SELECT throws_ok('q5',
    'XX000',
    'Illegal values found on Pickup 11',
    'Should fail: Opens(PICKUP) > closes(PICKUP)');

UPDATE customer SET opentime = 448, demand= -20 WHERE id =11;

SELECT throws_ok('q5',
    'XX000',
    'Illegal values found on Pickup 11',
    'Should fail: demand(PICKUP) < 0');

UPDATE customer SET demand= 10 WHERE id =11;

--------------------------------------
-- testing wrong data on delivery 
--------------------------------------
UPDATE customer SET opentime = 1000 WHERE id =1;

SELECT throws_ok('q5',
    'XX000',
    'Illegal values found on Delivery 1',
    'Should fail: Opens(DELIVERY) > closes(DELIVERY)');

UPDATE customer SET opentime = 912, demand= 20 WHERE id =1;

SELECT throws_ok('q5',
    'XX000',
    'Illegal values found on Delivery 1',
    'Should fail: demand(DELIVERY) > 0');

UPDATE customer SET demand = -10 WHERE id =11;






SELECT finish();
ROLLBACK;
