\i setup.sql

UPDATE edge_table SET cost = sign(cost), reverse_cost = sign(reverse_cost);
SELECT plan(56);


SELECT has_function('pgr_ksp',
    ARRAY['text', 'bigint', 'bigint', 'integer', 'boolean', 'boolean']);

SELECT function_returns('pgr_ksp',
    ARRAY['text', 'bigint', 'bigint', 'integer', 'boolean', 'boolean'],
    'setof record');


-- ONE TO ONE
SELECT style_dijkstra('pgr_ksp', ',2,3,2, true, false)');


SELECT finish();
ROLLBACK;
