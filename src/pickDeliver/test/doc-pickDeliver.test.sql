

\echo --q1
SELECT * FROM pgr_pickDeliver(
    'select * from customer order by id'::text, 25, 200, 30)
\echo --q2

