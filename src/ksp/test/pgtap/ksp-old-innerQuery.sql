\i setup.sql

SELECT plan(47);


SELECT has_function('pgr_ksp',
    ARRAY['text', 'integer', 'integer', 'integer', 'boolean']);

SELECT function_returns('pgr_ksp',
    ARRAY['text', 'integer', 'integer', 'integer', 'boolean'],
    'setof pgr_costresult3');


-- ONE TO ONE
SELECT style_old_dijkstra_with('pgr_ksp', ',2,3,2, true)');
SELECT style_old_dijkstra_no_rev('pgr_ksp', ',2,3,2, false)');


SELECT finish();
ROLLBACK;
