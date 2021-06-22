\i setup.sql

SELECT CASE WHEN NOT min_version('3.2.0') THEN plan(1) ELSE plan(11) END;

CREATE OR REPLACE FUNCTION types_check()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

IF NOT min_version('3.2.0') THEN
  RETURN QUERY
  SELECT skip(1, 'Function is new on 3.2.0');
  RETURN;
END IF;


RETURN QUERY
SELECT has_function('pgr_dijkstranear');

RETURN QUERY
SELECT has_function('pgr_dijkstranear', ARRAY['text','bigint','anyarray','boolean','bigint']);
RETURN QUERY
SELECT has_function('pgr_dijkstranear', ARRAY['text','anyarray','bigint','boolean','bigint']);
RETURN QUERY
SELECT has_function('pgr_dijkstranear', ARRAY['text','anyarray','anyarray','boolean','bigint','boolean']);
RETURN QUERY
SELECT has_function('pgr_dijkstranear', ARRAY['text','text','boolean','bigint','boolean']);

RETURN QUERY
SELECT function_returns('pgr_dijkstranear', ARRAY['text','bigint','anyarray','boolean','bigint'],  'setof record');
RETURN QUERY
SELECT function_returns('pgr_dijkstranear', ARRAY['text','anyarray','bigint','boolean','bigint'],  'setof record');
RETURN QUERY
SELECT function_returns('pgr_dijkstranear', ARRAY['text','anyarray','anyarray','boolean','bigint','boolean'],  'setof record');
RETURN QUERY
SELECT function_returns('pgr_dijkstranear', ARRAY['text','text','boolean','bigint','boolean'],  'setof record');

-- parameter's names
RETURN QUERY
SELECT set_eq(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_dijkstranear'$$,
    $$VALUES
        ('{"","","","directed","cap","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[]),
        ('{"","","","directed","cap","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[]),
        ('{"","","","directed","cap","global","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[]),
        ('{"","","directed","cap","global","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[]);
    $$
);


-- parameter types
RETURN QUERY
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_dijkstranear'$$,
    $$VALUES
    ('{25,2277,20,16,20,23,23,20,20,20,20,701,701}'::OID[]),
    ('{25,2277,2277,16,20,16,23,23,20,20,20,20,701,701}'::OID[]),
    ('{25,25,16,20,16,23,23,20,20,20,20,701,701}'::OID[]),
    ('{25,20,2277,16,20,23,23,20,20,20,20,701,701}'::OID[]);
$$
);
END;
$BODY$
LANGUAGE plpgsql;

SELECT * FROM types_check();

SELECT * FROM finish();
ROLLBACK;
