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

CREATE OR REPLACE FUNCTION types_check_via(fn TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
  params_types_words TEXT[] = ARRAY['text'];
  params_names TEXT[] = ARRAY[''];
  params_numbers OID[] = ARRAY[25];

  optional_params_types_words TEXT[] = ARRAY['boolean','boolean','boolean'];
  optional_params_names TEXT[] = ARRAY['directed','strict','u_turn_on_edge'];
  optional_params_numbers OID[] = ARRAY[16,16,16];

  return_params_names TEXT[] = ARRAY['seq','path_id','path_seq'];
  return_params_numbers OID[] = ARRAY[23,23,23,20,20,20,20,701,701,701];

BEGIN
  -- edges, restrictions, points, vias
  IF fn LIKE '%trsp%' THEN
    params_types_words := params_types_words || 'text'::TEXT;
    params_names := params_names || ARRAY[''];
    params_numbers := params_numbers || ARRAY[25::OID];
  END IF;

  IF fn LIKE '%withpoints%' THEN
    params_types_words := params_types_words || 'text'::TEXT;
    params_names := params_names || ARRAY[''];
    params_numbers := params_numbers || ARRAY[25::OID];
    optional_params_types_words := optional_params_types_words || ARRAY['boolean','character'];
    optional_params_names := optional_params_names || ARRAY['details','driving_side'];
    optional_params_numbers := optional_params_numbers || ARRAY[16::OID, 1042::OID];
    return_params_names = return_params_names || ARRAY['start_pid','end_pid'];
  ELSE
    return_params_names = return_params_names || ARRAY['start_vid','end_vid'];
  END IF;

  params_types_words := params_types_words || 'anyarray'::TEXT;
  params_names := params_names || ARRAY[''];
  params_numbers := params_numbers || ARRAY[2277::OID];
  return_params_names = return_params_names || ARRAY['node','edge','cost','agg_cost','route_agg_cost'];

  RETURN QUERY SELECT has_function(fn);

  RETURN QUERY SELECT has_function(fn, params_types_words || optional_params_types_words);
  RETURN QUERY SELECT function_returns(fn, params_types_words || optional_params_types_words, 'setof record');

  RETURN QUERY
  SELECT set_eq(
    format($$SELECT proargnames from pg_proc where proname = %1$L$$, fn),
    format('VALUES (%1$L::TEXT[])', '{"' || array_to_string(params_names || optional_params_names || return_params_names,'","') || '"}')
  );

  RETURN QUERY
  SELECT set_eq(
    format($$SELECT proallargtypes from pg_proc where proname = %1$L$$, fn),
    format('VALUES (%1$L::OID[])', '{' || array_to_string(params_numbers || optional_params_numbers || return_params_numbers,',') || '}')
  );
END;
$BODY$
LANGUAGE plpgsql;
