\i setup.sql

SELECT plan(47);


SELECT has_function('pgr_apspwarshall',
    ARRAY['text', 'boolean', 'boolean']);

SELECT function_returns('pgr_apspwarshall',
    ARRAY['text', 'boolean', 'boolean'],
    'setof pgr_costresult');

SELECT style_old_dijkstra_with('pgr_apspwarshall', ', true, true)');
SELECT style_old_dijkstra_no_rev('pgr_apspwarshall', ', true, false)');


SELECT finish();
ROLLBACK;
