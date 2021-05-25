
UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(14);

SELECT has_function('pgr_dagshortestpath');

SELECT has_function('pgr_dagshortestpath', ARRAY[ 'text', 'bigint', 'bigint' ]);
SELECT has_function('pgr_dagshortestpath', ARRAY[ 'text', 'anyarray', 'bigint' ]);
SELECT has_function('pgr_dagshortestpath', ARRAY[ 'text', 'bigint', 'anyarray' ]);
SELECT has_function('pgr_dagshortestpath', ARRAY[ 'text', 'anyarray', 'anyarray' ]);
SELECT has_function('pgr_dagshortestpath', ARRAY[ 'text', 'text' ]);

SELECT function_returns('pgr_dagshortestpath', ARRAY[ 'text', 'bigint', 'bigint' ], 'setof record');
SELECT function_returns('pgr_dagshortestpath', ARRAY[ 'text', 'anyarray', 'bigint' ], 'setof record');
SELECT function_returns('pgr_dagshortestpath', ARRAY[ 'text', 'bigint', 'anyarray' ], 'setof record');
SELECT function_returns('pgr_dagshortestpath', ARRAY[ 'text', 'anyarray', 'anyarray' ], 'setof record');
SELECT function_returns('pgr_dagshortestpath', ARRAY[ 'text', 'text' ], 'setof record');

-- testing column names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_dagshortestpath'$$,
    $$SELECT  '{"","","","seq","path_seq","node","edge","cost","agg_cost"}'::TEXT[] $$
);

SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_dagshortestpath'$$,
    $$SELECT  '{"","",seq,path_seq,node,edge,cost,agg_cost}'::TEXT[] $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_dagshortestpath'$$,
    $$VALUES
        ('{25,20,20,23,23,20,20,701,701}'::OID[]),
        ('{25,20,2277,23,23,20,20,701,701}'::OID[]),
        ('{25,2277,20,23,23,20,20,701,701}'::OID[]),
        ('{25,2277,2277,23,23,20,20,701,701}'::OID[]),
        ('{25,25,23,23,20,20,701,701}'::OID[])
    $$
);
