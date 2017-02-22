\i setup.sql

SELECT plan(47);


SELECT has_function('pgr_bddijkstra',
    ARRAY['text', 'integer', 'integer', 'boolean', 'boolean']);

SELECT function_returns('pgr_bddijkstra',
    ARRAY['text', 'integer', 'integer', 'boolean', 'boolean'],
    'setof pgr_costresult');



--with reverse cost
SELECT style_old_dijkstra_with('pgr_bddijkstra', ',2,3, true, true)');
SELECT style_old_dijkstra_no_rev('pgr_bddijkstra', ',2,3, true, false)');



SELECT finish();
ROLLBACK;
