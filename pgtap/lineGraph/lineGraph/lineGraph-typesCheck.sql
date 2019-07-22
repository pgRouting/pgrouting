
SELECT plan(4);

SELECT has_function('pgr_linegraph');

SELECT has_function('pgr_linegraph', ARRAY[ 'text', 'boolean' ]);

SELECT function_returns('pgr_linegraph', ARRAY[ 'text', 'boolean' ], 'setof record');

-- testing column names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_linegraph'$$,
    $$SELECT  '{"","directed","seq","source","target","cost","reverse_cost"}'::TEXT[] $$
);
