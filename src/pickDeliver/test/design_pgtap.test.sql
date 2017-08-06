
UPDATE vehicles SET start_open = 5, start_close = 4 WHERE id = 1;

SELECT * FROM _pgr_pickDeliverEuclidean(
    $$SELECT * FROM orders$$,
    $$SELECT * FROM vehicles$$);

UPDATE vehicles SET start_open = 0, start_close = 50 WHERE id = 1;

