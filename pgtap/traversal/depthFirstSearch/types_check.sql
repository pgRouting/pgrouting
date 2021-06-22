\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT CASE WHEN min_version('3.2.0') THEN plan (7) ELSE plan(1) END;

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
SELECT has_function('pgr_depthfirstsearch');
RETURN QUERY
SELECT has_function('pgr_depthfirstsearch', ARRAY['text','bigint','boolean','bigint']);
RETURN QUERY
SELECT has_function('pgr_depthfirstsearch', ARRAY['text','anyarray','boolean','bigint']);
RETURN QUERY
SELECT function_returns('pgr_depthfirstsearch', ARRAY['text','bigint','boolean','bigint'],  'setof record');
RETURN QUERY
SELECT function_returns('pgr_depthfirstsearch', ARRAY['text','anyarray','boolean','bigint'],  'setof record');

-- pgr_depthfirstsearch
-- parameter names
RETURN QUERY
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_depthfirstsearch'$$,
    $$SELECT  '{"","","directed","max_depth","seq","depth","start_vid","node","edge","cost","agg_cost"}'::TEXT[] $$
);

-- parameter types
RETURN QUERY
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_depthfirstsearch'$$,
    $$VALUES
        ('{25,20,16,20,20,20,20,20,20,701,701}'::OID[]),
        ('{25,2277,16,20,20,20,20,20,20,701,701}'::OID[])
    $$
);
END;
$BODY$
LANGUAGE plpgsql;

SELECT types_check();


SELECT * FROM finish();
ROLLBACK;
