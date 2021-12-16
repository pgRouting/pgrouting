
CREATE TABLE maptest (
    id BIGINT,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT);

INSERT INTO maptest (id, source, target, cost, reverse_cost) VALUES
(671222, 318255, 222698,  14.02,  14.02),
(671223, 222698,  36655, 197.16, 197.16),
(582877, 408918,   5556, 458.09, 458.09),
(582876, 318255, 408918,   3.89,   3.89),
(585280,   5556, 454424,  54.84,  54.84);

----

SELECT * FROM pgr_trsp_withPoints(
  $$SELECT * FROM maptest$$,
  $$SELECT * FROM new_restrictions WHERE id > 9$$,
  $$(SELECT 1 AS pid, 582877 AS edge_id, 0 AS fraction)
  UNION
  (SELECT 2, 582877, 0.5)$$,

  (SELECT source FROM maptest WHERE id = 582877),
  -2::BIGINT,
  true
);

SELECT * FROM pgr_withPoints(
  $$SELECT * FROM maptest$$,
  $$(SELECT 1 AS pid, 582877 AS edge_id, 0 AS fraction)
  UNION
  (SELECT 2, 582877, 0.5)$$,

  (SELECT source FROM maptest WHERE id = 582877),
  -2::BIGINT,
  true
);

----

SELECT * FROM pgr_trsp_withPoints(
  $$SELECT * FROM maptest$$,
  $$SELECT * FROM new_restrictions WHERE id > 9$$,
  $$(SELECT 1 AS pid, 582877 AS edge_id, 0.0000000001 AS fraction)
  UNION
  (SELECT 2, 582877, 0.5)$$,

  -1,
  -2,
  true
);

SELECT * FROM pgr_withPoints(
  $$SELECT * FROM maptest$$,
  $$(SELECT 1 AS pid, 582877 AS edge_id, 0.0000000001 AS fraction)
  UNION
  (SELECT 2, 582877, 0.5)$$,

  -1,
  -2,
  true
);

