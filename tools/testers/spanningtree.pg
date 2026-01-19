CREATE OR REPLACE FUNCTION spanntree_types_check(fn TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN
  RETURN QUERY SELECT has_function(fn);
  RETURN QUERY SELECT has_function(fn,    ARRAY['text']);
  RETURN QUERY SELECT function_returns(fn, ARRAY['text'], 'setof record');

  RETURN QUERY SELECT function_args_eq(fn,
    $$SELECT '{"","edge","cost"}'::TEXT[] $$
  );

  RETURN QUERY SELECT function_types_eq(fn,
    $$VALUES ('{text,int8,float8}'::TEXT[])$$
  );

END;
$BODY$
LANGUAGE plpgsql VOLATILE;


CREATE OR REPLACE FUNCTION spanntree_dd_types_check(fn TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN

  RETURN QUERY SELECT has_function(fn);

  RETURN QUERY SELECT has_function(fn,  ARRAY['text','bigint','numeric']);
  RETURN QUERY SELECT has_function(fn,  ARRAY['text','anyarray','numeric']);
  RETURN QUERY SELECT has_function(fn,  ARRAY['text','bigint','double precision']);
  RETURN QUERY SELECT has_function(fn,  ARRAY['text','anyarray','double precision']);
  RETURN QUERY SELECT function_returns(fn,  ARRAY['text','bigint','numeric'],  'setof record');
  RETURN QUERY SELECT function_returns(fn,  ARRAY['text','anyarray','numeric'],  'setof record');
  RETURN QUERY SELECT function_returns(fn,  ARRAY['text','bigint','double precision'],  'setof record');
  RETURN QUERY SELECT function_returns(fn,  ARRAY['text','anyarray','double precision'],  'setof record');

  IF NOT min_version('3.7.0') THEN

  RETURN QUERY SELECT function_args_eq(fn,
    $$VALUES
    ('{"","","","seq","depth","start_vid","node","edge","cost","agg_cost"}'::TEXT[])
    $$
  );

  RETURN QUERY SELECT function_types_eq(fn,
    $$VALUES
    ('{text,int8,float8,int8,int8,int8,int8,int8,float8,float8}'::TEXT[]),
    ('{text,anyarray,float8,int8,int8,int8,int8,int8,float8,float8}'::TEXT[]),
    ('{text,int8,numeric,int8,int8,int8,int8,int8,float8,float8}'::TEXT[]),
    ('{text,anyarray,numeric,int8,int8,int8,int8,int8,float8,float8}'::TEXT[]),
    ('{text,int8,float8,int8,int8,int8,int8,int8,float8,float8}'::TEXT[]),
    ('{text,anyarray,float8,int8,int8,int8,int8,int8,float8,float8}'::TEXT[])
    $$
  );

  ELSE

  RETURN QUERY SELECT function_args_eq(fn,
    $$VALUES
    ('{"","","","seq","depth","start_vid","pred","node","edge","cost","agg_cost"}'::TEXT[])
    $$
  );

  RETURN QUERY SELECT function_types_eq(fn,
    $$VALUES
    ('{text,int8,float8,int8,int8,int8,int8,int8,int8,float8,float8}'::TEXT[]),
    ('{text,anyarray,float8,int8,int8,int8,int8,int8,int8,float8,float8}'::TEXT[]),
    ('{text,int8,numeric,int8,int8,int8,int8,int8,int8,float8,float8}'::TEXT[]),
    ('{text,anyarray,numeric,int8,int8,int8,int8,int8,int8,float8,float8}'::TEXT[]),
    ('{text,int8,float8,int8,int8,int8,int8,int8,int8,float8,float8}'::TEXT[]),
    ('{text,anyarray,float8,int8,int8,int8,int8,int8,int8,float8,float8}'::TEXT[])
    $$
  );

  END IF;
END;
$BODY$
LANGUAGE plpgsql VOLATILE;

