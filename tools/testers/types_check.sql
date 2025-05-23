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

  -- TODO should be set_eq when old signatures are removed
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
  params_types_words TEXT[] = ARRAY['text'];
  params_names TEXT[] = ARRAY[''];
  params_numbers TEXT[] = ARRAY['text'];

  optional_params_types_words TEXT[] = ARRAY['boolean','boolean','boolean'];
  optional_params_names TEXT[] = ARRAY['directed','strict','u_turn_on_edge'];
  optional_params_numbers TEXT[] = '{bool,bool,bool}'::TEXT[];

  return_params_names TEXT[] = ARRAY['seq','path_id','path_seq','start_vid','end_vid','node','edge','cost','agg_cost','route_agg_cost'];
  return_params_numbers TEXT[] = '{int4,int4,int4,int8,int8,int8,int8,float8,float8,float8}'::TEXT[];

BEGIN
  IF fn IN ('pgr_trspvia','pgr_trspvia_withpoints','pgr_withpointsvia') AND NOT min_version('3.4.0') THEN
    RETURN QUERY SELECT skip(1, 'Signature added on 3.4.0');
    RETURN;
  END IF;

  IF fn LIKE '%trsp%' THEN
    -- restrictions sql
    params_types_words := params_types_words || 'text'::TEXT;
    params_names := params_names || ARRAY[''];
    params_numbers := params_numbers || ARRAY['text'];
  END IF;

  IF fn LIKE '%withpoints%' THEN
    -- points sql
    params_types_words := params_types_words || 'text'::TEXT;
    params_names := params_names || ARRAY[''];
    params_numbers := params_numbers || ARRAY['text'];
    -- points optionals
    optional_params_types_words := optional_params_types_words || ARRAY['character','boolean'];
    optional_params_names := optional_params_names || ARRAY['driving_side','details'];
    optional_params_numbers := optional_params_numbers || '{bpchar,bool}'::TEXT[];
  END IF;

  -- vias
  params_types_words := params_types_words || 'anyarray'::TEXT;
  params_names := params_names || ARRAY[''];
  params_numbers := params_numbers || ARRAY['anyarray'];

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
        params_numbers || optional_params_numbers || return_params_numbers,'","')
      || '"}'));


END;
$BODY$
LANGUAGE plpgsql;
