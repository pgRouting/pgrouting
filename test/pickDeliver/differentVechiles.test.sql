
INSERT into vehicles(start_x, start_y, start_open, start_close, capacity, number)
VALUES (40, 50, 0, 400, 200, 25);

SELECT * FROM _pgr_pickDeliverEuclidean(
    'SELECT * FROM orders ORDER BY id',
    'SELECT * FROM vehicles',
    30);

WITH
orders_id AS (
    SELECT id FROM orders
),
results_id AS (
    SELECT DISTINCT order_id AS id
    FROM _pgr_pickDeliverEuclidean(
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
    FROM _pgr_pickDeliverEuclidean(
        'SELECT * FROM orders ORDER BY id',
        'SELECT * FROM vehicles',
        30)
)
SELECT ((1 + (SELECT max(vehicle_id) FROM results) * 2 + 53 * 2) =
    (SELECT count(*) from results)) AS correct_value;
