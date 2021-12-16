

CREATE TABLE routing (id serial, length integer, source integer, target integer, reverse_cost integer);
INSERT INTO routing (length, source, target, reverse_cost) VALUES
(10000, 1, 2, 10000),
(5, 3, 1, 5),
(10, 4, 3, 10),
(10, 2, 4, 10);

SELECT * FROM pgr_trsp_withPoints(
  $$SELECT id, source, target, length as cost, length as reverse_cost FROM routing$$,
  $$SELECT * FROM new_restrictions WHERE id > 9$$,

  $$(SELECT 1 AS pid, 1 AS edge_id, 0.1 AS fraction)
  UNION
  (SELECT 2, 1, 0.9)$$,
  -1, -2, false
);

SELECT * FROM pgr_withPoints(
  $$SELECT id, source, target, length as cost, length as reverse_cost FROM routing$$,

  $$(SELECT 1 AS pid, 1 AS edge_id, 0.1 AS fraction)
  UNION
  (SELECT 2, 1, 0.9)$$,
  -1, -2, false
);
