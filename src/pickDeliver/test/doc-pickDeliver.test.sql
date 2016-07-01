
\echo --q1
SELECT * FROM _pgr_pickDeliver(
    'select * from customer order by id', 25, 200, 1, 30);
\echo --q2
SELECT * FROM _pgr_pickDeliver(
    'select * from customer order by id', 25, 200, 10, 30);
\echo --q3
