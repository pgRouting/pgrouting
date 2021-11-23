


SELECT * FROM pgr_trsp(
  $$SELECT id, (source+10) AS source, (target+10) AS target, cost::FLOAT FROM edge_table$$,
  $$SELECT 1 AS id, 100::float AS cost, 25::INTEGER AS target_id, ARRAY[33, 32, 25] AS path$$,
  17, 22, false
);

SELECT * FROM _pgr_trsp(
  $$SELECT id, source, target, cost FROM edge_table$$,
  $$ SELECT 1 AS id, ARRAY[4,7] AS path, 100 AS cost $$,
  2, 7, false
);

SELECT * FROM _pgr_trsp(
  $$SELECT id, source, target, cost FROM edge_table$$,
  $$SELECT * FROM new_restrictions$$,
  2, 7, false
);


SELECT * FROM pgr_trsp(
  $$SELECT id, source, target, cost FROM edge_table$$,
  $$SELECT * FROM new_restrictions$$,
  2, 7, false
);
