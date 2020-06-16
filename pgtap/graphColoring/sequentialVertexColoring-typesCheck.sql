
SELECT plan(4);

SELECT has_function('pgr_sequentialvertexcoloring');

SELECT has_function('pgr_sequentialvertexcoloring', ARRAY[ 'text', 'bigint', 'bigint', 'boolean', 'boolean' ]);

SELECT function_returns('pgr_sequentialvertexcoloring', ARRAY[ 'text', 'bigint', 'bigint', 'boolean', 'boolean' ], 'setof record');

-- testing column names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_sequentialvertexcoloring'$$,
    $$SELECT  '{"","","","directed","only_cost","seq","path_seq","node","edge","cost","agg_cost"}'::TEXT[] $$
);

