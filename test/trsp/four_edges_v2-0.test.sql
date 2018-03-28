

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

SELECT * FROM pgr_trsp(
  $$SELECT
    id::int, source::int, target::int, cost::float, reverse_cost::float
  FROM
    (VALUES
      (719, 52163, -56570, 179.400001, 179.400001),
      (717, 52222, 52163, 977.000001, 977.000001),
      (718, 52220, 52222, 961.000001, 961.000001),
      (716, -21019, 52220, 992.000001, 992.000001)
    ) AS t (id, source, target, cost, reverse_cost)$$,
  719, 0,
  718, 0,
  true, true, NULL);

 
SELECT * FROM pgr_trsp(
  $$SELECT
    id::int, source::int, target::int, cost::float, reverse_cost::float
  FROM
    (VALUES
      (719, 52163, -56570, 179.40000, 179.40000),
      (717, 52222, 52163, 977.00000, 977.00000),
      (718, 52220, 52222, 961.00000, 961.00000),
      (716, -21019, 52220, 992.00000, 992.00000)
    ) AS t (id, source, target, cost, reverse_cost)$$,
  (SELECT source::int  FROM four_edges WHERE id = 719),
  (SELECT source::int  FROM four_edges WHERE id = 718),
  true, true, NULL);

 
SELECT * FROM pgr_trsp(
  $$SELECT
    id::int, source::int, target::int, cost::float, reverse_cost::float
  FROM four_edges$$,
  719, 0,
  718, 0,
  true, true, NULL);

SELECT * FROM pgr_dijkstra(
  $$SELECT
    id::int, source::int, target::int, cost::float, reverse_cost::float
  FROM four_edges$$,
  52163,
  52220,
  true, true);

SELECT * FROM pgr_dijkstra(
  $$SELECT
    id::int, source::int, target::int, cost::float, reverse_cost::float
  FROM four_edges$$,
  (SELECT source::int  FROM four_edges WHERE id = 719),
  (SELECT source::int  FROM four_edges WHERE id = 718),
  true, true);

