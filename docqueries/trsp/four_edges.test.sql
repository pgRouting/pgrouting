SET extra_float_digits=-3;
CREATE TABLE four_edges (
    id BIGINT,
    source BIGINT,
    target BIGINT,
    cost FLOAT,
    reverse_cost FLOAT);

INSERT INTO four_edges (id, source, target, cost, reverse_cost) VALUES
( 719 ,  52163 , -56570 , 179.400001 ,   179.400001),
( 717 ,  52222 ,  52163 , 977.000001 ,   977.000001),
( 718 ,  52220 ,  52222 , 961.000001 ,   961.000001),
( 716 , -21019 ,  52220 , 992.000001 ,   992.000001);

----

SELECT * FROM pgr_trsp_withPoints(
  $$SELECT * FROM
    (VALUES
      (719, 52163, -56570, 179.400001, 179.400001),
      (717, 52222, 52163, 977.000001, 977.000001),
      (718, 52220, 52222, 961.000001, 961.000001),
      (716, -21019, 52220, 992.000001, 992.000001)
    ) AS t (id, source, target, cost, reverse_cost)$$,
  $$SELECT * FROM new_restrictions WHERE id > 9$$,
  $$(SELECT 1 AS pid, 719 AS edge_id, 0::float  AS fraction)
  UNION
  (SELECT 2, 718, 0.5)$$,
  (SELECT source  FROM four_edges WHERE id = 719),
  (SELECT source  FROM four_edges WHERE id = 718),
  true);

SELECT * FROM pgr_trsp_withPoints(
  $$SELECT * FROM four_edges$$,
  $$SELECT * FROM new_restrictions WHERE id > 9$$,
  $$(SELECT 1 AS pid, 719 AS edge_id, 0::float  AS fraction)
  UNION
  (SELECT 2, 718, 0.5)$$,
  (SELECT source  FROM four_edges WHERE id = 719),
  (SELECT source  FROM four_edges WHERE id = 718),
  true);


SELECT * FROM pgr_trsp(
  $$SELECT * FROM four_edges$$,
  $$SELECT * FROM new_restrictions WHERE id > 9$$,
  (SELECT source  FROM four_edges WHERE id = 719),
  (SELECT source  FROM four_edges WHERE id = 718),
  true);

SELECT * FROM pgr_trsp(
  $$SELECT * FROM
    (VALUES
      (719, 52163, -56570, 179.40000, 179.40000),
      (717, 52222, 52163, 977.00000, 977.00000),
      (718, 52220, 52222, 961.00000, 961.00000),
      (716, -21019, 52220, 992.00000, 992.00000)
    ) AS t (id, source, target, cost, reverse_cost)$$,
  $$SELECT * FROM new_restrictions WHERE id > 9$$,
  (SELECT source  FROM four_edges WHERE id = 719),
  (SELECT source  FROM four_edges WHERE id = 718),
  true);

SELECT * FROM pgr_dijkstra(
  $$SELECT * FROM four_edges$$,
  52163,
  52220,
  true);

SELECT * FROM pgr_dijkstra(
  $$SELECT * FROM four_edges$$,
  (SELECT source  FROM four_edges WHERE id = 719),
  (SELECT source  FROM four_edges WHERE id = 718),
  true);

----

SELECT * FROM pgr_trsp_withPoints(
  $$SELECT * FROM four_edges$$,
  $$SELECT * FROM new_restrictions WHERE id > 9$$,
  $$SELECT 719 AS edge_id, 0.5::float AS fraction$$,
  -1::BIGINT,
  (SELECT source  FROM four_edges WHERE id = 718),
  true);

SELECT * FROM pgr_withPoints(
  $$SELECT * FROM four_edges$$,
  $$SELECT 719 AS edge_id, 0.5::float AS fraction$$,
  -1::BIGINT,
  (SELECT source  FROM four_edges WHERE id = 718),
  true);

----

SELECT * FROM pgr_withPoints(
  $$SELECT * FROM four_edges$$,
  $$(SELECT 1 AS pid, 719 AS edge_id, 0.5::float AS fraction)
    UNION
    (SELECT 2, 718, 0)$$,
  -1, -2, true);

SELECT * FROM pgr_trsp_withPoints(
  $$SELECT * FROM four_edges$$,
  $$SELECT * FROM new_restrictions WHERE id > 9$$,
  $$(SELECT 1 AS pid, 719 AS edge_id, 0::float  AS fraction)
    UNION
    (SELECT 2, 718, 0.5)$$,
  -1, -2, true);

----

SELECT * FROM pgr_withPoints(
  $$SELECT * FROM four_edges$$,
  $$SELECT 718 AS edge_id, 0.5::float AS fraction$$,
  (SELECT source  FROM four_edges WHERE id = 719),
  -1::BIGINT,
  true);

SELECT * FROM pgr_trsp_withPoints(
  $$SELECT * FROM four_edges$$,
  $$SELECT * FROM new_restrictions WHERE id > 9$$,
  $$SELECT 718 AS edge_id, 0.5::float AS fraction$$,
  (SELECT source  FROM four_edges WHERE id = 719),
  -1::BIGINT,
  true);


----

SELECT * FROM pgr_withPoints(
  $$SELECT * FROM four_edges$$,
  $$(SELECT 719 AS edge_id, 0::float AS fraction)
    UNION
  (SELECT 718, 0.5)$$,
  -1, -2,
  true);

SELECT * FROM pgr_trsp_withPoints(
  $$SELECT * FROM four_edges$$,
  $$SELECT * FROM new_restrictions WHERE id > 9$$,
  $$(SELECT 719 AS edge_id, 0.5::float AS fraction)
    UNION
  (SELECT 718, 0.5)$$,
  -1, -2,
  true);
