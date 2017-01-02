/*
TODO code to create the eucledian matrix table and to modify the orders
*/

\echo --q1
SELECT * FROM _pgr_pickDeliver(
    'SELECT * FROM orders ORDER BY id',
    'SELECT 0 AS id,
        40 AS start_x, 50 AS start_y,
        0 AS start_open, 1236 AS start_close,
        200 AS capacity, 25 AS number',
    'SELECT * from dist_matrix',
    30);

\echo --q2
