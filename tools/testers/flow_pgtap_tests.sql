CREATE OR REPLACE FUNCTION flow_no_crash(fn_name TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN
  IF fn_name = 'pgr_edgeDisjointPaths' THEN
    PREPARE edges AS
    SELECT id, source, target, cost, reverse_cost  FROM edge_table;
  ELSE
    PREPARE edges AS
    SELECT id,
    source,
    target,
    capacity,
    reverse_capacity
    FROM edge_table
    ORDER BY id;
  END IF;



  PREPARE null_ret AS
  SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1);

  PREPARE null_ret_arr AS
  SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1);


  RETURN QUERY
  SELECT isnt_empty('edges', 'Should be not empty to tests be meaningful');
  RETURN QUERY
  SELECT is_empty('null_ret', 'Should be empty to tests be meaningful');
  RETURN QUERY
  SELECT set_eq('null_ret_arr', 'SELECT NULL::BIGINT[]', 'Should be empty to tests be meaningful');

  -- one to one
  params = ARRAY['$$edges$$',
  '1::BIGINT',
  '2::BIGINT'
  ]::TEXT[];
  subs = ARRAY[
  'NULL',
  '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))',
  '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))'
  ]::TEXT[];

  RETURN query SELECT * FROM no_crash_test(fn_name,params, subs);

  subs = ARRAY[
  'NULL',
  'NULL::BIGINT',
  'NULL::BIGINT'
  ]::TEXT[];
  RETURN query SELECT * FROM no_crash_test(fn_name,params, subs);
  -- one to many

  params = ARRAY['$$edges$$',
  '1::BIGINT',
  'ARRAY[2,5]::BIGINT[]'
  ]::TEXT[];
  subs = ARRAY[
  'NULL',
  '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))',
  '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))'
  ]::TEXT[];

  RETURN query SELECT * FROM no_crash_test(fn_name,params, subs);

  subs = ARRAY[
  'NULL',
  'NULL::BIGINT',
  'NULL::BIGINT[]'
  ]::TEXT[];
  RETURN query SELECT * FROM no_crash_test(fn_name,params, subs);

  -- many to one
  params = ARRAY['$$edges$$',
  'ARRAY[2,5]::BIGINT[]',
  '1'
  ]::TEXT[];
  subs = ARRAY[
  'NULL',
  '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))',
  '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))'
  ]::TEXT[];

  RETURN query SELECT * FROM no_crash_test(fn_name,params, subs);

  subs = ARRAY[
  'NULL',
  'NULL::BIGINT[]',
  'NULL::BIGINT'
  ]::TEXT[];
  RETURN query SELECT * FROM no_crash_test(fn_name,params, subs);

  -- many to many
  params = ARRAY['$$edges$$',
  'ARRAY[1]::BIGINT[]',
  'ARRAY[2,5]::BIGINT[]'
  ]::TEXT[];
  subs = ARRAY[
  'NULL',
  '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))',
  '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))'
  ]::TEXT[];

  RETURN query SELECT * FROM no_crash_test(fn_name,params, subs);

  subs = ARRAY[
  'NULL',
  'NULL::BIGINT[]',
  'NULL::BIGINT[]'
  ]::TEXT[];
  RETURN query SELECT * FROM no_crash_test(fn_name,params, subs);

  IF NOT min_version('3.2.0') THEN
    RETURN QUERY
    SELECT skip(1, 'Combinations functionality is new on 3.2.0');
    RETURN;
  END IF;

  PREPARE combinations AS
  SELECT source, target  FROM combinations_table WHERE target > 2;
  PREPARE null_combinations AS
  SELECT source, target FROM combinations_table WHERE source IN (-1);
  RETURN QUERY
  SELECT isnt_empty('combinations', 'Should be not empty to tests be meaningful');
  RETURN QUERY
  SELECT is_empty('null_combinations', 'Should be empty to tests be meaningful');

  params = ARRAY['$$edges$$', '$$combinations$$']::TEXT[];
  subs = ARRAY[
  'NULL',
  '$$(SELECT source, target FROM combinations_table  WHERE source IN (-1))$$'
  ]::TEXT[];

  RETURN query SELECT * FROM no_crash_test(fn_name, params, subs);

  subs = ARRAY[
  'NULL',
  'NULL::TEXT'
  ]::TEXT[];
  RETURN query SELECT * FROM no_crash_test(fn_name, params, subs);

END
$BODY$
LANGUAGE plpgsql VOLATILE;

