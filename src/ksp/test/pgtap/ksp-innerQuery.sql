\i setup.sql

SELECT plan(47);


SELECT has_function('pgr_ksp',
    ARRAY['text', 'bigint', 'bigint', 'integer', 'boolean', 'boolean']);

SELECT function_returns('pgr_ksp',
    ARRAY['text', 'bigint', 'bigint', 'integer', 'boolean', 'boolean'],
    'setof record');


-- ONE TO ONE
SELECT style_dijkstra('pgr_ksp', ',2,3,2, true, false)');


SELECT finish();
ROLLBACK;
