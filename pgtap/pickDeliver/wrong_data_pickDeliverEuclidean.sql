
\i setup.sql

SELECT plan(23);

PREPARE q1 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT * FROM vehicles$$,
    max_cycles := 30);


SELECT lives_ok('q1', 'Original query should not fail');

--------------------------------------
-- testing wrong data on max_cycles
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

SELECT throws_ok('q6',
    'XX000',
    'Illegal value in parameter: max_cycles',
    'Should throw: max_cycles < 0');

SELECT lives_ok('q61',
    'Should live: max_cycles == 0');


--------------------------------------
-- testing wrong data on initial_sol
--------------------------------------
PREPARE initsol1 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT * FROM vehicles$$,
    initial_sol := -1);

PREPARE initsol2 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT * FROM vehicles$$,
    initial_sol := 7);

PREPARE initsol3 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT * FROM vehicles$$,
    initial_sol := 0);



SELECT throws_ok('initsol1',
    'XX000',
    'Illegal value in parameter: initial_sol',
    'Should throw: initial_sol < 0');

SELECT throws_ok('initsol2',
    'XX000',
    'Illegal value in parameter: initial_sol',
    'Should throw: initial_sol > 6');

SELECT throws_ok('initsol3',
    'XX000',
    'Illegal value in parameter: initial_sol',
    'Should throw: initial_sol = 0');

--------------------------------------
-- testing wrong data on factor
--------------------------------------
PREPARE factor1 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT * FROM vehicles$$,
    factor := -1);

PREPARE factor2 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT * FROM vehicles$$,
    factor := 0);

PREPARE factor3 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT * FROM vehicles$$,
    factor := 1);

SELECT throws_ok('factor1',
    'XX000',
    'Illegal value in parameter: factor',
    'Should throw: factor < 0');

SELECT throws_ok('factor2',
    'XX000',
    'Illegal value in parameter: factor',
    'Should throw: factor = 0');

SELECT lives_ok('factor3',
    'Should live: factor >= 1');

-- id | capacity | start_x | start_y | start_open_t | start_close_t | start_service_t
--                 | end_x | end_y   | end_open_t   |   end_close_t | end_service_t


PREPARE vehiles0 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT id AS vid FROM vehicles$$);

PREPARE vehiles1 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT id FROM vehicles$$);

PREPARE vehiles2 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT id, capacity FROM vehicles$$);

PREPARE vehiles3 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT id, capacity, start_x FROM vehicles$$);

PREPARE vehiles4 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT id, capacity, start_x, start_y FROM vehicles$$);

SELECT throws_ok('vehiles0',
    'XX000',
    $$Column 'id' not Found$$,
    'Should throw: id is not included in data');

SELECT throws_ok('vehiles1',
    'XX000',
    $$Column 'capacity' not Found$$,
    'Should throw: capacity is not included in data');

SELECT throws_ok('vehiles2',
    'XX000',
    $$Column 'start_x' not Found$$,
    'Should throw: start_x is not included in data');

SELECT throws_ok('vehiles3',
    'XX000',
    $$Column 'start_y' not Found$$,
    'Should throw: start_y is not included in data');

SELECT lives_ok('vehiles4',
    'Should live: rest of parameters are optional');

-- end_open and end_close should exist together or not at all

PREPARE vehiles5 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT *, 10 AS end_close FROM vehicles$$);

PREPARE vehiles6 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT *, 10 AS end_open FROM vehicles$$);

-- end_x and end_y should exist together or not at all

PREPARE vehiles7 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT *, 10 AS end_x FROM vehicles$$);

PREPARE vehiles8 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT *, 10 AS end_open FROM vehicles$$);

SELECT throws_ok('vehiles5',
    'XX000',
    $$Column 'end_open' not Found$$,
    'vehiles5, Should throw: end_close found, but not end_open');

SELECT throws_ok('vehiles6',
    'XX000',
    $$Column 'end_close' not Found$$,
    'vehiles6, Should throw: end_open found, but not end_close');

SELECT throws_ok('vehiles7',
    'XX000',
    $$Column 'end_y' not Found$$,
    'vehiles7, Should throw: end_x found, but not end_y');

SELECT throws_ok('vehiles8',
    'XX000',
    $$Column 'end_close' not Found$$,
    'vehiles8, Should throw: end_y found, but not end_x');

---------------------
--  start_open > start_close
---------------------
UPDATE vehicles SET start_open = 5, start_close = 4 WHERE id = 1;

PREPARE vehicles9 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT * FROM vehicles$$);

SELECT throws_ok('vehicles9',
    'XX000',
    'Illegal values found on vehicle',
    'vehicles9, Should throw: start_open > start_close');

UPDATE vehicles SET start_open = 0, start_close = 50 WHERE id = 1;

---------------------
--  end_open > end_close
---------------------
PREPARE vehicles10 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT *, 5 AS end_open, 4 AS end_close FROM vehicles$$);

SELECT throws_ok('vehicles10',
    'XX000',
    'Illegal values found on vehicle',
    'vehicles10, Should throw: end_open > end_close');

--------------------------------------
-- testing wrong data on orders
--------------------------------------

---------------------
--  d_open > d_close
---------------------
UPDATE orders SET d_close = 5 WHERE id = 1;

PREPARE orders1 AS
SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT * FROM vehicles$$);

SELECT throws_ok('orders1',
    'XX000',
    'Order not feasible on any truck was found',
    'orders1, Should throw: d_open > d_close');

UPDATE orders SET d_close = 15 WHERE id = 1;

---------------------
--  p_open > p_close
---------------------
UPDATE orders SET p_close = 1 WHERE id = 1;

SELECT throws_ok('orders1',
    'XX000',
    'Order not feasible on any truck was found',
    'orders1, Should throw: p_open > p_close');

UPDATE orders SET p_close = 10 WHERE id = 1;

---------------------
--  demand <= 0
---------------------

UPDATE orders SET demand= -20 WHERE id =1;

SELECT throws_ok('orders1',
    'XX000',
    'Order not feasible on any truck was found',
    'Should throw: demand(PICKUP) < 0');

UPDATE orders SET demand= 10 WHERE id =11;

SELECT finish();
ROLLBACK;
