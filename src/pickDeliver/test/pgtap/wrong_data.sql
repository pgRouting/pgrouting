
\i setup.sql

SELECT plan(8);

PREPARE q1 AS
SELECT * FROM _pgr_pickDeliver(
    'SELECT *
    FROM orders ORDER BY id'::text, 25, 200, 1, 30);


SELECT lives_ok('q1', 'Original query should not fail');

/*
 id | demand | pick_x | pick_y | pick_open_t | pick_close_t | pick_service_t | deliver_x | deliver_y | deliver_open_t | deliver_open_t | deliver_close_t | deliver_service_t
 1  | 10     |   35   |   69   |   448       |   505        |    90          |    45     |   68      |    912         |   967          |    90           |    35
*/
PREPARE q2 AS
SELECT * FROM _pgr_pickDeliver(
    'SELECT *
    FROM orders WHERE id != 0 ORDER BY id'::text, 25, 200, 30);

SELECT throws_ok('q2',
    'XX000',
    'Depot node not found',
    'Should fail: depot is not included in data');

UPDATE orders SET pick_close = 500 WHERE id =0;

PREPARE q5 AS
SELECT * FROM _pgr_pickDeliver(
    'SELECT *
    FROM orders WHERE id in (0,1,11) ORDER BY id'::text, 25, 200, 30);

SELECT throws_ok('q5',
    'XX000',
    'The Order 11 is not feasible',
    'Should fail: Closing time of depot is too small and (pick,deliver) pair generates TWV');

--------------------------------------
-- testing wrong data on DEPOT 
--------------------------------------
UPDATE orders SET pick_open = 3000, pick_close = 1236 WHERE id =0;

SELECT throws_ok('q5',
    'XX000',
    'Illegal values found on the starting site',
    'Should fail: Opens(DEPOT) > closes(DEPOT)');

UPDATE orders SET pick_open = 0, demand = 20 WHERE id =0;

SELECT throws_ok('q5',
    'XX000',
    'Illegal values found on the starting site',
    'Should fail: Demand(DEPOT) != 0');

UPDATE orders SET demand = 0 WHERE id =0;

--------------------------------------
-- testing wrong data on pickup 
--------------------------------------
UPDATE orders SET pick_open = 600 WHERE id =11;

SELECT throws_ok('q5',
    'XX000',
    'Illegal values found on Order 11',
    'Should fail: Opens(PICKUP) > closes(PICKUP)');

UPDATE orders SET pick_open = 448, demand= -20 WHERE id =11;

SELECT throws_ok('q5',
    'XX000',
    'Illegal values found on Order 11',
    'Should fail: demand(PICKUP) < 0');

UPDATE orders SET demand= 10 WHERE id =11;

--------------------------------------
-- testing wrong data on delivery 
--------------------------------------
UPDATE orders SET deliver_open = 1000 WHERE id =11;

SELECT throws_ok('q5',
    'XX000',
    'Illegal values found on Order 11',
    'Should fail: Opens(DELIVERY) > closes(DELIVERY)');


SELECT finish();
ROLLBACK;
