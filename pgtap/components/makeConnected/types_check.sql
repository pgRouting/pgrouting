\i setup.sql

SELECT plan(4);

SELECT has_function('pgr_makeconnected');

SELECT function_returns('pgr_makeconnected', ARRAY['text'], 'setof record');


-- pgr_makeconnected
-- parameter names
SELECT bag_has(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_makeconnected'$$,
    $$SELECT  '{"",seq,start_vid,end_vid}'::TEXT[] $$
);

-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_makeconnected'$$,
    $$VALUES
        ('{25,20,20,20}'::OID[])
    $$
);

SELECT * FROM finish();
ROLLBACK;
