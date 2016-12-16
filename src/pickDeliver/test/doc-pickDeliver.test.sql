
WITH
pickups AS (
    SELECT id, demand, x as pick_x, y as pick_y, opentime as pick_open, closetime as pick_close, servicetime as pick_service
    FROM  customer where pindex = 0
),
deliveries AS (
    SELECT pindex AS id, x as deliver_x, y as deliver_y, opentime as deliver_open, closetime as deliver_close, servicetime as deliver_service
    FROM  customer where dindex = 0
)
SELECT * INTO orders
FROM pickups JOIN deliveries USING(id) ORDER BY pickups.id;


\echo --q1
SELECT * FROM _pgr_pickDeliver(
    'SELECT * FROM orders ORDER BY id', 25, 200, 1, 30);
\echo --q2
SELECT * FROM _pgr_pickDeliver(
    'SELECT * FROM orders ORDER BY id', 25, 200, 10, 30);
\echo --q3
