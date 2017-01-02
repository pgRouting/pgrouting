/*
TODO code to create the eucledian matrix table and to modify the orders
*/

\echo --q1
SELECT * FROM _pgr_pickDeliver(
    'SELECT * FROM orders ORDER BY id',
    'SELECT * from vehicles',
    'SELECT * from dist_matrix',
    30);

\echo --q2
