
SELECT plan(10);

SELECT has_function('pgr_dagshortestpath');

SELECT has_function('pgr_dagshortestpath', ARRAY[ 'text', 'bigint', 'bigint' ]);
SELECT has_function('pgr_dagshortestpath', ARRAY[ 'text', 'anyarray', 'bigint' ]);
SELECT has_function('pgr_dagshortestpath', ARRAY[ 'text', 'bigint', 'anyarray' ]);
SELECT has_function('pgr_dagshortestpath', ARRAY[ 'text', 'anyarray', 'anyarray' ]);

SELECT function_returns('pgr_dagshortestpath', ARRAY[ 'text', 'bigint', 'bigint' ], 'setof record');
SELECT function_returns('pgr_dagshortestpath', ARRAY[ 'text', 'anyarray', 'bigint' ], 'setof record');
SELECT function_returns('pgr_dagshortestpath', ARRAY[ 'text', 'bigint', 'anyarray' ], 'setof record');
SELECT function_returns('pgr_dagshortestpath', ARRAY[ 'text', 'anyarray', 'anyarray' ], 'setof record');

-- testing column names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_dagshortestpath'$$,
    $$SELECT  '{"","","","seq","path_seq","node","edge","cost","agg_cost"}'::TEXT[] $$
);

