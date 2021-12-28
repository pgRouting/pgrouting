CREATE OR REPLACE FUNCTION spanntree_types_check(fn TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN
  RETURN QUERY SELECT has_function(fn);
  RETURN QUERY SELECT has_function(fn,    ARRAY['text']);
  RETURN QUERY SELECT function_returns(fn, ARRAY['text'], 'setof record');

  RETURN QUERY SELECT set_eq(
    format($$SELECT  proargnames FROM pg_proc WHERE proname = %1$L$$,fn),
    $$SELECT '{"","edge","cost"}'::TEXT[] $$
  );

  RETURN QUERY SELECT set_eq(
    format($$SELECT  proallargtypes FROM pg_proc WHERE proname = %1$L$$, fn),
    $$SELECT '{25,20,701}'::OID[] $$
  );

END;
$BODY$
LANGUAGE plpgsql VOLATILE;

CREATE OR REPLACE FUNCTION spanntree_bfs_types_check(fn TEXT)
RETURNS SETOF TEXT AS
$BODY$
BEGIN
  RETURN QUERY SELECT has_function(fn);

  RETURN QUERY SELECT has_function(fn, ARRAY['text','bigint','bigint']);
  RETURN QUERY SELECT has_function(fn, ARRAY['text','anyarray','bigint']);
  RETURN QUERY SELECT function_returns(fn, ARRAY['text','bigint','bigint'],  'setof record');
  RETURN QUERY SELECT function_returns(fn, ARRAY['text','anyarray','bigint'],  'setof record');

  RETURN QUERY SELECT set_eq(
    format($$SELECT  proargnames FROM pg_proc WHERE proname = %1$L$$,fn),
    $$VALUES
    ('{"","","max_depth","seq","depth","start_vid","node","edge","cost","agg_cost"}'::TEXT[])
    $$
  );

  RETURN QUERY SELECT set_eq(
    format($$SELECT  proallargtypes FROM pg_proc WHERE proname = %1$L$$, fn),
    $$VALUES
    ('{25,20,20,20,20,20,20,20,701,701}'::OID[]),
    ('{25,2277,20,20,20,20,20,20,701,701}'::OID[])
    $$
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

  RETURN QUERY SELECT set_eq(
    format($$SELECT  proargnames FROM pg_proc WHERE proname = %1$L$$,fn),
    $$VALUES
    ('{"","","","seq","depth","start_vid","node","edge","cost","agg_cost"}'::TEXT[])
    $$
  );

  RETURN QUERY SELECT set_eq(
    format($$SELECT  proallargtypes FROM pg_proc WHERE proname = %1$L$$, fn),
    $$VALUES
    ('{25,20,701,20,20,20,20,20,701,701}'::OID[]),
    ('{25,2277,701,20,20,20,20,20,701,701}'::OID[]),
    ('{25,20,1700,20,20,20,20,20,701,701}'::OID[]),
    ('{25,2277,1700,20,20,20,20,20,701,701}'::OID[]),
    ('{25,20,701,20,20,20,20,20,701,701}'::OID[]),
    ('{25,2277,701,20,20,20,20,20,701,701}'::OID[])
    $$
  );
END;
$BODY$
LANGUAGE plpgsql VOLATILE;

