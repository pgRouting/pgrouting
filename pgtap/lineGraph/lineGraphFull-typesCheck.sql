
SELECT plan(3);

SELECT has_function('pgr_linegraphfull');

SELECT function_returns('pgr_linegraphfull','setof record');

-- testing column names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_linegraphfull'$$,
    $$SELECT  '{"","seq","source","target","cost","edge"}'::TEXT[] $$
);
