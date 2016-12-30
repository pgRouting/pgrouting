
WITH
pickups AS (
    SELECT id, demand, x as pick_x, y as pick_y, opentime as pick_open, closetime as pick_close, servicetime as pick_service
    FROM  customer WHERE pindex = 0 AND id != 0
),
deliveries AS (
    SELECT pindex AS id, x as deliver_x, y as deliver_y, opentime as deliver_open, closetime as deliver_close, servicetime as deliver_service
    FROM  customer WHERE dindex = 0 AND id != 0
)
SELECT * INTO orders
FROM pickups JOIN deliveries USING(id) ORDER BY pickups.id;


SELECT 0 AS id,
        40 AS start_x, 50 AS start_y,
        0 AS start_open, 1236 AS start_close,
        200 AS capacity, 25 AS number INTO vehicles;

/*
INSERT into vehicles (id, start_x, start_y, start_open, start_close, capacity, number)
VALUES (1, 40, 50, 0, 400, 200, 25);
*/
SELECT * FROM _pgr_pickDeliver(
    'SELECT * FROM orders ORDER BY id',
    'SELECT * FROM vehicles',
    30);

WITH
orders_id AS (
    SELECT id FROM orders
), 
results_id AS (
    SELECT DISTINCT order_id AS id
    FROM _pgr_pickDeliver(
        'SELECT * FROM orders ORDER BY id',
        'SELECT * FROM vehicles',
        30)
    WHERE stop_type NOT IN (0, 5, -1)
    ORDER BY order_id
)
SELECT * FROM orders_id LEFT JOIN results_id USING(ID);

WITH
results AS (
    SELECT * 
    FROM _pgr_pickDeliver(
        'SELECT * FROM orders ORDER BY id',
        'SELECT * FROM vehicles',
        30)
)
SELECT ((1 + (SELECT max(vehicle_id) FROM results) * 2 + 53 * 2) = 
    (SELECT count(*) from results)) AS correct_value;
