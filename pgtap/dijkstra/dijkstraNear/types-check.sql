\i setup.sql

SELECT plan(11);

SELECT has_function('pgr_dijkstranear');

SELECT has_function('pgr_dijkstranear', ARRAY['text','bigint','anyarray','boolean','bigint']);
SELECT has_function('pgr_dijkstranear', ARRAY['text','anyarray','bigint','boolean','bigint']);
SELECT has_function('pgr_dijkstranear', ARRAY['text','anyarray','anyarray','boolean','bigint','boolean']);
SELECT has_function('pgr_dijkstranear', ARRAY['text','text','boolean','bigint','boolean']);

SELECT function_returns('pgr_dijkstranear', ARRAY['text','bigint','anyarray','boolean','bigint'],  'setof record');
SELECT function_returns('pgr_dijkstranear', ARRAY['text','anyarray','bigint','boolean','bigint'],  'setof record');
SELECT function_returns('pgr_dijkstranear', ARRAY['text','anyarray','anyarray','boolean','bigint','boolean'],  'setof record');
SELECT function_returns('pgr_dijkstranear', ARRAY['text','text','boolean','bigint','boolean'],  'setof record');

-- parameter's names
SELECT set_eq(
    $$SELECT  proargnames from pg_proc where proname = 'pgr_dijkstranear'$$,
    $$VALUES
        ('{"","","","directed","cap","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[]),
        ('{"","","","directed","cap","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[]),
        ('{"","","","directed","cap","global","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[]),
        ('{"","","directed","cap","global","seq","path_seq","start_vid","end_vid","node","edge","cost","agg_cost"}'::TEXT[]);
    $$
);


-- parameter types
SELECT set_eq(
    $$SELECT  proallargtypes from pg_proc where proname = 'pgr_dijkstranear'$$,
    $$VALUES
    ('{25,2277,20,16,20,23,23,20,20,20,20,701,701}'::OID[]),
    ('{25,2277,2277,16,20,16,23,23,20,20,20,20,701,701}'::OID[]),
    ('{25,25,16,20,16,23,23,20,20,20,20,701,701}'::OID[]),
    ('{25,20,2277,16,20,23,23,20,20,20,20,701,701}'::OID[]);
$$
);

SELECT * FROM finish();
ROLLBACK;
