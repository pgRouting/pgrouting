-------------------------------------
-------------------------------------
-- tests to without restrictions
-------------------------------------
-------------------------------------

-------------------------------------
-- directed  with reverse_cost
-------------------------------------
SELECT * FROM pgr_trspVia(
  'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
  $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$,
  ARRAY[2, 7, 11]::INTEGER[],     -- array of vids
  true  -- directed graph?
);

-------------------------------------
-- directed  without reverse_cost
-------------------------------------
SELECT * FROM pgr_trspVia(
  'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
  $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$,
  ARRAY[2, 7, 11]::INTEGER[],     -- array of vids
  true  -- directed graph?
);

-------------------------------------
-- undirected  with reverse_cost
-------------------------------------
SELECT * FROM pgr_trspVia(
  'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost, reverse_cost FROM edge_table',
  $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$,
  ARRAY[2, 7, 11]::INTEGER[],     -- array of vids
  false  -- directed graph?
);

-------------------------------------
-- undirected  without reverse_cost
-------------------------------------
SELECT * FROM pgr_trspVia(
  'SELECT id::INTEGER, source::INTEGER, target::INTEGER, cost FROM edge_table',
  $$SELECT 100::float AS to_cost, 25::INTEGER AS target_id, '32, 33'::TEXT AS via_path$$,
  ARRAY[2, 7, 11]::INTEGER[],     -- array of vids
  false  -- directed graph?
);
