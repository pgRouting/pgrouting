
CREATE OR REPLACE FUNCTION astar_types_check(fn TEXT) RETURNS SETOF TEXT AS
$BODY$
DECLARE
the_type_name TEXT = 'double precision';
the_type_numb TEXT = '701';

BEGIN
  IF fn = 'pgr_bdastar' THEN
    the_type_name = 'numeric';
    the_type_numb = '1700';
  END IF;

  RETURN QUERY SELECT has_function(fn);

  RETURN QUERY SELECT has_function(fn, ARRAY['text','bigint','bigint','boolean','integer',the_type_name,the_type_name]);
  RETURN QUERY SELECT has_function(fn, ARRAY['text','bigint','anyarray','boolean','integer',the_type_name,the_type_name]);
  RETURN QUERY SELECT has_function(fn, ARRAY['text','anyarray','bigint','boolean','integer',the_type_name,the_type_name]);
  RETURN QUERY SELECT has_function(fn, ARRAY['text','anyarray','anyarray','boolean','integer',the_type_name,the_type_name]);

  RETURN QUERY SELECT function_returns(fn, ARRAY['text','bigint','bigint','boolean','integer',the_type_name,the_type_name],'setof record');
  RETURN QUERY SELECT function_returns(fn, ARRAY['text','bigint','anyarray','boolean','integer',the_type_name,the_type_name],'setof record');
  RETURN QUERY SELECT function_returns(fn, ARRAY['text','anyarray','bigint','boolean','integer',the_type_name,the_type_name],'setof record');
  RETURN QUERY SELECT function_returns(fn, ARRAY['text','anyarray','anyarray','boolean','integer',the_type_name,the_type_name],'setof record');

  IF min_version('3.2.0') THEN
    RETURN QUERY SELECT has_function(fn, ARRAY['text','text','boolean','integer',the_type_name,the_type_name]);
    RETURN QUERY SELECT function_returns(fn, ARRAY['text','text','boolean','integer',the_type_name,the_type_name],'setof record');
  ELSE
    RETURN QUERY SELECT skip(2, 'Combinations signature added on 3.2.0');
  END IF;

  IF min_version('3.2.0') THEN
    RETURN QUERY SELECT set_eq(
      format($$SELECT  proargnames FROM pg_proc WHERE proname = %1$L$$,fn),
      $$VALUES
      ('{"","","","directed","heuristic","factor","epsilon","seq","path_seq","node","edge","cost","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","seq","path_seq","end_vid","node","edge","cost","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","seq","path_seq","start_vid","node","edge","cost","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[]),
      ('{"","","directed","heuristic","factor","epsilon","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[])
      $$);

    RETURN QUERY SELECT set_eq(
      format($$SELECT  proallargtypes FROM pg_proc WHERE proname = %1$L$$,fn),
      format($$VALUES
      ('{25,20,20,16,23,%1$s,%1$s,23,23,20,20,701,701}'::OID[]),
      ('{25,20,2277,16,23,%1$s,%1$s,23,23,20,20,20,701,701}'::OID[]),
      ('{25,2277,20,16,23,%1$s,%1$s,23,23,20,20,20,701,701}'::OID[]),
      ('{25,2277,2277,16,23,%1$s,%1$s,23,23,20,20,20,20,701,701}'::OID[]),
      ('{25,25,16,23,%1$s,%1$s,23,23,20,20,20,20,701,701}'::OID[])
      $$,the_type_numb));
  ELSE
    RETURN QUERY SELECT set_eq(
      format($$SELECT  proargnames FROM pg_proc WHERE proname = %1$L$$,fn),
      $$VALUES
      ('{"","","","directed","heuristic","factor","epsilon","seq","path_seq","node","edge","cost","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","seq","path_seq","end_vid","node","edge","cost","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","seq","path_seq","start_vid","node","edge","cost","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[])
      $$);

    RETURN QUERY SELECT set_eq(
      format($$SELECT  proallargtypes FROM pg_proc WHERE proname = %1$L$$,fn),
      format($$VALUES
      ('{25,20,20,16,23,%1$s,%1$s,23,23,20,20,701,701}'::OID[]),
      ('{25,20,2277,16,23,%1$s,%1$s,23,23,20,20,20,701,701}'::OID[]),
      ('{25,2277,20,16,23,%1$s,%1$s,23,23,20,20,20,701,701}'::OID[]),
      ('{25,2277,2277,16,23,%1$s,%1$s,23,23,20,20,20,20,701,701}'::OID[])
      $$,the_type_numb));
  END IF;

END;
$BODY$
LANGUAGE plpgsql VOLATILE;

CREATE OR REPLACE FUNCTION astarcost_types_check(fn TEXT) RETURNS SETOF TEXT AS
$BODY$
DECLARE
the_type_name TEXT = 'double precision';
the_type_numb TEXT = '701';

BEGIN
  IF fn = 'pgr_bdastarcost' THEN
    the_type_name = 'numeric';
    the_type_numb = '1700';
  END IF;

  RETURN QUERY SELECT has_function(fn);

  RETURN QUERY SELECT has_function(fn, ARRAY['text','bigint','bigint','boolean','integer',the_type_name,the_type_name]);
  RETURN QUERY SELECT has_function(fn, ARRAY['text','bigint','anyarray','boolean','integer',the_type_name,the_type_name]);
  RETURN QUERY SELECT has_function(fn, ARRAY['text','anyarray','bigint','boolean','integer',the_type_name,the_type_name]);
  RETURN QUERY SELECT has_function(fn, ARRAY['text','anyarray','anyarray','boolean','integer',the_type_name,the_type_name]);

  RETURN QUERY SELECT function_returns(fn, ARRAY['text','bigint','bigint','boolean','integer',the_type_name,the_type_name],'setof record');
  RETURN QUERY SELECT function_returns(fn, ARRAY['text','bigint','anyarray','boolean','integer',the_type_name,the_type_name],'setof record');
  RETURN QUERY SELECT function_returns(fn, ARRAY['text','anyarray','bigint','boolean','integer',the_type_name,the_type_name],'setof record');
  RETURN QUERY SELECT function_returns(fn, ARRAY['text','anyarray','anyarray','boolean','integer',the_type_name,the_type_name],'setof record');

  IF min_version('3.2.0') THEN
    RETURN QUERY SELECT has_function(fn, ARRAY['text','text','boolean','integer',the_type_name,the_type_name]);
    RETURN QUERY SELECT function_returns(fn, ARRAY['text','text','boolean','integer',the_type_name,the_type_name],'setof record');
  ELSE
    RETURN QUERY SELECT skip(2, 'Combinations signature added on 3.2.0');
  END IF;

  IF min_version('3.2.0') THEN
    RETURN QUERY SELECT set_eq(
      format($$SELECT  proargnames FROM pg_proc WHERE proname = %1$L$$,fn),
      $$VALUES
      ('{"","","directed","heuristic","factor","epsilon","start_vid","end_vid","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","start_vid","end_vid","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","start_vid","end_vid","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","start_vid","end_vid","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","start_vid","end_vid","agg_cost"}'::TEXT[])
      $$);

    RETURN QUERY SELECT set_eq(
      format($$SELECT  proallargtypes FROM pg_proc WHERE proname = %1$L$$,fn),
      format($$VALUES
      ('{25,20,20,16,23,%1$s,%1$s,20,20,701}'::OID[]),
      ('{25,20,2277,16,23,%1$s,%1$s,20,20,701}'::OID[]),
      ('{25,2277,20,16,23,%1$s,%1$s,20,20,701}'::OID[]),
      ('{25,2277,2277,16,23,%1$s,%1$s,20,20,701}'::OID[]),
      ('{25,25,16,23,%1$s,%1$s,20,20,701}'::OID[])
      $$,the_type_numb));
  ELSE
    RETURN QUERY SELECT set_eq(
      format($$SELECT  proargnames FROM pg_proc WHERE proname = %1$L$$,fn),
      $$VALUES
      ('{"","","","directed","heuristic","factor","epsilon","start_vid","end_vid","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","start_vid","end_vid","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","start_vid","end_vid","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","start_vid","end_vid","agg_cost"}'::TEXT[])
      $$);

    RETURN QUERY SELECT set_eq(
      format($$SELECT  proallargtypes FROM pg_proc WHERE proname = %1$L$$,fn),
      format($$VALUES
      ('{25,20,20,16,23,%1$s,%1$s,20,20,701}'::OID[]),
      ('{25,20,2277,16,23,%1$s,%1$s,20,20,701}'::OID[]),
      ('{25,2277,20,16,23,%1$s,%1$s,20,20,701}'::OID[]),
      ('{25,2277,2277,16,23,%1$s,%1$s,20,20,701}'::OID[])
      $$,the_type_numb));
  END IF;

END;
$BODY$
LANGUAGE plpgsql VOLATILE;

CREATE OR REPLACE FUNCTION astarcostmatrix_types_check(fn TEXT) RETURNS SETOF TEXT AS
$BODY$
DECLARE
the_type_name TEXT = 'double precision';
the_type_numb TEXT = '701';

BEGIN
  IF fn = 'pgr_bdastarcostmatrix' THEN
    the_type_name = 'numeric';
    the_type_numb = '1700';
  END IF;

  RETURN QUERY SELECT has_function(fn);

  RETURN QUERY SELECT has_function(fn, ARRAY['text','anyarray','boolean','integer',the_type_name,the_type_name]);

  RETURN QUERY SELECT function_returns(fn, ARRAY['text','anyarray','boolean','integer',the_type_name,the_type_name],'setof record');

  RETURN QUERY SELECT set_eq(
    format($$SELECT  proargnames FROM pg_proc WHERE proname = %1$L$$,fn),
    $$VALUES
    ('{"","","directed","heuristic","factor","epsilon","start_vid","end_vid","agg_cost"}'::TEXT[])
    $$);

    RETURN QUERY SELECT set_eq(
      format($$SELECT  proallargtypes FROM pg_proc WHERE proname = %1$L$$,fn),
      format($$VALUES
      ('{25,2277,16,23,%1$s,%1$s,20,20,701}'::OID[])
      $$,the_type_numb));

END;
$BODY$
LANGUAGE plpgsql VOLATILE;

