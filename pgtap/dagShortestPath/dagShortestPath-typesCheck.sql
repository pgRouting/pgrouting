
SELECT plan(4);

SELECT has_function('pgr_dagshortestpath');

SELECT has_function('pgr_dagshortestpath', ARRAY[ 'text', 'bigint', 'bigint' ]);

SELECT function_returns('pgr_dagshortestpath', ARRAY[ 'text', 'bigint', 'bigint' ], 'setof record');

-- testing column names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_dagshortestpath'$$,
    $$SELECT  '{"","","","seq","path_seq","node","edge","cost","agg_cost"}'::TEXT[] $$
);

