CREATE OR REPLACE FUNCTION single_path_types_check(
  fn TEXT,
  opt_names TEXT[] DEFAULT '{directed}'::TEXT[],
  opt_types TEXT[] DEFAULT '{bool}'::TEXT[],
  created_v TEXT DEFAULT '3.0.0',
  standard_v TEXT default '3.0.0')
RETURNS SETOF TEXT AS
$BODY$
DECLARE
  out_names TEXT[] := '{seq,path_seq,start_vid,end_vid,node,edge,cost,agg_cost}'::TEXT[];
  out_types TEXT[] := '{int4,int4,int8,int8,int8,int8,float8,float8}'::TEXT[];
  extra_name TEXT[] := '{}'::TEXT[];
  extra_type TEXT[] := '{}'::TEXT[];
  taptypes TEXT[];
  args TEXT;
  types TEXT;
  bounds INTEGER := array_length(opt_names, 1);
BEGIN

  IF NOT min_version(created_v) THEN
    RETURN QUERY SELECT skip(1, fn || ': Created on version ' || created_v);
    RETURN;
  END IF;

  RETURN QUERY SELECT has_function(fn);

  IF NOT min_version(standard_v) THEN
    RETURN QUERY SELECT skip(1, fn || ': Standardized on ' || standard_v || ', skipping non standardized signatures');
    RETURN;
  END IF;

  IF fn ilike '%trsp%' THEN
    extra_name := '{""}'::TEXT[];
    extra_type := '{text}'::TEXT[];
  END IF;

  IF fn ilike '%withPoints%' THEN
    extra_name := extra_name || '{""}'::TEXT[];
    extra_type := extra_type || '{text}'::TEXT[];
  END IF;

  IF fn ilike '%near%' THEN
    bounds := bounds - 1;
  END IF;

  taptypes := array_replace(array_replace(array_replace(opt_types,
      'bool', 'boolean'),
      'int8', 'bigint'),
    'bpchar', 'character');

  IF NOT fn ilike '%near%' THEN
    RETURN QUERY SELECT has_function(fn, extra_type || '{text,bigint,bigint}' || taptypes);
  END IF;

  RETURN QUERY SELECT has_function(fn, extra_type || '{text,bigint,anyarray}' || taptypes[1:bounds]);
  RETURN QUERY SELECT has_function(fn, extra_type || '{text,anyarray,bigint}' || taptypes[1:bounds]);
  RETURN QUERY SELECT has_function(fn, extra_type || '{text,anyarray,anyarray}' || taptypes);
  IF min_version('3.2.0') THEN RETURN QUERY SELECT has_function(fn, extra_type || '{text,text}' || taptypes); END IF;

  IF NOT fn ilike '%near%' THEN
    RETURN QUERY SELECT function_returns(fn, extra_type || '{text,bigint,bigint}' || taptypes, 'setof record');
  END IF;
  RETURN QUERY SELECT function_returns(fn, extra_type || '{text,bigint,anyarray}' || taptypes[1:bounds], 'setof record');
  RETURN QUERY SELECT function_returns(fn, extra_type || '{text,anyarray,bigint}' || taptypes[1:bounds], 'setof record');
  RETURN QUERY SELECT function_returns(fn, extra_type || '{text,anyarray,anyarray}' || taptypes, 'setof record');
  IF min_version('3.2.0') THEN RETURN QUERY SELECT function_returns(fn, extra_type || '{text,text}' || taptypes, 'setof record'); END IF;


  IF fn ilike '%near%' THEN

    args :=  format($$VALUES
      ('%1$s'::TEXT[] || '{"","",""}'::TEXT[] || '%2$s'::TEXT[] || '%3$s'::TEXT[]),
      ('%1$s'::TEXT[] || '{"","",""}'::TEXT[] || '%4$s'::TEXT[] || '%3$s'::TEXT[]),
      ('%1$s'::TEXT[] || '{"",""}'::TEXT[] || '%2$s'::TEXT[] || '%3$s'::TEXT[])
      $$,extra_name, opt_names, out_names, opt_names[1:bounds]);

    types :=  format($$VALUES
      ('%1$s'::TEXT[] || '{text,int8,anyarray}'::TEXT[] || '%4$s'::TEXT[] || '%3$s'::TEXT[]),
      ('%1$s'::TEXT[] || '{text,anyarray,int8}'::TEXT[] || '%4$s'::TEXT[] || '%3$s'::TEXT[]),
      ('%1$s'::TEXT[] || '{text,anyarray,anyarray}'::TEXT[] || '%2$s'::TEXT[] || '%3$s'::TEXT[]),
      ('%1$s'::TEXT[] || '{text,text}'::TEXT[] || '%2$s'::TEXT[] || '%3$s'::TEXT[])
      $$, extra_type, opt_types, out_types, opt_types[1:bounds]);

  ELSE

    args :=  format($$VALUES
      ('%1$s'::TEXT[] || '{"","",""}'::TEXT[] || '%2$s'::TEXT[] || '%3$s'::TEXT[]),
      ('%1$s'::TEXT[] || '{"",""}'::TEXT[] || '%2$s'::TEXT[] || '%3$s'::TEXT[])
      $$,extra_name, opt_names, out_names);

    types :=  format($$VALUES
      ('%1$s'::TEXT[] || '{text,int8,int8}'::TEXT[] || '%2$s'::TEXT[] || '%3$s'::TEXT[]),
      ('%1$s'::TEXT[] || '{text,int8,anyarray}'::TEXT[] || '%2$s'::TEXT[] || '%3$s'::TEXT[]),
      ('%1$s'::TEXT[] || '{text,anyarray,int8}'::TEXT[] || '%2$s'::TEXT[] || '%3$s'::TEXT[]),
      ('%1$s'::TEXT[] || '{text,anyarray,anyarray}'::TEXT[] || '%2$s'::TEXT[] || '%3$s'::TEXT[]),
      ('%1$s'::TEXT[] || '{text,text}'::TEXT[] ||  '%2$s'::TEXT[] || '%3$s'::TEXT[])
      $$, extra_type, opt_types, out_types);
  END IF;

  RETURN QUERY SELECT CASE WHEN min_version('4.0.0') THEN function_args_has(fn, args) ELSE function_args_has(fn, args) END;
  RETURN QUERY SELECT CASE WHEN min_version('4.0.0') THEN function_types_has(fn, types) ELSE function_types_has(fn, types) END;

END
$BODY$
LANGUAGE plpgsql VOLATILE;




CREATE OR REPLACE FUNCTION types_check_general(fn TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
  params_types_words TEXT[] = ARRAY['text'];
  params_names TEXT[] = ARRAY[''];
  params_numbers TEXT[] = ARRAY['text'];

  optional_params_types_words TEXT[] = ARRAY['boolean'];
  optional_params_names TEXT[] = ARRAY['directed'];
  optional_params_numbers TEXT[] = ARRAY['bool'];

  return_params_names TEXT[];
  return_params_numbers TEXT[];
  return_params_names_start TEXT[] = ARRAY['seq','path_seq'];
  return_params_numbers_start TEXT[] = '{int4,int4}'::TEXT[];
  return_params_names_end TEXT[] = ARRAY['node','edge','cost','agg_cost'];
  return_params_numbers_end TEXT[] = '{int8,int8,float8,float8}'::TEXT;

  start_v TEXT = 'start_vid';
  end_v TEXT = 'end_vid';
  startend_v TEXT[] = ARRAY[start_v,end_v];

BEGIN
  IF fn IN ('pgr_trsp', 'pgr_trsp_withpoints') AND NOT min_version('3.4.0') THEN
    RETURN QUERY SELECT skip(1, 'Signature added on 3.4.0');
    RETURN;
  END IF;

  return_params_names = return_params_names_start;
  return_params_numbers = return_params_numbers_start;

  IF fn LIKE '%trsp%' THEN
    params_types_words := params_types_words || 'text'::TEXT;
    params_names := params_names || ARRAY[''];
    params_numbers := params_numbers || ARRAY['text'];
  END IF;

  IF fn = 'pgr_withpoints' THEN
    return_params_names = return_params_names || ARRAY['start_pid','end_pid'];
  ELSE
    return_params_names = return_params_names || ARRAY['start_vid','end_vid'];
    return_params_numbers = return_params_numbers || ARRAY['int8','int8']::TEXT[];
  END IF;
  return_params_names = return_params_names || return_params_names_end;
  return_params_numbers = return_params_numbers || return_params_numbers_end;

  IF fn LIKE '%withpoints%' THEN
    params_types_words := params_types_words || 'text'::TEXT;
    params_names := params_names || ARRAY[''];
    params_numbers := params_numbers || ARRAY['text'];
    optional_params_types_words := optional_params_types_words || ARRAY['character','boolean'];
    optional_params_names := optional_params_names || ARRAY['driving_side','details'];
    optional_params_numbers := optional_params_numbers || ARRAY['bpchar','bool'];
  END IF;


  RETURN QUERY SELECT has_function(fn);

  RETURN QUERY SELECT has_function(fn, params_types_words || ARRAY['bigint','bigint'] || optional_params_types_words);
  RETURN QUERY SELECT has_function(fn, params_types_words || ARRAY['bigint','anyarray'] || optional_params_types_words);
  RETURN QUERY SELECT has_function(fn, params_types_words || ARRAY['anyarray','bigint'] || optional_params_types_words);
  RETURN QUERY SELECT has_function(fn, params_types_words || ARRAY['anyarray','anyarray'] || optional_params_types_words);
  IF (min_version('3.1.0') AND fn = 'pgr_dijkstra') OR (min_version('3.2.0') AND fn = 'pgr_withpoints') OR fn IN ('pgr_trsp','pgr_trsp_withpoints') THEN
    RETURN QUERY SELECT has_function(fn, params_types_words || ARRAY['text'] || optional_params_types_words);
  END IF;

  RETURN QUERY SELECT function_returns(fn, params_types_words || ARRAY['bigint','bigint'] || optional_params_types_words,'setof record');
  RETURN QUERY SELECT function_returns(fn, params_types_words || ARRAY['bigint','anyarray'] || optional_params_types_words,'setof record');
  RETURN QUERY SELECT function_returns(fn, params_types_words || ARRAY['anyarray','bigint'] || optional_params_types_words,'setof record');
  RETURN QUERY SELECT function_returns(fn, params_types_words || ARRAY['anyarray','anyarray'] || optional_params_types_words,'setof record');
  IF (min_version('3.1.0') AND fn = 'pgr_dijkstra') OR (min_version('3.2.0') AND fn = 'pgr_withpoints') OR fn IN ('pgr_trsp','pgr_trsp_withpoints') THEN
    RETURN QUERY SELECT function_returns(fn, params_types_words || ARRAY['text'] || optional_params_types_words,'setof record');
  END IF;


  IF fn = 'pgr_withpoints' THEN
    start_v = 'start_pid';
    end_v = 'end_pid';
    startend_v = ARRAY[start_v,end_v];
  END IF;

  IF fn = 'pgr_dijkstra' OR fn = 'pgr_withpoints' THEN
    RETURN QUERY
    SELECT function_args_eq(fn,
      format('VALUES (%1$L::TEXT[]),(%2$L::TEXT[]),(%3$L::TEXT[]),(%4$L::TEXT[])',
        -- one to one
        '{"' || array_to_string(
          params_names || ARRAY['',''] || optional_params_names || return_params_names_start || return_params_names_end,'","')
        || '"}',
        -- one to many
        '{"' || array_to_string(
          params_names || ARRAY['',''] || optional_params_names || return_params_names_start || end_v || return_params_names_end,'","')
        || '"}',
        -- many to one
        '{"' || array_to_string(
          params_names || ARRAY['',''] || optional_params_names || return_params_names_start || start_v || return_params_names_end,'","')
        || '"}',
        -- many to many
        '{"' || array_to_string(
          params_names || ARRAY['',''] || optional_params_names || return_params_names_start || startend_v || return_params_names_end,'","')
        || '"}'
    ));

    RETURN QUERY
    SELECT function_types_eq(fn,
      format('VALUES (%1$L::TEXT[]),(%2$L::TEXT[]),(%3$L::TEXT[]),(%4$L::TEXT[])',
        -- one to one
        '{' || array_to_string(
          params_numbers || '{int8,int8}'::TEXT[] || optional_params_numbers || return_params_numbers_start || return_params_numbers_end,',')
        || '}',
        -- one to many
        '{' || array_to_string(
          params_numbers || '{int8,anyarray}'::TEXT[] || optional_params_numbers || return_params_numbers_start || 'int8' || return_params_numbers_end,',')
        || '}',
        -- many to one
        '{' || array_to_string(
          params_numbers || '{anyarray,int8}'::TEXT[] || optional_params_numbers || return_params_numbers_start || 'int8'  || return_params_numbers_end,',')
        || '}',
        -- many to many
        '{' || array_to_string(
          params_numbers || '{anyarray,anyarray}'::TEXT[] || optional_params_numbers || return_params_numbers_start || '{int8,int8}'::TEXT[] || return_params_numbers_end,',')
        || '}'
        ));

    IF (min_version('3.1.0') AND fn = 'pgr_dijkstra') OR (min_version('3.2.0') AND fn = 'pgr_withpoints') THEN
      -- combinations
      RETURN QUERY
      SELECT function_args_eq(fn,
        format('VALUES (%1$L::TEXT[])',
          '{"' || array_to_string(
            params_names || ARRAY[''] || optional_params_names || return_params_names_start || startend_v || return_params_names_end,'","')
          || '"}'
        ));

      RETURN QUERY
      SELECT function_types_eq(fn,
        format('VALUES (%1$L::TEXT[])',
          '{' || array_to_string(
            params_numbers || 'text'::TEXT || optional_params_numbers || return_params_numbers_start || '{int8,int8}'::TEXT[] || return_params_numbers_end,',')
          || '}'
      ));

    END IF;
    RETURN;
  END IF;

  -- TODO should be _eq when old signatures are removed
  RETURN QUERY
  SELECT function_args_has(fn,
    format('VALUES (%1$L::TEXT[]),(%2$L::TEXT[])',
      '{"' || array_to_string(
        params_names || ARRAY[''] || optional_params_names || return_params_names,'","')
      || '"}',
      '{"' || array_to_string(
        params_names || ARRAY['',''] || optional_params_names || return_params_names,'","')
      || '"}'
  ));

  RETURN QUERY
  SELECT function_types_has(fn,
    format('VALUES (%1$L::TEXT[]),(%2$L::TEXT[]),(%3$L::TEXT[]),(%4$L::TEXT[])',
      -- one to one
      '{' || array_to_string(
        params_numbers || '{int8,int8}'::TEXT[] || optional_params_numbers || return_params_numbers,',')
      || '}',
      -- one to many
      '{' || array_to_string(
        params_numbers || '{int8,anyarray}'::TEXT[] || optional_params_numbers || return_params_numbers,',')
      || '}',
      -- many to one
      '{' || array_to_string(
        params_numbers || '{anyarray,int8}'::TEXT[] || optional_params_numbers || return_params_numbers,',')
      || '}',
      -- many to many
      '{' || array_to_string(
        params_numbers || '{anyarray,anyarray}'::TEXT[] || optional_params_numbers || return_params_numbers,',')
      || '}',
      -- combinations
      '{' || array_to_string(
        params_numbers || 'text'::TEXT || optional_params_numbers || return_params_numbers,',')
      || '}'
    ));

END;
$BODY$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION types_check_via(fn TEXT)
RETURNS SETOF TEXT AS
$BODY$
DECLARE
  -- edges sql
  params_names TEXT[] = ARRAY[''];
  params_types_words TEXT[] = ARRAY['text'];
  params_types TEXT[] = ARRAY['text'];

  optional_params_names TEXT[] = ARRAY['directed','strict','u_turn_on_edge'];
  optional_params_types_words TEXT[] = ARRAY['boolean','boolean','boolean'];
  optional_params_types TEXT[] = '{bool,bool,bool}'::TEXT[];

  return_params_names TEXT[] = ARRAY['seq','path_id','path_seq','start_vid','end_vid','node','edge','cost','agg_cost','route_agg_cost'];
  return_params_types TEXT[] = '{int4,int4,int4,int8,int8,int8,int8,float8,float8,float8}'::TEXT[];

  names TEXT;
  typs TEXT;
BEGIN

   IF fn ilike '%withpoints%' AND NOT min_version('4.0.0') THEN
     RETURN QUERY SELECT skip(1, fn || ': New signatures on 4.0.0');
     RETURN;
   END IF;

  IF fn IN ('pgr_trspvia') AND NOT min_version('3.4.0') THEN
    RETURN QUERY SELECT skip(1, 'Signature added on 3.4.0');
    RETURN;
  END IF;

  IF fn LIKE '%trsp%' THEN
    -- restrictions sql
    params_names := params_names || ARRAY[''];
    params_types_words := params_types_words || 'text'::TEXT;
    params_types := params_types || ARRAY['text'];
  END IF;

  IF fn LIKE '%withpoints%' THEN
    -- points sql
    params_names := params_names || ARRAY[''];
    params_types_words := params_types_words || 'text'::TEXT;
    params_types := params_types || ARRAY['text'];
    -- points optionals
    optional_params_names := optional_params_names || 'details'::TEXT;
    optional_params_types_words := optional_params_types_words || 'boolean'::TEXT;
    optional_params_types := optional_params_types || 'bool'::TEXT;
  END IF;

  -- vias
  params_names := params_names || ARRAY[''];
  params_types_words := params_types_words || 'anyarray'::TEXT;
  params_types := params_types || 'anyarray'::TEXT;

  IF fn LIKE '%withpoints%' THEN
    -- driving side
    params_names := params_names || ARRAY[''];
    params_types_words := params_types_words || 'character'::TEXT;
    params_types := params_types || 'bpchar'::TEXT;
  END IF;

  RETURN QUERY SELECT has_function(fn);
  RETURN QUERY SELECT has_function(fn, params_types_words || optional_params_types_words);
  RETURN QUERY SELECT function_returns(fn, params_types_words || optional_params_types_words,'setof record');


  RETURN QUERY SELECT function_args_eq(fn,
    format(
      $$VALUES (%1$L::TEXT[])$$,
      -- one via
      '{"' || array_to_string(
        params_names || optional_params_names || return_params_names,'","')
      || '"}'));

  RETURN QUERY SELECT function_types_eq(fn,
    format(
      $$VALUES (%1$L::TEXT[])$$,
      -- one via
      '{"' || array_to_string(
        params_types || optional_params_types || return_params_types,'","')
      || '"}'));


END;
$BODY$
LANGUAGE plpgsql;
