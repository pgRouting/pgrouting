CREATE OR REPLACE FUNCTION traversal_types_check(
  fn TEXT,
  opt_names TEXT[] DEFAULT '{directed,max_depth}'::TEXT[],
  opt_types TEXT[] DEFAULT '{bool,int8}'::TEXT[],
  created_v TEXT DEFAULT '3.0.0',
  standard_v TEXT default '3.0.0')
RETURNS SETOF TEXT AS
$BODY$
DECLARE
  out_names TEXT[] := '{seq,depth,start_vid,pred,node,edge,cost,agg_cost}'::TEXT[];
  out_types TEXT[] := '{int8,int8,int8,int8,int8,int8,float8,float8}'::TEXT[];
  extra_name TEXT[] := '{}'::TEXT[];
  extra_type TEXT[] := '{}'::TEXT[];
  taptypes TEXT[];
  args TEXT;
  types TEXT;
BEGIN

  IF NOT min_version(created_v) THEN
    RETURN QUERY SELECT skip(1, fn || ': Created on version ' || created_v);
    RETURN;
  END IF;

  RETURN QUERY SELECT has_function(fn);

  IF fn ilike '%breadth%' AND NOT min_version(standard_v)THEN
    opt_names := '{max_depth,directed}'::TEXT[];
    opt_types := '{int8,bool}'::TEXT[];
  END IF;

  IF fn ilike '%withpoints%' THEN
    extra_name := extra_name || '{""}'::TEXT[];
    extra_type := extra_type || '{text}'::TEXT[];

    IF NOT min_version('3.6.0') THEN

      RETURN QUERY SELECT skip(1, fn || ': All signatures on version ' || created_v);
      RETURN;

    ELSIF NOT min_version('4.0.0') THEN

      opt_names := '{"",directed,driving_side,details}'::TEXT[];
      opt_types := '{float8,bool,bpchar,bool}'::TEXT[];

    END IF;

  END IF;

  taptypes := array_replace(array_replace(array_replace
             (array_replace(array_replace(opt_types,
      'bool', 'boolean'),
      'int8', 'bigint'),
      'int4', 'integer'),
      'float8', 'double precision'),
    'bpchar', 'character');

  RETURN QUERY SELECT has_function(fn, extra_type || '{text,bigint}' || taptypes);
  RETURN QUERY SELECT function_returns(fn, extra_type || '{text,bigint}' || taptypes, 'setof record');

  IF fn = 'pgr_withpointsdd' OR fn = 'pgr_drivingdistance' THEN
    -- equicost
    RETURN QUERY SELECT has_function(fn, extra_type || '{text,anyarray}' || taptypes || '{boolean}'::TEXT[]);
    RETURN QUERY SELECT function_returns(fn, extra_type || '{text,anyarray}' || taptypes || '{boolean}'::TEXT[], 'setof record');
  ELSE
    RETURN QUERY SELECT has_function(fn, extra_type || '{text,anyarray}' || taptypes);
    RETURN QUERY SELECT function_returns(fn, extra_type || '{text,anyarray}' || taptypes, 'setof record');
  END IF;

  IF NOT min_version(standard_v) THEN
    RETURN QUERY SELECT skip(1, fn || ': Standardized on ' || standard_v || ', skipping non standardized signatures');
    RETURN;
  END IF;

  if fn = 'pgr_withpointsdd' OR fn = 'pgr_drivingdistance' THEN

    args :=  format($$VALUES
      ('%1$s'::TEXT[] || '{"",""}'::TEXT[] || '%2$s'::TEXT[] || '%3$s'::TEXT[]),
      ('%1$s'::TEXT[] || '{"",""}'::TEXT[] || '%2$s'::TEXT[] || '{equicost}'::TEXT[] || '%3$s'::TEXT[])
      $$,extra_name, opt_names, out_names, opt_names);

    types :=  format($$VALUES
      ('%1$s'::TEXT[] || '{text,int8}'::TEXT[] || '%4$s'::TEXT[] || '%3$s'::TEXT[]),
      ('%1$s'::TEXT[] || '{text,anyarray}'::TEXT[] || '%2$s'::TEXT[] || '{bool}'::TEXT[] || '%3$s'::TEXT[])
      $$, extra_type, opt_types, out_types, opt_types);

  ELSE
    args :=  format($$VALUES
      ('%1$s'::TEXT[] || '{"",""}'::TEXT[] || '%2$s'::TEXT[] || '%3$s'::TEXT[])
      $$,extra_name, opt_names, out_names, opt_names);

    types :=  format($$VALUES
      ('%1$s'::TEXT[] || '{text,int8}'::TEXT[] || '%4$s'::TEXT[] || '%3$s'::TEXT[]),
      ('%1$s'::TEXT[] || '{text,anyarray}'::TEXT[] || '%2$s'::TEXT[] || '%3$s'::TEXT[])
      $$, extra_type, opt_types, out_types, opt_types);

  END IF;

  IF fn ilike '%dd%' THEN

    RETURN QUERY SELECT function_args_has(fn, args);
    RETURN QUERY SELECT function_types_has(fn, types);

  ELSIF min_version('4.0.0') THEN

    RETURN QUERY SELECT function_args_eq(fn, args);
    RETURN QUERY SELECT function_types_eq(fn, types);

  ELSE

    RETURN QUERY SELECT function_args_has(fn, args);
    RETURN QUERY SELECT function_types_has(fn, types);

  END IF;

END;
$BODY$
LANGUAGE plpgsql VOLATILE;


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

  IF fn ilike '%trsp%' THEN
    extra_name := '{""}'::TEXT[];
    extra_type := '{text}'::TEXT[];
  END IF;

  IF fn ilike '%withPoints%' THEN
    extra_name := extra_name || '{""}'::TEXT[];
    extra_type := extra_type || '{text}'::TEXT[];

    IF NOT min_version('4.0.0') THEN
      opt_names := '{directed,driving_side,details}'::TEXT[];
      opt_types := '{bool,bpchar,bool}'::TEXT[];
    END IF;
  END IF;

  IF fn ilike '%near%' THEN
    bounds := bounds - 1;
  END IF;

  taptypes := array_replace(array_replace(array_replace
             (array_replace(array_replace(opt_types,
      'bool', 'boolean'),
      'int8', 'bigint'),
      'int4', 'integer'),
      'float8', 'double precision'),
    'bpchar', 'character');

  RETURN QUERY SELECT has_function(fn);

  IF NOT fn ilike '%near%' THEN
    RETURN QUERY SELECT has_function(fn, extra_type || '{text,bigint,bigint}' || taptypes);
  END IF;

  RETURN QUERY SELECT has_function(fn, extra_type || '{text,bigint,anyarray}' || taptypes[1:bounds]);
  RETURN QUERY SELECT has_function(fn, extra_type || '{text,anyarray,bigint}' || taptypes[1:bounds]);
  RETURN QUERY SELECT has_function(fn, extra_type || '{text,anyarray,anyarray}' || taptypes);
  IF min_version('3.2.0') THEN RETURN QUERY SELECT has_function(fn, extra_type || '{text,text}' || taptypes); END IF;

  IF NOT fn ilike '%near%' THEN
    RETURN QUERY SELECT function_returns(fn, extra_type || '{text,bigint,bigint}' || taptypes[1:bounds], 'setof record');
  END IF;
  RETURN QUERY SELECT function_returns(fn, extra_type || '{text,bigint,anyarray}' || taptypes[1:bounds], 'setof record');
  RETURN QUERY SELECT function_returns(fn, extra_type || '{text,anyarray,bigint}' || taptypes[1:bounds], 'setof record');
  RETURN QUERY SELECT function_returns(fn, extra_type || '{text,anyarray,anyarray}' || taptypes, 'setof record');
  IF min_version('3.2.0') THEN RETURN QUERY SELECT function_returns(fn, extra_type || '{text,text}' || taptypes, 'setof record'); END IF;


  IF NOT min_version(standard_v) THEN
    RETURN QUERY SELECT skip(1, fn || ': Standardized on ' || standard_v || ', skipping non standardized signatures');
    RETURN;
  END IF;

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


CREATE OR REPLACE FUNCTION cost_types_check(
  fn TEXT,
  opt_names TEXT[] DEFAULT '{directed}'::TEXT[],
  opt_types TEXT[] DEFAULT '{bool}'::TEXT[],
  created_v TEXT DEFAULT '3.0.0',
  standard_v TEXT default '3.0.0')
RETURNS SETOF TEXT AS
$BODY$
DECLARE
  out_names TEXT[] := '{start_vid,end_vid,agg_cost}'::TEXT[];
  out_types TEXT[] := '{int8,int8,float8}'::TEXT[];
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

  IF fn ilike '%trsp%' THEN
    extra_name := '{""}'::TEXT[];
    extra_type := '{text}'::TEXT[];
  END IF;

  IF fn ilike '%withPoints%' THEN
    extra_name := extra_name || '{""}'::TEXT[];
    extra_type := extra_type || '{text}'::TEXT[];

    IF NOT min_version('4.0.0') THEN
      opt_names := '{directed,driving_side}'::TEXT[];
      opt_types := '{bool,bpchar}'::TEXT[];
    END IF;
  END IF;

  IF fn ilike '%near%' THEN
    bounds := bounds - 1;
  END IF;

  taptypes := array_replace(array_replace(array_replace
             (array_replace(array_replace(opt_types,
      'bool', 'boolean'),
      'int8', 'bigint'),
      'int4', 'integer'),
      'float8', 'double precision'),
    'bpchar', 'character');

  RETURN QUERY SELECT has_function(fn);

  IF fn ilike '%matrix%' THEN

    RETURN QUERY SELECT has_function(fn, extra_type || '{text,anyarray}' || taptypes[1:bounds]);
    RETURN QUERY SELECT function_returns(fn, extra_type || '{text,anyarray}' || taptypes[1:bounds], 'setof record');

  ELSE

  IF NOT fn ilike '%near%' THEN
    RETURN QUERY SELECT has_function(fn, extra_type || '{text,bigint,bigint}' || taptypes);
  END IF;

  RETURN QUERY SELECT has_function(fn, extra_type || '{text,bigint,anyarray}' || taptypes[1:bounds]);
  RETURN QUERY SELECT has_function(fn, extra_type || '{text,anyarray,bigint}' || taptypes[1:bounds]);
  RETURN QUERY SELECT has_function(fn, extra_type || '{text,anyarray,anyarray}' || taptypes);
  IF min_version('3.2.0') THEN RETURN QUERY SELECT has_function(fn, extra_type || '{text,text}' || taptypes); END IF;

  IF NOT fn ilike '%near%' THEN
    RETURN QUERY SELECT function_returns(fn, extra_type || '{text,bigint,bigint}' || taptypes[1:bounds], 'setof record');
  END IF;
  RETURN QUERY SELECT function_returns(fn, extra_type || '{text,bigint,anyarray}' || taptypes[1:bounds], 'setof record');
  RETURN QUERY SELECT function_returns(fn, extra_type || '{text,anyarray,bigint}' || taptypes[1:bounds], 'setof record');
  RETURN QUERY SELECT function_returns(fn, extra_type || '{text,anyarray,anyarray}' || taptypes, 'setof record');
  IF min_version('3.2.0') THEN RETURN QUERY SELECT function_returns(fn, extra_type || '{text,text}' || taptypes, 'setof record'); END IF;

  END IF;

  IF NOT min_version(standard_v) THEN
    RETURN QUERY SELECT skip(1, fn || ': Standardized on ' || standard_v || ', skipping non standardized signatures');
    RETURN;
  END IF;

  IF fn ilike '%matrix%' THEN

    args :=  format($$VALUES
      ('%1$s'::TEXT[] || '{"",""}'::TEXT[] || '%2$s'::TEXT[] || '%3$s'::TEXT[])
      $$,extra_name, opt_names, out_names);

    types :=  format($$VALUES
      ('%1$s'::TEXT[] || '{text,anyarray}'::TEXT[] || '%2$s'::TEXT[] || '%3$s'::TEXT[])
      $$, extra_type, opt_types, out_types);

  ELSIF fn ilike '%near%' THEN

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

  ELSIF min_version('3.2.0') THEN

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

  ELSE

    args :=  format($$VALUES
      ('%1$s'::TEXT[] || '{"","",""}'::TEXT[] || '%2$s'::TEXT[] || '%3$s'::TEXT[])
      $$,extra_name, opt_names, out_names);

    types :=  format($$VALUES
      ('%1$s'::TEXT[] || '{text,int8,int8}'::TEXT[] || '%2$s'::TEXT[] || '%3$s'::TEXT[]),
      ('%1$s'::TEXT[] || '{text,int8,anyarray}'::TEXT[] || '%2$s'::TEXT[] || '%3$s'::TEXT[]),
      ('%1$s'::TEXT[] || '{text,anyarray,int8}'::TEXT[] || '%2$s'::TEXT[] || '%3$s'::TEXT[]),
      ('%1$s'::TEXT[] || '{text,anyarray,anyarray}'::TEXT[] || '%2$s'::TEXT[] || '%3$s'::TEXT[])
      $$, extra_type, opt_types, out_types);

  END IF;

  RETURN QUERY SELECT CASE WHEN min_version('4.0.0') THEN function_args_has(fn, args) ELSE function_args_has(fn, args) END;
  RETURN QUERY SELECT CASE WHEN min_version('4.0.0') THEN function_types_has(fn, types) ELSE function_types_has(fn, types) END;

END
$BODY$
LANGUAGE plpgsql VOLATILE;





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
