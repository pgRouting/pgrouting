\i setup.sql

SELECT plan(2);

SELECT has_function('pgr_isplanar');

SELECT function_returns('pgr_isplanar', ARRAY['text'], 'boolean');


-- pgr_isplanar
-- parameter names

-- SELECT bag_has(
--     $$SELECT  proargnames from pg_proc where proname = 'pgr_isplanar'$$,
--     $$SELECT  ''::TEXT[] $$
-- );
--
-- -- parameter types
-- SELECT set_eq(
--     $$SELECT  proallargtypes from pg_proc where proname = 'pgr_isplanar'$$,
--     $$VALUES
--         (''::OID[])
--     $$
-- );

SELECT * FROM finish();
ROLLBACK;
