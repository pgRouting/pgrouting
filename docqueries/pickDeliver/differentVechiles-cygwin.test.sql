

INSERT into vehicles(start_x, start_y, start_open, start_close, capacity, number)
VALUES (40, 50, 0, 400, 200, 1);

UPDATE vehicles SET number = 1;

ALTER TABLE vehicles DROP COLUMN number;

SELECT * INTO results
FROM _pgr_pickDeliverEuclidean(
    'SELECT * FROM orders ORDER BY id',
    'SELECT * FROM vehicles',
    30);


WITH
orders_id AS (
    SELECT id FROM orders
),
results_id AS (
    SELECT DISTINCT order_id AS id
    FROM results
    WHERE stop_type NOT IN (1, 6, -1)
    ORDER BY order_id
)
SELECT count(*)=53 FROM orders_id LEFT JOIN results_id USING(ID);

WITH
the_results AS (
    SELECT *
    FROM results
)
SELECT ((SELECT max(vehicle_number) FROM results) * 2 + 53 * 2 + 1) =
    (SELECT count(*) from the_results) AS correct_value;

-- SELECT * FROM results;
