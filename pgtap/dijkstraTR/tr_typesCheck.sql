
SELECT plan(4);

SELECT has_function('pgr_turnrestrictedpath');

SELECT has_function('pgr_turnrestrictedpath',
    ARRAY[ 'text', 'text', 'bigint', 'bigint', 'integer', 'boolean', 'boolean', 'boolean', 'boolean']
);

SELECT function_returns('pgr_turnrestrictedpath',
    ARRAY[ 'text', 'text', 'bigint', 'bigint', 'integer', 'boolean', 'boolean', 'boolean', 'boolean'],
    'setof record');

-- testing column names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_turnrestrictedpath'$$,
    $$SELECT  '{"","","","","","directed","heap_paths","stop_on_first","strict","seq","path_id","path_seq","node","edge","cost","agg_cost"}'::TEXT[] $$
);
