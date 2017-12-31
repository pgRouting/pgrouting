
\i setup.sql

SELECT plan(2);

SELECT * INTO results
FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT * FROM vehicles$$);

DECLARE q1 CURSOR FOR
SELECT DISTINCT order_id
FROM results
WHERE stop_type IN (-1, 1, 6);

SELECT results_eq(
    'q1'::refcursor,
    ARRAY[ -1 ]::BIGINT[]
);

DECLARE q2 CURSOR FOR
SELECT DISTINCT stop_type FROM results ORDER BY stop_type;

SELECT results_eq(
    'q2'::refcursor,
    ARRAY[ -1, 1, 2, 3, 6 ]::INTEGER[]
);

SELECT finish();
ROLLBACK;
