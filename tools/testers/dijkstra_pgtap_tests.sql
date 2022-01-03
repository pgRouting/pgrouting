CREATE OR REPLACE FUNCTION no_crash_dijkstra(fn_name TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
params TEXT[];
subs TEXT[];
BEGIN

  -- function used on:
  -- pgr_dijkstra,
  -- pgr_dijkstraCost

  PREPARE edges AS
  SELECT id, source, target, cost, reverse_cost  FROM edge_table;

  PREPARE null_ret AS
  SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1);

  PREPARE null_ret_arr AS
  SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1);

  RETURN QUERY SELECT isnt_empty('edges', 'Should be not empty to tests be meaningful');

  RETURN QUERY SELECT is_empty('null_ret', 'Should be empty to tests be meaningful');

  RETURN QUERY SELECT set_eq('null_ret_arr', 'SELECT NULL::BIGINT[]', 'Should be empty to tests be meaningful');


  -- one to one
  params = ARRAY[
  '$$SELECT id, source, target, cost, reverse_cost  FROM edge_table$$'
  ,'1::BIGINT',
  '2::BIGINT'
  ]::TEXT[];
  subs = ARRAY[
  'NULL',
  '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))',
  '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))'
  ]::TEXT[];

  RETURN QUERY SELECT * FROM no_crash_test(fn_name, params, subs);

  subs = ARRAY[
  'NULL',
  'NULL::BIGINT',
  'NULL::BIGINT'
  ]::TEXT[];
  RETURN QUERY SELECT * FROM no_crash_test(fn_name, params, subs);

  -- one to many
  params = ARRAY['$$edges$$','1', 'ARRAY[2,5]::BIGINT[]']::TEXT[];
  subs = ARRAY[
  'NULL',
  '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))',
  '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))'
  ]::TEXT[];

  RETURN QUERY SELECT * FROM no_crash_test(fn_name, params, subs);

  subs = ARRAY[
  'NULL',
  'NULL::BIGINT',
  'NULL::BIGINT[]'
  ]::TEXT[];
  RETURN QUERY SELECT * FROM no_crash_test(fn_name, params, subs);

  -- many to one
  params = ARRAY['$$edges$$', 'ARRAY[2,5]::BIGINT[]', '1']::TEXT[];
  subs = ARRAY[
  'NULL',
  '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))',
  '(SELECT id FROM edge_table_vertices_pgr  WHERE id IN (-1))'
  ]::TEXT[];

  RETURN QUERY SELECT * FROM no_crash_test(fn_name, params, subs);

  subs = ARRAY[
  'NULL',
  'NULL::BIGINT[]',
  'NULL::BIGINT'
  ]::TEXT[];
  RETURN QUERY SELECT * FROM no_crash_test(fn_name, params, subs);

  -- many to many
  params = ARRAY['$$edges$$','ARRAY[1]::BIGINT[]', 'ARRAY[2,5]::BIGINT[]']::TEXT[];
  subs = ARRAY[
  'NULL',
  '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))',
  '(SELECT array_agg(id) FROM edge_table_vertices_pgr  WHERE id IN (-1))'
  ]::TEXT[];

  RETURN QUERY SELECT * FROM no_crash_test(fn_name, params, subs);

  subs = ARRAY[
  'NULL',
  'NULL::BIGINT[]',
  'NULL::BIGINT[]'
  ]::TEXT[];
  RETURN QUERY SELECT * FROM no_crash_test(fn_name, params, subs);

  PREPARE combinations AS
  SELECT source, target  FROM combinations_table;

  PREPARE null_combinations AS
  SELECT source, target FROM combinations_table WHERE false;

  RETURN QUERY SELECT isnt_empty('combinations', 'Should be not empty to tests be meaningful');

  RETURN QUERY SELECT is_empty('null_combinations', 'Should be empty to tests be meaningful');

  -- Combinations SQL
  params = ARRAY['$$edges$$','$$combinations$$']::TEXT[];
  subs = ARRAY[
  'NULL',
  '$$(SELECT source, target FROM combinations_table WHERE false )$$'
  ]::TEXT[];
  RETURN QUERY SELECT * FROM no_crash_test(fn_name, params, subs);

  subs = ARRAY[
  'NULL',
  'NULL::TEXT'
  ]::TEXT[];
  RETURN QUERY SELECT * FROM no_crash_test(fn_name, params, subs);

END
$BODY$
LANGUAGE plpgsql VOLATILE;


CREATE OR REPLACE FUNCTION dijkstra_types_check(fn TEXT) RETURNS SETOF TEXT AS
$BODY$
BEGIN
  RETURN QUERY SELECT has_function(fn);

  RETURN QUERY SELECT has_function(fn, ARRAY['text','bigint','bigint','boolean']);
  RETURN QUERY SELECT has_function(fn, ARRAY['text','bigint','anyarray','boolean']);
  RETURN QUERY SELECT has_function(fn, ARRAY['text','anyarray','bigint','boolean']);
  RETURN QUERY SELECT has_function(fn, ARRAY['text','anyarray','anyarray','boolean']);
  RETURN QUERY SELECT has_function(fn, ARRAY['text','text','boolean']);

  RETURN QUERY SELECT function_returns(fn, ARRAY['text','bigint','bigint','boolean'],'setof record');
  RETURN QUERY SELECT function_returns(fn, ARRAY['text','bigint','anyarray','boolean'],'setof record');
  RETURN QUERY SELECT function_returns(fn, ARRAY['text','anyarray','bigint','boolean'],'setof record');
  RETURN QUERY SELECT function_returns(fn, ARRAY['text','anyarray','anyarray','boolean'],'setof record');
  RETURN QUERY SELECT function_returns(fn, ARRAY['text','text','boolean'],'setof record');

  RETURN QUERY SELECT set_eq(
    format($$SELECT  proargnames from pg_proc where proname = %1$L$$,fn),
    $$VALUES
    ('{"","","","directed","seq","path_seq","node","edge","cost","agg_cost"}'::TEXT[]),
    ('{"","","","directed","seq","path_seq","end_vid","node","edge","cost","agg_cost"}'::TEXT[]),
    ('{"","","","directed","seq","path_seq","start_vid","node","edge","cost","agg_cost"}'::TEXT[]),
    ('{"","","","directed","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[]),
    ('{"","","directed","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[])
    $$);

  RETURN QUERY SELECT set_eq(
    format($$SELECT  proallargtypes from pg_proc where proname = %1$L$$,fn),
    $$VALUES
    ('{25,20,20,16,23,23,20,20,701,701}'::OID[]),
    ('{25,20,2277,16,23,23,20,20,20,701,701}'::OID[]),
    ('{25,2277,20,16,23,23,20,20,20,701,701}'::OID[]),
    ('{25,2277,2277,16,23,23,20,20,20,20,701,701}'::OID[]),
    ('{25,25,16,23,23,20,20,20,20,701,701}'::OID[])
    $$);

END
$BODY$
LANGUAGE plpgsql VOLATILE;
