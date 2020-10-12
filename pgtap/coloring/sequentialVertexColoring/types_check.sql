\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(5);

SELECT has_function('pgr_sequentialvertexcoloring');

SELECT has_function('pgr_sequentialvertexcoloring', ARRAY['text']);
SELECT function_returns('pgr_sequentialvertexcoloring', ARRAY['text'],  'setof record');

-- pgr_sequentialvertexcoloring
-- parameter names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_sequentialvertexcoloring'$$,
    $$SELECT  '{"","vertex_id","color_id"}'::TEXT[] $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_sequentialvertexcoloring'$$,
    $$VALUES
        ('{25,20,20}'::OID[])
    $$
);

SELECT * FROM finish();
ROLLBACK;
