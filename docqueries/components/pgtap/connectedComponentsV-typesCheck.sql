-- CopyRight(c) pgRouting developers
-- Creative Commons Attribution-Share Alike 3.0 License : https://creativecommons.org/licenses/by-sa/3.0/

SELECT plan(4);

SELECT has_function('pgr_connectedcomponentsv');

SELECT has_function('pgr_connectedcomponentsv', ARRAY[ 'text', 'bigint', 'bigint', 'boolean', 'boolean' ]);

SELECT function_returns('pgr_connectedcomponentsv', ARRAY[ 'text', 'bigint', 'bigint', 'boolean', 'boolean' ], 'setof record');

-- testing column names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_connectedcomponentsv'$$,
    $$SELECT  '{"","","","directed","only_cost","seq","path_seq","node","edge","cost","agg_cost"}'::TEXT[] $$
);

