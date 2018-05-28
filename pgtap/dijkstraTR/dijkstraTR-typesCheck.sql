
SELECT plan(4);

SELECT has_function('pgr_dijkstratr');

SELECT has_function('pgr_dijkstratr', ARRAY[ 'text', 'text', 'bigint', 'bigint', 'boolean', 'boolean', 'boolean' ]);

SELECT function_returns('pgr_dijkstratr',
    ARRAY[ 'text', 'text', 'bigint', 'bigint', 'boolean', 'boolean', 'boolean' ],
    'setof record');

-- testing column names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_dijkstratr'$$,
    $$SELECT  '{"","","","","directed","only_cost","strict","seq","path_seq","node","edge","cost","agg_cost"}'::TEXT[] $$
);
