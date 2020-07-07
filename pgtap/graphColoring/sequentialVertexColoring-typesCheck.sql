\i setup.sql

SELECT plan(5);

SELECT todo_start('Complete the types check');

SELECT has_function('pgr_sequentialvertexcoloring');

SELECT has_function('pgr_sequentialvertexcoloring', ARRAY['text']);
SELECT function_returns('pgr_sequentialvertexcoloring', ARRAY['text'],  'setof record');

-- pgr_sequentialvertexcoloring
-- parameter names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_sequentialvertexcoloring'$$,
    $$SELECT  '{"","node","color"}'::TEXT[] $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_sequentialvertexcoloring'$$,
    $$VALUES
        ('{25,20,20}'::OID[])
    $$
);

SELECT todo_end();

SELECT * FROM finish();
ROLLBACK;
