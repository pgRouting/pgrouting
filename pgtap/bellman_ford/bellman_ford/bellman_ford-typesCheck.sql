\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT CASE WHEN min_version('3.2.0') THEN plan (18) ELSE plan(15) END;

CREATE OR REPLACE FUNCTION types_check()
RETURNS SETOF TEXT AS
$BODY$
BEGIN

RETURN QUERY
SELECT has_function('pgr_bellmanford');

RETURN QUERY
SELECT has_function('pgr_bellmanford', ARRAY['text','bigint','bigint','boolean']);

RETURN QUERY
SELECT has_function('pgr_bellmanford', ARRAY['text','bigint','anyarray','boolean']);

RETURN QUERY
SELECT has_function('pgr_bellmanford', ARRAY['text','anyarray','bigint','boolean']);

RETURN QUERY
SELECT has_function('pgr_bellmanford', ARRAY['text','anyarray','anyarray','boolean']);

RETURN QUERY
SELECT function_returns('pgr_bellmanford', ARRAY['text','bigint','bigint','boolean'], 'setof record');

RETURN QUERY
SELECT function_returns('pgr_bellmanford', ARRAY['text','bigint','anyarray','boolean'], 'setof record');

RETURN QUERY
SELECT function_returns('pgr_bellmanford', ARRAY['text','anyarray','bigint','boolean'], 'setof record');

RETURN QUERY
SELECT function_returns('pgr_bellmanford', ARRAY['text','anyarray','anyarray','boolean'], 'setof record');

-- testing column names
RETURN QUERY
SELECT set_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_bellmanford'$$,
    $$SELECT  '{"","","","directed","seq","path_seq","node","edge","cost","agg_cost"}'::TEXT[] $$
);

RETURN QUERY
SELECT set_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_bellmanford'$$,
    $$SELECT  '{"","","","directed","seq","path_seq","start_vid","node","edge","cost","agg_cost"}'::TEXT[] $$
);

RETURN QUERY
SELECT set_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_bellmanford'$$,
    $$SELECT  '{"","","","directed","seq","path_seq","end_vid","node","edge","cost","agg_cost"}'::TEXT[] $$
);

RETURN QUERY
SELECT set_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_bellmanford'$$,
    $$SELECT  '{"","","","directed","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[] $$
);

-- parameter types
RETURN QUERY
SELECT set_has(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_bellmanford'$$,
    $$VALUES
        ('{25,20,20,16,23,23,20,20,701,701}'::OID[]),
        ('{25,20,2277,16,23,23,20,20,20,701,701}'::OID[]),
        ('{25,2277,20,16,23,23,20,20,20,701,701}'::OID[]),
        ('{25,2277,2277,16,23,23,20,20,20,20,701,701}'::OID[])
    $$
);

RETURN QUERY
SELECT CASE
WHEN NOT min_version('3.2.0') THEN
  skip(1, 'Combinations signature added on version 3.2.0')
ELSE
  collect_tap(
    has_function('pgr_bellmanford', ARRAY['text','text','boolean']),
    function_returns('pgr_bellmanford', ARRAY['text','text','boolean'], 'setof record'),
    set_has(
      $$SELECT  proargnames from pg_proc where proname = 'pgr_bellmanford'$$,
      $$SELECT  '{"","",directed,seq,path_seq,start_vid,end_vid,node,edge,cost,agg_cost}'::TEXT[] $$
    ),
    set_has(
      $$SELECT  proallargtypes from pg_proc where proname = 'pgr_bellmanford'$$,
      $$VALUES ('{25,25,16,23,23,20,20,20,20,701,701}'::OID[]) $$
    )
  )
END;

END;
$BODY$
LANGUAGE plpgsql;

SELECT types_check();


SELECT finish();
ROLLBACK;
