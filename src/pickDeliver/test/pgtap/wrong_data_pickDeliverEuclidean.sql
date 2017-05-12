
\i setup.sql

SELECT plan(11);

PREPARE q1 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT * FROM vehicles$$,
    max_cycles := 30);


SELECT lives_ok('q1', 'Original query should not fail');

--------------------------------------
-- testing wrong data on parameters 
--------------------------------------
PREPARE q6 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT * FROM vehicles$$,
    max_cycles := -1);

PREPARE q61 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT * FROM vehicles$$,
    max_cycles := 0);

PREPARE q7 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT * FROM vehicles$$,
    initial_sol := -1);

PREPARE q8 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT * FROM vehicles$$,
    initial_sol := 7);


SELECT throws_ok('q6',
    'XX000',
    'Illegal value in parameter: max_cycles',
    'Should throw: max_cycles < 0');

SELECT lives_ok('q61',
    'Should live: max_cycles == 0');

SELECT throws_ok('q7',
    'XX000',
    'Illegal value in parameter: initial',
    'Should throw: initial < 0');

SELECT throws_ok('q8',
    'XX000',
    'Illegal value in parameter: initial',
    'Should throw: initial > 6');


/*
 id | demand | pick_x | pick_y | pick_open_t | pick_close_t | pick_service_t | deliver_x | deliver_y | deliver_open_t | deliver_open_t | deliver_close_t | deliver_service_t
 1  | 10     |   35   |   69   |   448       |   505        |    90          |    45     |   68      |    912         |   967          |    90           |    35
*/
PREPARE q2 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT id FROM vehicles$$,
    30);

SELECT throws_ok('q2',
    'XX000',
    $$Column 'capacity' not Found$$,
    'Should fail: depot is not included in data');

UPDATE orders SET d_close = 500 WHERE id =11;

PREPARE q5 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    'SELECT * FROM orders WHERE id in (11) ORDER BY id',
    $$SELECT * FROM vehicles$$,
    30);

SELECT throws_ok('q5',
    'XX000',
    'The order 11 is not feasible on any truck',
    'Should fail: Closing time of depot is too small and (pick,deliver) pair generates TWV');

UPDATE orders SET d_close = 967 WHERE id =11;

--------------------------------------
-- testing wrong data on Vehicle 
--------------------------------------
UPDATE vehicles SET start_open = 3000  WHERE id = 0;
UPDATE vehicles SET start_open = 3000;

SELECT todo(1);
SELECT throws_ok('q5',
    'XX000',
    'Illegal values found on vehicle',
    'Should fail: Opens(DEPOT) > closes(DEPOT)');

UPDATE vehicles SET start_open = 0 WHERE id =0;
UPDATE vehicles SET start_open = 0;


--------------------------------------
-- testing wrong data on pickup 
--------------------------------------
UPDATE orders SET p_open = 600 WHERE id =11;

SELECT throws_ok('q5',
    'XX000',
    'The order 11 is not feasible on any truck',
    'Should fail: Opens(PICKUP) > closes(PICKUP)');

UPDATE orders SET p_open = 448, demand= -20 WHERE id =11;

SELECT throws_ok('q5',
    'XX000',
    'The order 11 is not feasible on any truck',
    'Should fail: demand(PICKUP) < 0');

UPDATE orders SET demand= 10 WHERE id =11;

--------------------------------------
-- testing wrong data on delivery 
--------------------------------------
UPDATE orders SET d_open = 1000 WHERE id =11;

SELECT throws_ok('q5',
    'XX000',
    'The order 11 is not feasible on any truck',
    'Should fail: Opens(DELIVERY) > closes(DELIVERY)');

SELECT finish();
ROLLBACK;
