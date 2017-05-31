
SELECT plan(4);

SELECT has_function('pgr_dijkstratrsp');

SELECT has_function('pgr_dijkstratrsp', ARRAY[ 'text', 'text', 'bigint', 'bigint', 'boolean', 'boolean' ]);

--SELECT todo_start();
SELECT function_returns('pgr_dijkstratrsp',
    --ARRAY['text','bigint','bigint','boolean'],
    ARRAY[ 'text', 'text', 'bigint', 'bigint', 'boolean', 'boolean' ],
     'setof record');
--SELECT todo_end();

-- testing column names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_dijkstratrsp'$$,
    $$SELECT  '{"","","","","directed","only_cost","seq","path_seq","node","edge","cost","agg_cost"}'::TEXT[] $$
);
