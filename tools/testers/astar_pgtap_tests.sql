
CREATE OR REPLACE FUNCTION astar_types_check(fn TEXT) RETURNS SETOF TEXT AS
$BODY$
DECLARE
the_type_name TEXT = 'double precision';
the_type_numb TEXT = 'float8';

BEGIN
  IF fn = 'pgr_bdastar' THEN
    the_type_name = 'numeric';
    the_type_numb = 'numeric';
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

  IF min_version('3.6.0') AND fn IN('pgr_astar', 'pgr_bdastar') THEN
    RETURN QUERY SELECT function_args_eq(fn,
      $$VALUES
      ('{"","","","directed","heuristic","factor","epsilon","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[]),
      ('{"","","directed","heuristic","factor","epsilon","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[])
      $$);

    RETURN QUERY SELECT function_types_eq(fn,
      format($$VALUES
      ('{text,int8,int8,bool,int4,%1$s,%1$s,int4,int4,int8,int8,int8,int8,float8,float8}'::TEXT[]),
      ('{text,int8,anyarray,bool,int4,%1$s,%1$s,int4,int4,int8,int8,int8,int8,float8,float8}'::TEXT[]),
      ('{text,anyarray,int8,bool,int4,%1$s,%1$s,int4,int4,int8,int8,int8,int8,float8,float8}'::TEXT[]),
      ('{text,anyarray,anyarray,bool,int4,%1$s,%1$s,int4,int4,int8,int8,int8,int8,float8,float8}'::TEXT[]),
      ('{text,text,bool,int4,%1$s,%1$s,int4,int4,int8,int8,int8,int8,float8,float8}'::TEXT[])
      $$,the_type_numb));

  ELSIF min_version('3.2.0') THEN
    RETURN QUERY SELECT function_args_eq(fn,
      $$VALUES
      ('{"","","","directed","heuristic","factor","epsilon","seq","path_seq","node","edge","cost","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","seq","path_seq","end_vid","node","edge","cost","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","seq","path_seq","start_vid","node","edge","cost","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[]),
      ('{"","","directed","heuristic","factor","epsilon","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[])
      $$);

    RETURN QUERY SELECT function_types_eq(fn,
      format($$VALUES
      ('{text,int8,int8,bool,int4,%1$s,%1$s,int4,int4,int8,int8,float8,float8}'::TEXT[]),
      ('{text,int8,anyarray,bool,int4,%1$s,%1$s,int4,int4,int8,int8,int8,float8,float8}'::TEXT[]),
      ('{text,anyarray,int8,bool,int4,%1$s,%1$s,int4,int4,int8,int8,int8,float8,float8}'::TEXT[]),
      ('{text,anyarray,anyarray,bool,int4,%1$s,%1$s,int4,int4,int8,int8,int8,int8,float8,float8}'::TEXT[]),
      ('{text,text,bool,int4,%1$s,%1$s,int4,int4,int8,int8,int8,int8,float8,float8}'::TEXT[])
      $$,the_type_numb));
  ELSE
    RETURN QUERY SELECT function_args_eq(fn,
      $$VALUES
      ('{"","","","directed","heuristic","factor","epsilon","seq","path_seq","node","edge","cost","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","seq","path_seq","end_vid","node","edge","cost","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","seq","path_seq","start_vid","node","edge","cost","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[])
      $$);

    RETURN QUERY SELECT function_types_eq(fn,
      format($$VALUES
      ('{text,int8,int8,bool,int4,%1$s,%1$s,int4,int4,int8,int8,float8,float8}'::TEXT[]),
      ('{text,int8,anyarray,bool,int4,%1$s,%1$s,int4,int4,int8,int8,int8,float8,float8}'::TEXT[]),
      ('{text,anyarray,int8,bool,int4,%1$s,%1$s,int4,int4,int8,int8,int8,float8,float8}'::TEXT[]),
      ('{text,anyarray,anyarray,bool,int4,%1$s,%1$s,int4,int4,int8,int8,int8,int8,float8,float8}'::TEXT[])
      $$,the_type_numb));
  END IF;

END;
$BODY$
LANGUAGE plpgsql VOLATILE;

CREATE OR REPLACE FUNCTION astarcost_types_check(fn TEXT) RETURNS SETOF TEXT AS
$BODY$
DECLARE
the_type_name TEXT = 'double precision';
the_type_numb TEXT = 'float8';

BEGIN
  IF fn = 'pgr_bdastarcost' THEN
    the_type_name = 'numeric';
    the_type_numb = 'numeric';
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
    RETURN QUERY SELECT function_args_eq(fn,
      $$VALUES
      ('{"","","directed","heuristic","factor","epsilon","start_vid","end_vid","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","start_vid","end_vid","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","start_vid","end_vid","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","start_vid","end_vid","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","start_vid","end_vid","agg_cost"}'::TEXT[])
      $$);

    RETURN QUERY SELECT function_types_eq(fn,
      format($$VALUES
      ('{text,int8,int8,bool,int4,%1$s,%1$s,int8,int8,float8}'::TEXT[]),
      ('{text,int8,anyarray,bool,int4,%1$s,%1$s,int8,int8,float8}'::TEXT[]),
      ('{text,anyarray,int8,bool,int4,%1$s,%1$s,int8,int8,float8}'::TEXT[]),
      ('{text,anyarray,anyarray,bool,int4,%1$s,%1$s,int8,int8,float8}'::TEXT[]),
      ('{text,text,bool,int4,%1$s,%1$s,int8,int8,float8}'::TEXT[])
      $$,the_type_numb));
  ELSE
    RETURN QUERY SELECT function_args_eq(fn,
      $$VALUES
      ('{"","","","directed","heuristic","factor","epsilon","start_vid","end_vid","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","start_vid","end_vid","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","start_vid","end_vid","agg_cost"}'::TEXT[]),
      ('{"","","","directed","heuristic","factor","epsilon","start_vid","end_vid","agg_cost"}'::TEXT[])
      $$);

    RETURN QUERY SELECT function_types_eq(fn,
      format($$VALUES
      ('{text,int8,int8,bool,int4,%1$s,%1$s,int8,int8,float8}'::TEXT[]),
      ('{text,int8,anyarray,bool,int4,%1$s,%1$s,int8,int8,float8}'::TEXT[]),
      ('{text,anyarray,int8,bool,int4,%1$s,%1$s,int8,int8,float8}'::TEXT[]),
      ('{text,anyarray,anyarray,bool,int4,%1$s,%1$s,int8,int8,float8}'::TEXT[])
      $$,the_type_numb));
  END IF;

END;
$BODY$
LANGUAGE plpgsql VOLATILE;

CREATE OR REPLACE FUNCTION astarcostmatrix_types_check(fn TEXT) RETURNS SETOF TEXT AS
$BODY$
DECLARE
the_type_name TEXT = 'double precision';
the_type_numb TEXT = 'float8';

BEGIN
  IF fn = 'pgr_bdastarcostmatrix' THEN
    the_type_name = 'numeric';
    the_type_numb = 'numeric';
  END IF;

  RETURN QUERY SELECT has_function(fn);

  RETURN QUERY SELECT has_function(fn, ARRAY['text','anyarray','boolean','integer',the_type_name,the_type_name]);

  RETURN QUERY SELECT function_returns(fn, ARRAY['text','anyarray','boolean','integer',the_type_name,the_type_name],'setof record');

  RETURN QUERY SELECT function_args_eq(fn,
    $$VALUES
    ('{"","","directed","heuristic","factor","epsilon","start_vid","end_vid","agg_cost"}'::TEXT[])
    $$);

  RETURN QUERY SELECT function_types_eq(fn,
    format($$VALUES
      ('{text,anyarray,bool,int4,%1$s,%1$s,int8,int8,float8}'::TEXT[])
      $$,the_type_numb));

END;
$BODY$
LANGUAGE plpgsql VOLATILE;

